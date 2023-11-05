/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "ModelBank.h"
#include "../../core/model/AppManager.h"
#include "../../core.model.make/model/ModelCreator.h"

namespace nar {

   void ModelBank::LoadModels() {
      auto asset_manager = AppManager::Get()->app()->activity->assetManager;
      AAssetDir *dir = AAssetManager_openDir(asset_manager, "");
      const char *file_name = AAssetDir_getNextFileName(dir);

      while (file_name != nullptr) {
         if (Util::GetFileExtension(file_name) == "dae") {
            std::string_view file_name_with_ext = file_name;
            LoadSingleModel(file_name);
         }

         file_name = AAssetDir_getNextFileName(dir);
      }
   }

   void ModelBank::LoadSingleModel(std::string_view file_name) {
      Assimp::Importer importer;
      Assimp::AndroidJNIIOSystem *ioSystem =
          new Assimp::AndroidJNIIOSystem(AppManager::Get()->app()->activity);
      if (nullptr != ioSystem) {
         importer.SetIOHandler(ioSystem);
      }
      const aiScene *scene = importer.ReadFile(file_name.data(), 0);
      auto model = ModelCreator::Get()->CreateModel(scene);
      auto file_name_no_ext = Util::GetFileNameNoExt(file_name);
      models_.insert({file_name_no_ext, model});
   }

   FileData ModelBank::GetAssetData(const char *relative_path) {
      auto asset_manager = AppManager::Get()->app()->activity->assetManager;
      AAsset *asset = AAssetManager_open(asset_manager, relative_path, AASSET_MODE_STREAMING);

      return (FileData){AAsset_getLength(asset), AAsset_getBuffer(asset), asset};
   }

   std::shared_ptr<Model> ModelBank::GetModel(std::string_view model_name) {
      if (models_.count(model_name))
         return models_.at(model_name);
      return nullptr;
   }
}
