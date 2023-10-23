#include "Pch.h"
#include "engine/Game.h"
#include "Win32CompileSpecific.h"

/// This is the main entry point of a native application that is using
/// android_native_app_glue.  It runs in its own thread, with its own
/// event loop for receiving input events and doing other things.
void android_main(struct android_app *app) {
  nar::Game::Get()->Run(app); // Run new game instance
}
