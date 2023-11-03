/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Engine.h"
#include "../../engine.assets/model/ImageBank.h"
#include "../../engine.assets/model/ModelBank.h"
#include "../view/GraphicsGLView.h"
#include "AppManager.h"
#include "Loader.h"
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
        srand(time(0)); // Make sure randomization is unique each game start.
        AppManager::Get()->set_app(app); // Store app for later use.
        AppManager::Get()->Init();       // Init by setting properties of app object.
        Loader::Get()->Init();                    // Init loader.
        OpenXrProgram::Get()->Init();             // Init XR program.
        OptionsManager::Get()->Init();            // Init options.
        GraphicsGLView::Get()->UpdateOptions();   // Update uptions for GL graphics.
        ImageBank::Get()->LoadImages();           // Init image assets.
        ModelBank::Get()->LoadModels();           // Init model assets.
        MapGenerator::Get()->GenerateMapArea();   // Create randomly generated game map.
    }

    /**
     * Automatic cleanup at disposal of Singleton<Engine>
     */
    Engine::~Engine() {
        AppManager::Get()->Cleanup();
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
