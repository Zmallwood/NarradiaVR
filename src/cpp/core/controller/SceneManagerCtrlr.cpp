/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "SceneManagerCtrlr.h"
#include "../../core/model/SceneManager.h"
#include "../../scenes/intro/controller/IntroSceneController.h"
#include "../../scenes/main/controller/MainSceneController.h"
#include "../../scenes/main_menu/controller/MainMenuSceneController.h"

namespace nar {
    /**
     * Add scene controllers.
     */
    SceneManagerCtrlr::SceneManagerCtrlr() {
        scene_controllers_.insert({SceneNames::Intro, std::make_shared<IntroSceneController>()});
        scene_controllers_.insert(
            {SceneNames::MainMenu, std::make_shared<MainMenuSceneController>()});
        scene_controllers_.insert({SceneNames::Main, std::make_shared<MainSceneController>()});
    }

    /**
     * Handle input for current scenej
     */
    void SceneManagerCtrlr::HandleInput() {
        if (scene_controllers_.count(SceneManager::Get()->current_scene()) != 0)
            scene_controllers_.at(SceneManager::Get()->current_scene())->HandleInput();
    }

    /**
     * Set current scene to a new scene, which may or may not be different,
     * and update game flow for the current scene.
     */
    void SceneManagerCtrlr::UpdateGameFlow() {
        SceneManager::Get()->set_current_scene(SceneManager::Get()->new_scene());
        if (scene_controllers_.count(SceneManager::Get()->current_scene()) != 0)
            scene_controllers_.at(SceneManager::Get()->current_scene())->UpdateGameFlow();
    }
}
