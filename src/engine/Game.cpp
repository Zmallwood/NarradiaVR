#include "Pch.h"
#include "Game.h"
#include "core/xr/common/Common.h"
#include "core/xr/options/Options.h"
#include "core/xr/program/OpenXrProgram.h"
#include "core/xr/platform_plugin/AndroidPlatformPlugin.h"
#include "core/xr/graphics_plugin/OpenGLESGraphicsPlugin.h"
#include "core/Loader.h"
#include "core/GameLoop.h"
#include "core/SceneManager.h"
#include "OptionsManager.h"
#include "AndroidAppManager.h"

namespace nar {
  /// Start new game instance.
  void Game::Run(android_app *app) {
    try {
      AndroidAppManager::Get()->set_app(app);
      InitializeGame();
      GameLoop::Get()->Run();
      CleanupGame();
    } catch (const std::exception &ex) {
      Log::Write(Log::Level::Error, ex.what());
    } catch (...) {
      Log::Write(Log::Level::Error, "Unknown Error");
    }
  }

  /// Do all initialization before entering game loop
  bool Game::InitializeGame() {
    AndroidAppManager::Get()->Initialize();
    Loader::Get()->Initialize();
    OpenXrProgram::Get()->Initialize();
    if (!OptionsManager::Get()->Initialize())
      return false;
    OpenGLESGraphicsPlugin::Get()->UpdateOptions();
    SceneManager::Get()->InitializeScenes();
    return true;
  }

  /// Cleanup after exiting game loop
  void Game::CleanupGame() {
    AndroidAppManager::Get()->Cleanup();
    nar::DisposeAllSingletons();
  }
}