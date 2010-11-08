#ifndef FLOW_CONTROL__H
#define FLOW_CONTROL__H

////////////////////////////////////////////////////////////////////////////////

#include <NetCore/NetCoreExport.h>

////////////////////////////////////////////////////////////////////////////////

namespace net {

   class NETCORE_EXPORT FlowControl
   {
   public:
      FlowControl();

      void Reset();
      void Update(float deltaTime, float rtt);
      float GetSendRate() const;

   private:
      enum Mode
      {
         Good,
         Bad
      };

      Mode mMode;
      float mPenaltyTime;
      float mGoodConditionsTime;
      float mPenaltyReductionAccumulator;
   };

} // namespace net

////////////////////////////////////////////////////////////////////////////////

#endif // FLOW_CONTROL__H
