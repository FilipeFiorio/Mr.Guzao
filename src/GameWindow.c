/**
 * @file GameWindow.c
 * @author Prof. Dr. David Buzatto
 * @brief GameWindow implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdlib.h>
#include <stdbool.h>

#include "GameWindow.h"
#include "GameWorld.h"
#include "ResourceManager.h"
#include "raylib/raylib.h"

/**
 * @brief Creates a dinamically allocated GameWindow struct instance.
 */
GameWindow* createGameWindow( 
        int width, 
        int height, 
        const char *title, 
        int targetFPS,
        bool vSync,
        bool antialiasing, 
        bool resizable, 
        bool fullScreen,
        bool undecorated, 
        bool alwaysOnTop, 
        bool invisibleBackground, 
        bool alwaysRun, 
        bool loadResources, 
        bool initAudio ) {

    GameWindow *gameWindow = (GameWindow*) malloc( sizeof( GameWindow ) );

    gameWindow->width = width;
    gameWindow->height = height;
    gameWindow->title = title;
    gameWindow->targetFPS = targetFPS;
    gameWindow->vSync = vSync;
    gameWindow->antialiasing = antialiasing;
    gameWindow->resizable = resizable;
    gameWindow->fullScreen = fullScreen;
    gameWindow->undecorated = undecorated;
    gameWindow->alwaysOnTop = alwaysOnTop;
    gameWindow->invisibleBackground = invisibleBackground;
    gameWindow->alwaysRun = alwaysRun;
    gameWindow->loadResources = loadResources;
    gameWindow->initAudio = initAudio;
    gameWindow->gw = NULL;
    gameWindow->initialized = false;

    return gameWindow;

}

/**
 * @brief Initializes the Window, starts the game loop and, when it
 * finishes, the window will be finished and destroyed too.
 */
void initGameWindow( GameWindow *gameWindow ) {

    if ( !gameWindow->initialized ) {

        gameWindow->initialized = true;

        if ( gameWindow->antialiasing ) {
            SetConfigFlags( FLAG_MSAA_4X_HINT );
        }

        if(gameWindow->vSync) {
            SetConfigFlags( FLAG_VSYNC_HINT ); 
        }

        if ( gameWindow->resizable ) {
            SetConfigFlags( FLAG_WINDOW_RESIZABLE );
        }

        if ( gameWindow->undecorated ) {
            SetConfigFlags( FLAG_WINDOW_UNDECORATED );
        }

        if ( gameWindow->alwaysOnTop ) {
            SetConfigFlags( FLAG_WINDOW_TOPMOST );
        }

        if ( gameWindow->invisibleBackground ) {
            SetConfigFlags( FLAG_WINDOW_TRANSPARENT );
        }

        if ( gameWindow->alwaysRun ) {
            SetConfigFlags( FLAG_WINDOW_ALWAYS_RUN );
        }
        
        InitWindow( gameWindow->width, gameWindow->height, gameWindow->title );

        int mLargura = GetMonitorWidth(GetCurrentMonitor());
        int mAltura = GetMonitorHeight(GetCurrentMonitor());
        int largura = mLargura > 0 ? mLargura : gameWindow->width;
        int altura = mAltura > 0 ? mAltura : gameWindow->height;

        if (gameWindow->fullScreen && mLargura > 0 && mAltura > 0) {
            SetWindowSize( largura, altura );
            ToggleFullscreen();
        }
        
        if ( gameWindow->initAudio ) {
            InitAudioDevice();

            if ( !IsAudioDeviceReady() ) {
                gameWindow->initAudio = false;
                TraceLog(LOG_WARNING, "Audio device could not be initialized. Continuing without audio.");
            }
        }

        int refreshRate = GetMonitorRefreshRate(GetCurrentMonitor());
        int fps = refreshRate > 0 ? refreshRate : gameWindow->targetFPS;
        SetTargetFPS(fps);    

        if ( gameWindow->loadResources ) {
            loadResourcesResourceManager();
        }

        gameWindow->gw = createGameWorld();

        // game loop
        while ( !WindowShouldClose() ) {
            updateGameWorld( gameWindow->gw, GetFrameTime() );
            drawGameWorld( gameWindow->gw );
            HideCursor();
        }

        if ( gameWindow->loadResources ) {
            unloadResourcesResourceManager();
        }

        bool initAudio = gameWindow->initAudio;

        destroyGameWindow( gameWindow );

        if ( initAudio && IsAudioDeviceReady() ) {
            CloseAudioDevice();
        }

        CloseWindow();

    }

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWindow( GameWindow *gameWindow ) {
    destroyGameWorld( gameWindow->gw );
    free( gameWindow );
}