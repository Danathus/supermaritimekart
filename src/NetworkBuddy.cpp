#include <NetworkBuddy.h>

#include <cassert>

#include <dtCore/system.h>
#include <dtCore/timer.h>
#include <dtGame/basemessages.h>
#include <dtUtil/log.h>

#include <DeltaNetworkAdapter/NetworkMessages.h>
#include <DeltaNetworkAdapter/NetworkMessage.h>
#include <DeltaNetworkAdapter/SynchronizedEventMessage.h>

#include <NetCore/netassert.h>
#include <NetCore/NetworkEngine.h>

//#include <CCTS_ActorLibrary/IdentityAssignmentManager.h>
//#include <CCTS_ActorLibrary/RoleActor.h>
#include <DeltaNetworkAdapter/NetworkEngineComponent.h>

////////////////////////////////////////////////////////////////////////////////

//using namespace CCTS;

NetworkBuddy* NetworkBuddy::mpSelf = NULL;

////////////////////////////////////////////////////////////////////////////////

static std::string GetIdentifierFromMachineInfo(const dtGame::MachineInfo* machineInfo)
{
   std::string identifier = "<null>"; // = message.GetSource().GetUniqueId().ToString();
   {
      if (machineInfo)
      {
#if 0
         const RoleActor* role = IdentityAssignmentManager::GetRef().GetIdentity(machineInfo);
         if (role)
         {
            identifier = role->GetFullName();
         }
         else
#endif
         {
            std::stringstream strstrm;
            strstrm << "machine ";
            strstrm << machineInfo->GetUniqueId().ToString();
            identifier = strstrm.str();
         }
      }
   }
   return identifier;
}

////////////////////////////////////////////////////////////////////////////////

bool NetworkBuddy::SerializeMessage(dtUtil::DataStream& stream, const dtGame::Message* message, size_t maxPayloadSize) const
{
   const bool success = GetNetworkComponent()->SerializeMessage(stream, message, maxPayloadSize);
   return success;
}

////////////////////////////////////////////////////////////////////////////////
NetworkBuddy& NetworkBuddy::GetRef()
{
   if (!mpSelf)
   {
      mpSelf = new NetworkBuddy();
   }
   return *mpSelf;
}

bool NetworkBuddy::IsServer() const
{
   const bool isServer = net::NetworkEngine::GetRef().GetMesh().IsRunning();
   return isServer;
}

int NetworkBuddy::GetMaxAvailablePacketSize() const
{
   const int maxAvailablePacketSize = net::NetworkEngine::GetRef().GetNode().GetMaxGuaranteedPacketPayloadSize();
   return maxAvailablePacketSize;
}

#if 0
////////////////////////////////////////////////////////////////////////////////
bool NetworkBuddy::SendNetworkMessage(dtGame::Message& message, const CCTS::RoleActor* destination, TransmissionProtocol protocol)
{
   const bool success = SendNetworkMessage(message, *CCTS::IdentityAssignmentManager::GetRef().GetMachineInfo(destination), protocol);
   return success;
}
#endif

////////////////////////////////////////////////////////////////////////////////
bool NetworkBuddy::SendNetworkMessage(dtGame::Message& message, net::NodeID destination, TransmissionProtocol protocol)
{
   bool success = true;

   if (IsShutdownProcessStarted())
   {
      printf("NetworkBuddy::SendNetworkMessage() called when IsShutdownProcessStarted() is true; aborting...\n");
      // accept no more requests to send
      success = false;
      return success;
   }

   // make sure source machine info is set to us
   message.SetSource(GetGameManager()->GetMachineInfo());

   // make sure the destination machine info is set appropriately
   if (!message.GetDestination())
   {
      const dtGame::MachineInfo* machineInfo = GetMachineInfo(destination);
      message.SetDestination(machineInfo);
   }

   if (protocol & TRANSPORT_FLOWCONTROLLED)
   {
      // wrap it up in a network message and send it to ourselves for more detailed processing
      dtCore::RefPtr<DeltaNetworkAdapter::NetworkMessage> networkMessage;
      CreateMessage(DeltaNetworkAdapter::NetworkMessageType::INFO_NETWORK_MESSAGE, networkMessage);
      networkMessage->SetDestination(destination);
      networkMessage->SetProtocol(protocol);
      networkMessage->SetPayload(&message);
      GetGameManager()->SendNetworkMessage(*networkMessage);
   }
   else
   {
      static size_t bufferLen = net::NetworkEngine::GetRef().GetNode().GetMaxPacketSize();
      dtUtil::DataStream stream(new char[bufferLen], bufferLen);

      // stuff a tag in front to indicate we're "fast and loose"
      stream << int(TRANSPORT_FAST_AND_LOOSE);

      // write the message -- allow room for everything but the tag
      const size_t allowedPayloadSize = bufferLen - sizeof(int);
      success = success && SerializeMessage(stream, &message, allowedPayloadSize);

      // check to see if the data written pushed us over the edge
      if (!success)
      {
         // this last message was too big to fit at the end of the buffer
         // set an error code and/or print a failure message
         dtUtil::Log::GetInstance().LogMessage(dtUtil::Log::LOG_ERROR, __FUNCTION__, __LINE__,
            "message of type %s too big; message size %d bytes, max payload size %d bytes\n",
            message.GetMessageType().GetName().c_str(),
            stream.GetWritePosition(), allowedPayloadSize);
      }
      else
      {
         // if we have data to send, issue it out
         if (stream.GetWritePosition() > 0)
         {
            //printf("NetworkBuddy::SendNetworkMessage()[%s:%d] sending a fast-and-loose packet\n", __FILE__, __LINE__);
            success = net::NetworkEngine::GetRef().GetNode().SendPacket(destination, (const unsigned char *)stream.GetBuffer(), stream.GetWritePosition());
            netassert(success);
         }
      }
   }

   return success;
}

////////////////////////////////////////////////////////////////////////////////
bool NetworkBuddy::SendNetworkMessage(dtGame::Message& message, const dtGame::MachineInfo& destination, TransmissionProtocol protocol)
{
   const net::NodeID destinationNodeID = GetNodeID(&destination);
   const bool success = SendNetworkMessage(message, destinationNodeID, protocol);
   return success;
}

////////////////////////////////////////////////////////////////////////////////
bool NetworkBuddy::MulticastNetworkMessage(const net::Address& address, const dtGame::Message& message)
{
   bool success = true;

   {
      static size_t bufferLen = net::NetworkEngine::GetRef().GetNode().GetMaxPacketSize();
      dtUtil::DataStream stream(new char[bufferLen], bufferLen);

      // write the message (multicast is fast-and-loose only -- so allow entire packet space)
      success = success && SerializeMessage(stream, &message, bufferLen);

      // check to see if the data written pushed us over the edge
      if (!success)
      {
         // this message was too big to fit at the end of the buffer
         // set an error code and/or print a failure message
         dtUtil::Log::GetInstance().LogMessage(dtUtil::Log::LOG_ERROR, __FUNCTION__, __LINE__,
            "message of type %s too big; message size %d bytes, max packet size %d bytes\n",
            message.GetMessageType().GetName().c_str(),
            stream.GetWritePosition(), bufferLen);
      }
      else
      {
         // if we have data to send, issue it out
         if (stream.GetWritePosition() > 0)
         {
            success = net::NetworkEngine::GetRef().GetNode().MulticastPacket(address, (const unsigned char *)stream.GetBuffer(), stream.GetWritePosition());
            netassert(success);
         }
      }
   }

   return success;
}

////////////////////////////////////////////////////////////////////////////////
dtCore::RefPtr<dtGame::Message> NetworkBuddy::DeserializeNetworkMessage(net::NodeID senderNodeID, dtUtil::DataStream& stream)
{
   dtCore::RefPtr<dtGame::Message> message;

   message = GetNetworkComponent()->DeserializeNetworkMessage(senderNodeID, stream);

   return message;
}

#if 0
////////////////////////////////////////////////////////////////////////////////
void NetworkBuddy::TagMessageForLogging(const dtGame::Message& message)
{
   const dtGame::MachineInfo* loggerMachineInfo = IdentityAssignmentManager::GetRef().GetMachineInfo(IdentityAssignmentManager::GetRef().GetLoggerRole());

   if (loggerMachineInfo)
   {
      const std::string sourceID      = GetIdentifierFromMachineInfo(&message.GetSource());
      const std::string destinationID = GetIdentifierFromMachineInfo(message.GetDestination());

      dtCore::RefPtr<CCTS::SynchronizedEventMessage> syncEventMessage;
      CreateMessage(CCTS::UtilMessageType::INFO_SYNCHRONIZED_EVENT_MESSAGE, syncEventMessage);
      syncEventMessage->SetSimTime(GetGameManager()->GetSimulationTime());
      syncEventMessage->SetSourceID(sourceID);
      syncEventMessage->SetDestinationID(destinationID);
      syncEventMessage->SetPayload(&message);

      SendNetworkMessage(*syncEventMessage, *loggerMachineInfo);
   }
   else
   {
      // no logger detected -- drop message for now... (?)
   }
}
#endif

////////////////////////////////////////////////////////////////////////////////
void NetworkBuddy::StartShutdownProcess()
{
   // start shutdown process, so over the course of several frames, we finish up
   mIsShuttingDown = true;
}

////////////////////////////////////////////////////////////////////////////////
bool NetworkBuddy::IsShutdownProcessFinished() const
{
   const bool allQueuesEmpty = GetNetworkComponent()->AllOutgoingQueuesEmpty();
   const bool allFinished = IsShutdownProcessStarted() && allQueuesEmpty;
   return allFinished;
}

////////////////////////////////////////////////////////////////////////////////
void NetworkBuddy::WaitForShutdownProcessToFinish()
{
   dtCore::Timer timer;
   const dtCore::Timer_t initialRealClockTime = timer.Tick();
   dtCore::Timer_t previousRealClockTime = initialRealClockTime;
   const double initialSimulationTime = dtCore::System::GetInstance().GetSimulationTime();

   dtCore::RefPtr<dtGame::TickMessage> tickMessage;
   GetGameManager()->GetMessageFactory().CreateMessage(dtGame::MessageType::TICK_LOCAL, tickMessage);
   {
      tickMessage->SetDeltaSimTime(float(0));
      tickMessage->SetDeltaRealTime(float(0));
      tickMessage->SetSimTimeScale(GetGameManager()->GetTimeScale());
      tickMessage->SetDestination(&GetGameManager()->GetMachineInfo());
      tickMessage->SetSimulationTime(dtCore::System::GetInstance().GetSimulationTime());
   }

   // note this may not be the appropriate level to write this functionality
   // (it may have to go higher, somewhere we can tick Delta)
   if (mIsShuttingDown) while (!IsShutdownProcessFinished())
   {
      // update the tick message
      {
         const dtCore::Timer_t currentRealClockTime = timer.Tick();

         tickMessage->SetDeltaRealTime(timer.DeltaSec(previousRealClockTime, currentRealClockTime));
         tickMessage->SetDeltaSimTime(tickMessage->GetDeltaRealTime() * tickMessage->GetSimTimeScale());
         tickMessage->SetSimulationTime(initialSimulationTime + timer.DeltaSec(initialRealClockTime, currentRealClockTime) * tickMessage->GetSimTimeScale());

         previousRealClockTime = currentRealClockTime;
      }

      // tick the network component...
      GetNetworkComponent()->ProcessMessage(*tickMessage);

      // delay for some period of time
      dtCore::AppSleep(1000 / 60); // 1/60th of a second should do us
   }

}

////////////////////////////////////////////////////////////////////////////////
NetworkBuddy::NetworkBuddy()
   : mpGameManager(NULL)
   , mpNetComponent(NULL)
   , mIsShuttingDown(false)
{
   //
}

////////////////////////////////////////////////////////////////////////////////
NetworkBuddy::~NetworkBuddy()
{
   mpGameManager = NULL;
   mpNetComponent = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void NetworkBuddy::Destroy()
{
   if (mpSelf)
   {
      delete mpSelf;
      mpSelf = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
const dtGame::MachineInfo* NetworkBuddy::GetMachineInfo(net::NodeID nodeID) const
{
   assert(GetNetworkComponent());
   const dtGame::MachineInfo* machineInfo = GetNetworkComponent()->GetMachineInfo(nodeID);
   return machineInfo;
}

////////////////////////////////////////////////////////////////////////////////
const dtGame::MachineInfo* NetworkBuddy::GetMachineInfo(const std::string& uniqueID) const
{
   assert(GetNetworkComponent());
   const dtGame::MachineInfo* machineInfo = GetNetworkComponent()->GetMachineInfo(uniqueID);
   return machineInfo;
}

////////////////////////////////////////////////////////////////////////////////
net::NodeID NetworkBuddy::GetNodeID(const dtGame::MachineInfo* machineInfo) const
{
   assert(GetNetworkComponent());
   net::NodeID nodeID = GetNetworkComponent()->GetNodeID(machineInfo);
   return nodeID;
}

////////////////////////////////////////////////////////////////////////////////
const MachineInfoList& NetworkBuddy::GetConnectedMachineList() const
{
   return GetNetworkComponent()->GetConnectedMachineList();
}

////////////////////////////////////////////////////////////////////////////////