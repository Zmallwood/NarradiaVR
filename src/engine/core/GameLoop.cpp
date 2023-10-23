#include "Pch.h"
#include "GameLoop.h"
#include "xr/program/OpenXrProgram.h"
#include "engine/AndroidAppManager.h"
#include "EventReader.h"
#include "EventPoller.h"

namespace nar {
  ///
  /// Enter the main game loop and stay there until game exit.
  ///
  void GameLoop::Run() {
    auto app = AndroidAppManager::Get()->app();

    while (app->destroyRequested == 0) {
      EventReader::Get()->ReadEvents();
      EventPoller::Get()->PollEvents();

      if (exit_render_loop_) {
        ANativeActivity_finish(app->activity);
        continue;
      }

      if (!OpenXrProgram::Get()->IsSessionRunning()) {
        // Throttle loop since xrWaitFrame won't be called.
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        continue;
      }

      OpenXrProgram::Get()->PollActions();
      OpenXrProgram::Get()->RenderFrame();
    }
  }
}