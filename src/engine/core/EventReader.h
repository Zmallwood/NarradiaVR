#pragma once

namespace nar {
  class EventReader : public Singleton<EventReader> {
   public:
    void ReadEvents();
  };
}