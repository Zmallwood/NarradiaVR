#include "Pch.h"
#include "Engine.h"
#include "system/system_OpenXrProgram.h"
#include "view/system_GraphicsGL.h"
#include "system/system_Loader.h"
#include "system/system_OptionsManager.h"
#include "system/system_AndroidAppManager.h"
#include "model/bank_ImageBank.h"

namespace nar {
   void Engine::Init(android_app *app) {
      app_ = app;
      GET(AndroidAppManager)->set_app(app);
      GET(AndroidAppManager)->Init();
      GET(Loader)->Init();
      GET(OpenXrProgram)->Init();
      GET(OptionsManager)->Init();
      GET(GraphicsGL)->UpdateOptions();
      //GET(SceneManagerOLD)->InitScenes();
      GET(ImageBank)->LoadImages();
   }

   Engine::~Engine() {
      GET(AndroidAppManager)->Cleanup();
      nar::DisposeAllSingletons();
   }

   void Engine::UpdateGameLogic() {
      if (!skip_frame_) {
         scene_manager_->UpdateGameLogic();
      }
   }
}