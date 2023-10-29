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

#include "Pch.h"
#include "Engine.h"
#include "OpenXrProgram.h"
#include "engine/view/GraphicsGLView.h"
#include "Loader.h"
#include "OptionsManager.h"
#include "AndroidVRAppManager.h"
#include "ImageBank.h"
#include "world.generation/model/MapGenerator.h"

namespace nar {
   void Engine::Init(android_app *app) {
      app_ = app;
      AndroidVRAppManager::Get()->set_app(app);
      AndroidVRAppManager::Get()->Init();
      Loader::Get()->Init();
      OpenXrProgram::Get()->Init();
      OptionsManager::Get()->Init();
      GraphicsGLView::Get()->UpdateOptions();
      ImageBank::Get()->LoadImages();
      MapGenerator::Get()->GenerateMapArea();
   }

   Engine::~Engine() {
      AndroidVRAppManager::Get()->Cleanup();
   }

   void Engine::UpdateGameLogic() {
      if (!skip_frame_) {
         scene_manager_->UpdateGameLogic();
      }
   }
}