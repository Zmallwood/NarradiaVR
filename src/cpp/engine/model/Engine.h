/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "SceneManager.h"

struct android_app;

namespace nar {
   class Engine : public Singleton<Engine> {
     public:
      Engine();
      ~Engine();
      void Init(android_app *app);
      void UpdateGameLogic();

      bool game_is_running() {
         return game_is_running_;
      }

      void set_game_is_running(bool value) {
         game_is_running_ = value;
      }

      bool exit_render_loop() {
         return exit_render_loop_;
      }

      void set_exit_render_loop(bool value) {
         exit_render_loop_ = value;
      }

      bool skip_frame() {
         return skip_frame_;
      }

      void set_skip_frame(bool value) {
         skip_frame_ = value;
      }

     private:
      android_app *app_ = nullptr;
      bool game_is_running_ = true;
      bool exit_render_loop_ = false;
      bool skip_frame_ = false;
   };
}