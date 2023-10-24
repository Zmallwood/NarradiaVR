#pragma once

namespace nar {
  ///
  /// Top most class in the project structure, called from android_main().
  ///
  class Game : public Singleton<Game> {
   public:
    void Run(android_app *app);

   private:
    bool InitGame();
    void CleanupGame();
  };
}