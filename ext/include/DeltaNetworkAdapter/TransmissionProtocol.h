#ifndef TRANSMISSION_PROTOCOL__H
#define TRANSMISSION_PROTOCOL__H

////////////////////////////////////////////////////////////////////////////////

typedef int TransmissionProtocol;

enum TransmissionProtocolFeatures
{
   // basic building block modes of functionality; ///////////////////////
   //   see below for recommended composites
   TRANSPORT_FLOWCONTROLLED = 1 << 0, // sent at regular intervals, adaptive flow managed intelligently
   TRANSPORT_SEQUENCED      = 1 << 1, // guaranteed to not be received (processed) out of the order sent
   TRANSPORT_SYNCHRONIZED   = 1 << 2, // guaranteed to be received while connection persists

   // these are the two recommended modes for general usage //////////////
   //   most efficient, no guarantees (good for high-frequency lossy throughput)
   TRANSPORT_FAST_AND_LOOSE  = 0,
   //   TCP-like managed data that will get there at a steady pace, in sequence
   TRANSPORT_SLOW_AND_STEADY = TRANSPORT_FLOWCONTROLLED | TRANSPORT_SEQUENCED | TRANSPORT_SYNCHRONIZED,

   TRANSPORT_DEFAULT = TRANSPORT_SLOW_AND_STEADY // currently the full feature set that's implemented
};

////////////////////////////////////////////////////////////////////////////////

#endif // TRANSMISSION_PROTOCOL__H
