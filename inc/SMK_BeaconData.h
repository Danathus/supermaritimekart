#ifndef SMK_BEACON_DATA__H
#define SMK_BEACON_DATA__H

//////////////////////////////////////////////////////////////////////////////

#include <string>

#include <NetSetGo/NetCore/Serialization.h> // for Serializable class

//////////////////////////////////////////////////////////////////////////////

class BeaconData : public Serializable
{
public:
   struct Data
   {
      unsigned char mMapNameLength;
      char mMapName[64];

      Data();
      bool SetMapName(const std::string& name);
   };

   BeaconData() : Serializable() {}

   Data& GetData() { return mData; }
   const Data& GetData() const { return mData; }

   // implementations of pure virtual methods
   size_t Serialize(unsigned char* buffer, size_t numBytes) const;
   size_t Deserialize(const unsigned char* buffer, size_t numBytes);
   //
   size_t GetSize() const;

protected:
private:
   Data mData;
};

//////////////////////////////////////////////////////////////////////////////

#endif // SMK_BEACON_DATA__H
