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
#include "engine/model/SceneManager.h"
#include "ISceneView.h"
#include "engine/model/SceneNames.h"

namespace nar {
   class SceneManagerView {
     public:
      SceneManagerView(std::shared_ptr<SceneManager> scene_manager);
      void Render();

     private:
      std::shared_ptr<SceneManager> scene_manager_;
      std::map<SceneNames, std::shared_ptr<ISceneView>> scene_views_;
   };
}