/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "system_GameLoop.h"
#include "xr/program/system_OpenXrProgram.h"
#include "xr/program/input_actions/system_InputActionsHandler.h"
#include "engine/system_AndroidAppManager.h"
#include "system_SystemEventPoller.h"
#include "system_SceneManager.h"

namespace nar {
   /** Enter the main game loop and stay there until game exit.
    */
   void GameLoop::Run() {
      while (GET(AndroidAppManager)->app()->destroyRequested == 0) {
         GET(SystemEventPoller)->ReadAndPollSystemEvents();

         if (QuittingGameIfRequested())
            continue;

         if (ThrottlingGameIfSessionNotRunning())
            continue;

         GET(InputActionsHandler)->PollInputActions();

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