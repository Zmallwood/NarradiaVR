#pragma once

namespace nar {
  class SystemEventReader : public Singleton<SystemEventReader> {
   public:
    void ReadSystemEvents();
  };
}