/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Engine.h"
#include "OpenXrProgram.h"
#include "engine/view/GraphicsGLView.h"
#include "Loader.h"
#include "OptionsManager.h"
#include "AndroidVRAppManager.h"
#include "ImageBank.h"
#include "world.generation/model/MapGenerator.h"
#include "ModelBank.h"

namespace nar {
   void Engine::Init(android_app *app) {
      app_ = app;
      srand(time(0));
      AndroidVRAppManager::Get()->set_app(app);
      AndroidVRAppManager::Get()->Init();
      Loader::Get()->Init();
      OpenXrProgram::Get()->Init();
      OptionsManager::Get()->Init();
      GraphicsGLView::Get()->UpdateOptions();
      ImageBank::Get()->LoadImages();
      ModelBank::Get()->LoadModels();
      MapGenerator::Get()->GenerateMapArea();
   }

   Engine::~Engine() {
      AndroidVRAppManager::Get()->Cleanup();
   }

   void Engine::UpdateGameLogic() {
      if (!skip_frame_) {
         SceneManager::Get()->UpdateGameLogic();
      }
   }
}
