#include "Pch.h"
#include "system_AndroidAppManager.h"
#include "system_VRApp.h"

namespace nar {
   void AndroidAppManager::Init() {
      JNIEnv *env;
      app_->activity->vm->AttachCurrentThread(&env, nullptr);
      app_->userData = &appState_;
      app_->onAppCmd = VRApp::HandleAppCmd;
   }

   void AndroidAppManager::Cleanup() {
      app_->activity->vm->DetachCurrentThread();
   }
}