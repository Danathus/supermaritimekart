#include <SMK_BeaconData.h>

#include <cassert>

//////////////////////////////////////////////////////////////////////////////

BeaconData::Data::Data()
   : mMapNameLength(0)
{
   SetMapName("<Undefined>");
}

bool BeaconData::Data::SetMapName(const std::string& name)
{
   bool success = name.length() < 63;

   if (success)
   {
      mMapNameLength = name.length();
      memcpy(mMapName, &name[0], mMapNameLength);
   }

   return success;
}

//////////////////////////////////////////////////////////////////////////////

size_t BeaconData::Serialize(unsigned char* buffer, size_t numBytes) const
{
   bool success = true;

   size_t position = 0;

   assert(mData.mMapNameLength < 63);

   if (success = success && (position + sizeof(mData.mMapNameLength) <= numBytes)) { position += WriteByte(&buffer[position], mData.mMapNameLength);    }
   if (success = success && (position +        mData.mMapNameLength  <= numBytes)) { memcpy(&buffer[position], &mData.mMapName[0], mData.mMapNameLength); position += mData.mMapNameLength; }

   return position;
}

size_t BeaconData::Deserialize(const unsigned char* buffer, size_t numBytes)
{
   bool success = true;

   size_t position = 0;

   if (success = success && (position + sizeof(mData.mMapNameLength) <= numBytes))
   {
      position += ReadByte(&buffer[position], mData.mMapNameLength);
      success = success && mData.mMapNameLength < 63;
   }
   if (success = success && (position +        mData.mMapNameLength  <= numBytes))
   {
      memcpy(mData.mMapName, &buffer[position], mData.mMapNameLength); position += mData.mMapNameLength;
      mData.mMapName[mData.mMapNameLength] = '\0';
   }

   return position;
}

size_t BeaconData::GetSize() const
{
   const size_t size =
      sizeof(char) +   // mMapNameLength
      sizeof(char)*64; // mMapName
   return size;
}

//////////////////////////////////////////////////////////////////////////////
