#include "Pch.h"
#include "AndroidAppManager.h"
#include "VRApp.h"

namespace nar {
  void AndroidAppManager::Initialize() {
    JNIEnv *env;
    app_->activity->vm->AttachCurrentThread(&env, nullptr);
    app_->userData = &appState_;
    app_->onAppCmd = VRApp::HandleAppCmd;
  }

  void AndroidAppManager::Cleanup() {
    app_->activity->vm->DetachCurrentThread();
  }
}