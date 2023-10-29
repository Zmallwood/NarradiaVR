/* Copyright 2023 Zmallwood
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

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