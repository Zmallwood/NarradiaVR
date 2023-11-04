/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "../../core/model/SceneNames.h"
#include "ISceneCtrlr.h"
#include <map>
#include <memory>

namespace nar {
    /**
     * Holds all scene controllers and updates the one currently active.
     */
    class SceneManagerCtrlr : public Ston<SceneManagerCtrlr> {
      public:
        SceneManagerCtrlr();
        void HandleInput();
        void UpdateGameFlow();

      private:
        std::map<SceneNames, std::shared_ptr<ISceneCtrlr>> scene_controllers_;
    };
}
