#include "Pch.h"
#include "EventPoller.h"
#include "xr/program/OpenXrProgram.h"
#include "GameLoop.h"

namespace nar {
  void EventPoller::PollEvents() {
    OpenXrProgram::Get()->PollEvents();
  }
}