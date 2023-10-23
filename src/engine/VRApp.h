#pragma once

struct android_app;

namespace nar {
  class VRApp {
   public:
    static void HandleAppCmd(android_app *app, int32_t cmd);
  };
}