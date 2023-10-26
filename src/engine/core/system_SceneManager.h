#pragma once
#include "enum_SceneNames.h"

namespace nar {
   class IScene;

   class SceneManager : public Singleton<SceneManager> {
     public:
      void InitScenes();
      void UpdateCurrentScene();
      void RenderCurrentScene();
      void ChangeScene(SceneNames new_scene);

     private:
      std::map<SceneNames, std::shared_ptr<IScene>> scenes_;
      SceneNames current_scene_ = SceneNames::None;
   };
}