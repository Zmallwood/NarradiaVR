#pragma once

namespace nar {
   /**
    * Top most class in the project structure, called from android_main().
    */
   class AddmajaGame : public Singleton<AddmajaGame> {
     public:
      /**
       * Start new game instance.
       */
      void Run(android_app *app);

     private:
      /**
       * Do all initialization before entering game loop
       */
      bool InitGame();

      /**
       * Cleanup after exiting game loop
       */
      void CleanupGame();
   };
}