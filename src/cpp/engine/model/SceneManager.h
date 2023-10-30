/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "IScene.h"
#include "SceneNames.h"

namespace nar {
   class SceneManager {
     public:
      SceneManager();

      void UpdateGameLogic();

      std::map<SceneNames, std::shared_ptr<IScene>> scenes() {
         return scenes_;
      }

      SceneNames current_scene() {
         return current_scene_;
      }

      void set_current_scene(SceneNames value) {
         current_scene_ = value;
      }

      SceneNames new_scene() {
         return new_scene_;
      }

      void set_new_scene(SceneNames value) {
         new_scene_ = value;
      }

     private:
      std::map<SceneNames, std::shared_ptr<IScene>> scenes_;
      SceneNames current_scene_ = SceneNames::None;
      SceneNames new_scene_ = SceneNames::None;
   };
}