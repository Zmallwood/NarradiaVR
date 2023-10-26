#pragma once

namespace nar {
   class SystemEventPollerController {
     public:
      void PollSystemEvents();

     private:
      const XrEventDataBaseHeader *TryReadNextEvent();
   };
}