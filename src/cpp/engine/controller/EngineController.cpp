/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "EngineController.h"
#include "SceneManagerController.h"
#include "SystemEventPollerController.h"
#include "engine/model/AndroidVRAppManager.h"
#include "engine/model/Engine.h"
#include "engine/model/OpenXrProgram.h"
#include "func/PollInputActions.h"
#include "func/ReadSystemEvents.h"
#include <thread>

namespace nar {
    /**
     * Read and handle system events and player input actions.
     */
    void EngineController::HandleInput() {
        ReadSystemEvents();
        SystemEventPollerController::Get()->PollSystemEvents();
        PollInputActions();
        SceneManagerController::Get()->HandleInput();
    }

    /**
     * Handle game flow regarding changing scens and exiting the game.
     */
    void EngineController::UpdateGameFlow() {
        if (QuittingGameIfRequested() || ThrottlingGameIfSessionNotRunning())
            Engine::Get()->set_skip_frame(true);
        else
            Engine::Get()->set_skip_frame(false);

        auto game_is_running = AndroidVRAppManager::Get()->app()->destroyRequested == 0;
        Engine::Get()->set_game_is_running(game_is_running);

        SceneManagerController::Get()->UpdateGameFlow();
    }

    /**
     * Check if has been requested to exit the game.
     */
    bool EngineController::QuittingGameIfRequested() {
        if (Engine::Get()->exit_render_loop()) {
            ANativeActivity_finish(AndroidVRAppManager::Get()->app()->activity);
            return true;
        }
        return false;
    }

    /**
     * Throttle game if session is not running.
     */
    bool EngineController::ThrottlingGameIfSessionNotRunning() {
        if (!OpenXrProgram::Get()->IsSessionRunning()) {
            // Throttle loop since xrWaitFrame won't be called.
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            return true;
        }
        return false;
    }
}
