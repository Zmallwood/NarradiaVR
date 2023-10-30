/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "Pch.h"
#include "Game.h"
#include "engine/model/Engine.h"
#include "engine/view/EngineView.h"
#include "engine/controller/EngineController.h"

namespace nar {
   /** Start new game instance.
    */
   void Game::Run(android_app *app) {
      srand(time(0));
      auto &engine = *Engine::Get(); // Cosmos
      engine.Init(app);
      EngineView engine_view(engine);
      EngineController engine_controller(engine);

      while (engine.game_is_running()) {
         engine_controller.HandleInput();
         engine_controller.UpdateGameFlow();
         engine.UpdateGameLogic();
         engine_view.Render();
      }

      DisposeAllSingletons();
   }
}