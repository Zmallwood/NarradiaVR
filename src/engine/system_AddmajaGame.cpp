#include "Pch.h"
#include "system_AddmajaGame.h"
#include "core/xr/common/func_Common.h"
#include "core/xr/options/system_Options.h"
#include "core/xr/program/system_OpenXrProgram.h"
#include "core/xr/platform/system_AndroidPlatform.h"
#include "core/xr/graphics/system_GraphicsGL.h"
#include "core/system_Loader.h"
#include "core/system_GameLoop.h"
#include "core/system_SceneManager.h"
#include "system_OptionsManager.h"
#include "system_AndroidAppManager.h"

namespace nar {
   void AddmajaGame::Run(android_app *app) {
      GET(AndroidAppManager)->set_app(app);
      InitGame();
      GET(GameLoop)->Run();
      CleanupGame();
   }

   bool AddmajaGame::InitGame() {
      GET(AndroidAppManager)->Init();
      GET(Loader)->Init();
      GET(OpenXrProgram)->Init();
      if (!GET(OptionsManager)->Init())
         return false;
      GET(GraphicsGL)->UpdateOptions();
      GET(SceneManager)->InitScenes();
      return true;
   }

   void AddmajaGame::CleanupGame() {
      GET(AndroidAppManager)->Cleanup();
      nar::DisposeAllSingletons();
   }
}