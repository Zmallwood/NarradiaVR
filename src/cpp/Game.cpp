/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "Pch.h"
#include "Game.h"
#include "engine/model/Engine.h"
#include "engine/view/EngineView.h"
#include "engine/controller/EngineController.h"
#include "engine/model/SceneManager.h"
#include "engine/view/SceneManagerView.h"

namespace nar {
   /** Start new game instance.
    */
   void Game::Run(android_app *app) {
      srand(time(0));
      Engine::Touch();
      Engine::Get()->Init(app);
      SceneManager::Touch();
      SceneManagerView::Touch();
      EngineView::Touch();
      EngineController::Touch();

      while (Engine::Get()->game_is_running()) {
         EngineController::Get()->HandleInput();
         EngineController::Get()->UpdateGameFlow();
         Engine::Get()->UpdateGameLogic();
         EngineView::Get()->Render();
      }

      DisposeAllSingletons();
   }
}