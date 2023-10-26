/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once

namespace nar {
   /** Top most class in the project structure, called from android_main().
    */
   class Game : public Singleton<Game> {
     public:
      void Run(android_app *app);

     private:
      bool InitGame();
      void CleanupGame();
   };
}