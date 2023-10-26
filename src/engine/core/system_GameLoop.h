#pragma once
#include "xr/program/system_OpenXrProgram.h"

namespace nar {
   class OpenXrProgram;

   class GameLoop : public Singleton<GameLoop> {
     public:
      /**
       * Enter the main game loop and stay there until game exit.
       */
      void Run();

      void set_request_restart(bool value) {
         request_restart_ = value;
      }

      void set_exit_render_loop(bool value) {
         exit_render_loop_ = value;
      }

     private:
      bool QuittingGameIfRequested();
      bool ThrottlingGameIfSessionNotRunning();

      bool request_restart_ = false;
      bool exit_render_loop_ = false;
   };
}