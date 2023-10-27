/* Copyright 2023 Zmallwood
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#pragma once

#include "engine/model/Engine.h"
#include "SceneManagerController.h"
#include "SystemEventReaderController.h"
#include "SystemEventPollerController.h"
#include "InputActionsController.h"

namespace nar {
   class EngineController {
     public:
      EngineController(Engine &engine)
          : engine_(engine),
            scene_manager_controller_(engine.scene_manager()) {
      }
      void HandleInput();
      void UpdateGameFlow();

     private:
      bool QuittingGameIfRequested();
      bool ThrottlingGameIfSessionNotRunning();

      Engine &engine_;
      SceneManagerController scene_manager_controller_;
      SystemEventReaderController system_event_reader_controller_;
      SystemEventPollerController system_event_poller_controller_;
      InputActionsController input_actions_controller_;
   };
}