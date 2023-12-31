/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Game.h"

/** This is the main entry point of a native application that is using
 *  android_native_app_glue.  It runs in its own thread, with its own
 *  event loop for receiving input events and doing other things.
 */
void android_main(struct android_app *app) {
   nar::Game().Run(app);
}
