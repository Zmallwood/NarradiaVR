/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "../../engine/model/SceneManager.h"
#include "../../engine/model/SceneNames.h"
#include "ISceneController.h"
#include <map>
#include <memory>

namespace nar {
    /**
     * Holds all scene controllers and updates the one currently active.
     */
    class SceneManagerController : public Singleton<SceneManagerController> {
      public:
        SceneManagerController();
        void HandleInput();
        void UpdateGameFlow();

      private:
        std::map<SceneNames, std::shared_ptr<ISceneController>> scene_controllers_;
    };
}
