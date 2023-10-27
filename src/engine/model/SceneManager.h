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