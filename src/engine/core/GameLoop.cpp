#include "Pch.h"
#include "GameLoop.h"
#include "xr/program/OpenXrProgram.h"
#include "engine/AndroidAppManager.h"
#include "SystemEventPoller.h"
#include "SceneManager.h"
#include "input/ControllerInput.h"

namespace nar {
  ///
  /// Enter the main game loop and stay there until game exit.
  ///
  void GameLoop::Run() {
    while (AndroidAppManager::Get()->app()->destroyRequested == 0) {
      SystemEventPoller::Get()->ReadAndPollSystemEvents();

      if (QuittingGameIfRequested())
        continue;

      if (ThrottlingGameIfSessionNotRunning())
        continue;

      ControllerInput::Get()->Reset();

      OpenXrProgram::Get()->PollInputActions();

      SceneManager::Get()->UpdateCurrentScene();
      SceneManager::Get()->RenderCurrentScene();
    }
  }

  bool GameLoop::QuittingGameIfRequested() {
    if (exit_render_loop_) {
      ANativeActivity_finish(AndroidAppManager::Get()->app()->activity);
      return true;
    }
    return false;
  }

  bool GameLoop::ThrottlingGameIfSessionNotRunning() {
    if (!OpenXrProgram::Get()->IsSessionRunning()) {
      // Throttle loop since xrWaitFrame won't be called.
      std::this_thread::sleep_for(std::chrono::milliseconds(250));
      return true;
    }
    return false;
  }
}