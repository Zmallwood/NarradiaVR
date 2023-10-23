#pragma once

namespace nar {
  class EventPoller : public Singleton<EventPoller> {
   public:
    void PollEvents();

   private:
    const XrEventDataBaseHeader *TryReadNextEvent();
  };
}