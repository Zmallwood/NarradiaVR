/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once
#include "xr/program/system_OpenXrProgram.h"

namespace nar {
   class OpenXrProgram;

   class GameLoop : public Singleton<GameLoop> {
     public:
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