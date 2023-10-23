#pragma once
#include "core/AndroidAppState.h"

namespace nar {
  class AndroidAppManager : public Singleton<AndroidAppManager> {
   public:
    void Initialize();
    void Cleanup();

    android_app *app() {
      return app_;
    }

    void set_app(android_app *value) {
      app_ = value;
    }

   private:
    android_app *app_;
    AndroidAppState appState_;
  };
}