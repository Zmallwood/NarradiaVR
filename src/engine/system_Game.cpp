/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "system_Game.h"
#include "core/xr/options/system_Options.h"
#include "core/xr/program/system_OpenXrProgram.h"
#include "core/xr/platform/system_AndroidPlatform.h"
#include "core/xr/graphics/system_GraphicsGL.h"
#include "core/system_Loader.h"
#include "core/system_GameLoop.h"
#include "core/system_SceneManager.h"
#include "system_OptionsManager.h"
#include "system_AndroidAppManager.h"

namespace nar {
   /** Start new game instance.
    */
   void Game::Run(android_app *app) {
      GET(AndroidAppManager)->set_app(app);
      InitGame();
      GET(GameLoop)->Run();
      CleanupGame();
   }

   /** Do all initialization before entering game loop
    */
   bool Game::InitGame() {
      GET(AndroidAppManager)->Init();
      GET(Loader)->Init();
      GET(OpenXrProgram)->Init();
      if (!GET(OptionsManager)->Init())
         return false;
      GET(GraphicsGL)->UpdateOptions();
      GET(SceneManager)->InitScenes();
      return true;
   }

   /** Cleanup after exiting game loop
    */
   void Game::CleanupGame() {
      GET(AndroidAppManager)->Cleanup();
      nar::DisposeAllSingletons();
   }
}