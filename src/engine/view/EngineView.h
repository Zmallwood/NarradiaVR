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
#include "SceneManagerView.h"

namespace nar {
   class EngineView {
     public:
      EngineView(Engine &engine)
          : engine_(engine),
            scene_manager_view_(engine.scene_manager()) {
      }
      void Render();

     private:
      Engine &engine_;
      SceneManagerView scene_manager_view_;
   };
}