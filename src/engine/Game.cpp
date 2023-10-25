#include "Pch.h"
#include "Game.h"
#include "core/xr/common/Common.h"
#include "core/xr/options/Options.h"
#include "core/xr/program/OpenXrProgram.h"
#include "core/xr/platform/AndroidPlatform.h"
#include "core/xr/graphics/GraphicsGL.h"
#include "core/Loader.h"
#include "core/GameLoop.h"
#include "core/SceneManager.h"
#include "OptionsManager.h"
#include "AndroidAppManager.h"

namespace nar {
  /**
   * Start new game instance.
   */
  void Game::Run(android_app *app) {
    try {
      GET(AndroidAppManager)->set_app(app);
      InitGame();
      GET(GameLoop)->Run();
      CleanupGame();
    }
    catch (const std::exception &ex) {
      Log::Write(Log::Level::Error, ex.what());
    }
    catch (...) {
      Log::Write(Log::Level::Error, "Unknown Error");
    }
  }

  /**
   * Do all initialization before entering game loop
   */
  bool Game::InitGame() {
    GET(AndroidAppManager)->Init();
    GET(Loader)->Init();
    GET(OpenXrProgram)->Init();
    if (!GET(OptionsManager)->Init())
      return false;
    GET(GraphicsGL)->UpdateOptions();
    GET(SceneManager)->InitScenes();
    return true;
  }

  /**
   * Cleanup after exiting game loop
   */
  void Game::CleanupGame() {
    GET(AndroidAppManager)->Cleanup();
    nar::DisposeAllSingletons();
  }
}