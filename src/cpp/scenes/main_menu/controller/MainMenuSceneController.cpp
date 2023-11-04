/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "MainMenuSceneController.h"
#include "../../../core/model/SceneManager.h"
#include "core/model/Engine.h"
#include "core/model/HandDeviceInput.h"

namespace nar {
    void MainMenuSceneController::HandleInput() {
    }

    void MainMenuSceneController::UpdateGameFlow() {
        if (HandDeviceInput::Get()->right_input_device()->HasBeenGrabbedPickResult()) {
            SceneManager::Get()->set_new_scene(SceneNames::Main);
        }
    }
}
