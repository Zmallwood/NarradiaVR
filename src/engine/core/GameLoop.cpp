#include "Pch.h"
#include "GameLoop.h"
#include "xr/program/OpenXrProgram.h"
#include "engine/AndroidAppManager.h"
#include "SystemEventPoller.h"
#include "SceneManager.h"

namespace nar {
  /**
   * Enter the main game loop and stay there until game exit.
   */
  void GameLoop::Run() {
    while (GET(AndroidAppManager)->app()->destroyRequested == 0) {
      GET(SystemEventPoller)->ReadAndPollSystemEvents();

      if (QuittingGameIfRequested())
        continue;

      if (ThrottlingGameIfSessionNotRunning())
        continue;

      GET(OpenXrProgram)->PollInputActions();

      GET(SceneManager)->UpdateCurrentScene();
      GET(SceneManager)->RenderCurrentScene();
    }
  }

  bool GameLoop::QuittingGameIfRequested() {
    if (exit_render_loop_) {
      ANativeActivity_finish(GET(AndroidAppManager)->app()->activity);
      return true;
    }
    return false;
  }

  bool GameLoop::ThrottlingGameIfSessionNotRunning() {
    if (!GET(OpenXrProgram)->IsSessionRunning()) {
      // Throttle loop since xrWaitFrame won't be called.
      std::this_thread::sleep_for(std::chrono::milliseconds(250));
      return true;
    }
    return false;
  }
}