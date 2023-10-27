#pragma once
#include "SceneManager.h"

struct android_app;

namespace nar {
   class Engine : public Singleton<Engine> {
     public:
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

      std::shared_ptr<SceneManager> scene_manager() {
         return scene_manager_;
      }

     private:
      android_app *app_ = nullptr;
      bool game_is_running_ = true;
      bool exit_render_loop_ = false;
      bool skip_frame_ = false;
      std::shared_ptr<SceneManager> scene_manager_ = std::make_shared<SceneManager>();
   };
}