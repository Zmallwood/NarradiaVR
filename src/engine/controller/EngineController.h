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