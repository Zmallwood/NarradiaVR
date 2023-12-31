/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "IntroSceneController.h"
#include "../../../core/model/SceneManager.h"
#include "core/model/Engine.h"
#include "core/model/HandDeviceInput.h"

namespace nar {
    void IntroSceneController::HandleInput() {
    }

    void IntroSceneController::UpdateGameFlow() {
        if (HandDeviceInput::Get()->right_input_device()->HasBeenGrabbedPickResult()) {
            SceneManager::Get()->set_new_scene(SceneNames::MainMenu);
        }
    }
}
