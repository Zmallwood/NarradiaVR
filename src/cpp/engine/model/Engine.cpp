/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Engine.h"
#include "../view/GraphicsGLView.h"
#include "AndroidVRAppManager.h"
#include "ImageBank.h"
#include "Loader.h"
#include "ModelBank.h"
#include "OpenXrProgram.h"
#include "OptionsManager.h"
#include "SceneManager.h"
#include "world.generation/model/MapGenerator.h"

namespace nar {
    /**
     * Called at game start.
     */
    void Engine::Init(android_app *app) {
        app_ = app;
        srand(time(0));
        AndroidVRAppManager::Get()->set_app(app);
        AndroidVRAppManager::Get()->Init();
        Loader::Get()->Init();
        OpenXrProgram::Get()->Init();
        OptionsManager::Get()->Init();
        GraphicsGLView::Get()->UpdateOptions();
        ImageBank::Get()->LoadImages();
        ModelBank::Get()->LoadModels();
        MapGenerator::Get()->GenerateMapArea();
    }

    /**
     * Automatic cleanup at disposal of Singleton<Engine>
     */
    Engine::~Engine() {
        AndroidVRAppManager::Get()->Cleanup();
    }

    /**
     * Forward the update task to the SceneManager.
     */
    void Engine::UpdateGameLogic() {
        if (!skip_frame_) {
            SceneManager::Get()->UpdateGameLogic();
        }
    }
}
