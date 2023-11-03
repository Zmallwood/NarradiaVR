/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "Game.h"
#include "engine/controller/EngineCtrlr.h"
#include "engine/model/Engine.h"
#include "engine/view/EngineView.h"

namespace nar {
    /**
     * Initialize and start new game instance.
     */
    void Game::Run(android_app *app) {
        Engine::Get()->Init(app);

        while (Engine::Get()->game_is_running()) { // Core game loop
            EngineCtrlr::Get()->HandleInput();     // Controller
            EngineCtrlr::Get()->UpdateGameFlow();  // Controller
            Engine::Get()->UpdateGameLogic();      // Model
            EngineView::Get()->Render();           // View
        }

        DisposeAllSingletons(); // Required to be done manually on android
    }
}
