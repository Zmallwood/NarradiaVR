#pragma once

namespace nar {
  struct AndroidAppState {
    ANativeWindow *native_window = nullptr;
    bool resumed = false;
  };
}