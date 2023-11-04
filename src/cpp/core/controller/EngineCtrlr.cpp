/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "EngineCtrlr.h"
#include "../../core.input_actions/controller/InputPollController.h"
#include "SceneManagerCtrlr.h"
#include "SysEventPollCtrlr.h"
#include "core/model/AppManager.h"
#include "core/model/Engine.h"
#include "core/model/OpenXrProgram.h"
#include "func/ReadSystemEvents.h"
#include <thread>

namespace nar {
    /**
     * Read and handle system events and player input actions.
     */
    void EngineCtrlr::HandleInput() {
        ReadSystemEvents();
        SysEventPollCtrlr::Get()->PollSystemEvents();
        InputPollController::Get()->PollInputActions();
        SceneManagerCtrlr::Get()->HandleInput();
    }

    /**
     * Handle game flow regarding changing scens and exiting the game.
     */
    void EngineCtrlr::UpdateGameFlow() {
        if (QuittingGameIfRequested() || ThrottlingGameIfSessionNotRunning())
            Engine::Get()->set_skip_frame(true);
        else
            Engine::Get()->set_skip_frame(false);

        auto game_is_running = AppManager::Get()->app()->destroyRequested == 0;
        Engine::Get()->set_game_is_running(game_is_running);

        SceneManagerCtrlr::Get()->UpdateGameFlow();
    }

    /**
     * Check if has been requested to exit the game.
     */
    bool EngineCtrlr::QuittingGameIfRequested() {
        if (Engine::Get()->exit_render_loop()) {
            ANativeActivity_finish(AppManager::Get()->app()->activity);
            return true;
        }
        return false;
    }

    /**
     * Throttle game if session is not running.
     */
    bool EngineCtrlr::ThrottlingGameIfSessionNotRunning() {
        if (!OpenXrProgram::Get()->IsSessionRunning()) {
            // Throttle loop since xrWaitFrame won't be called.
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            return true;
        }
        return false;
    }
}
