#pragma once

namespace nar {
   class SystemEventPoller : public Singleton<SystemEventPoller> {
     public:
      void ReadAndPollSystemEvents();

     private:
      const XrEventDataBaseHeader *TryReadNextEvent();
   };
}