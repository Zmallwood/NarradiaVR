#include "Pch.h"
#include "Engine.h"
#include "engine/system/system_OpenXrProgram.h"
#include "engine/view/system_GraphicsGL.h"
#include "engine/system/system_Loader.h"
#include "engine/system/system_OptionsManager.h"
#include "AndroidVRAppManager.h"
#include "engine/model/ImageBank.h"

namespace nar {
   void Engine::Init(android_app *app) {
      app_ = app;
      GET(AndroidVRAppManager)->set_app(app);
      GET(AndroidVRAppManager)->Init();
      GET(Loader)->Init();
      GET(OpenXrProgram)->Init();
      GET(OptionsManager)->Init();
      GET(GraphicsGL)->UpdateOptions();
      //GET(SceneManagerOLD)->InitScenes();
      GET(ImageBank)->LoadImages();
   }

   Engine::~Engine() {
      GET(AndroidVRAppManager)->Cleanup();
      nar::DisposeAllSingletons();
   }

   void Engine::UpdateGameLogic() {
      if (!skip_frame_) {
         scene_manager_->UpdateGameLogic();
      }
   }
}