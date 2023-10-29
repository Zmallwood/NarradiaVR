/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "Game.h"
#include "engine/model/Engine.h"
#include "engine/view/EngineView.h"
#include "engine/controller/EngineController.h"

namespace nar {
   /** Start new game instance.
    */
   void Game::Run(android_app *app) {
      auto &engine = *Engine::Get(); // Cosmos
      engine.Init(app);
      EngineView engine_view(engine);
      EngineController engine_controller(engine);

      while (engine.game_is_running()) {
         engine_controller.HandleInput();
         engine_controller.UpdateGameFlow();
         engine.UpdateGameLogic();
         engine_view.Appear();
      }

      DisposeAllSingletons();
   }
}