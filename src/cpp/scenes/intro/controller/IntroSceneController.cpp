/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "IntroSceneController.h"
#include "engine/model/HandDeviceInput.h"
#include "engine/model/Engine.h"

namespace nar {
   void IntroSceneController::HandleInput() {
   }

   void IntroSceneController::UpdateGameFlow() {
      __android_log_print(ANDROID_LOG_INFO, "Narradia", "intro controller");
      if (HandDeviceInput::Get()->right_input_device()->HasBeenGrabbedPickResult()) {
         SceneManager::Get()->set_new_scene(SceneNames::MainMenu);
      }
   }
}