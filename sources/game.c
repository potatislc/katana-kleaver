#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "raylib.h"
#include "window_handler.h"
#include "asset_loader.h"
#include "renderer.h"

void GameInit()
{
    WindowHandlerInit();

    RendererInit();

    LoadGameTextures();
    InitAudioDevice();
    LoadGameAudio();

    // Set Seed
    srand(time(0));

    //SetTargetFPS(60);

    WindowHandlerToggleFullscreen();
    RendererFitVirtualRectToScreen();
}

void GameRun()
{
    while (!WindowShouldClose())
    {
        GameUpdate();
        RenderToTarget();
        RenderToScreen();
    }

    GameDeInit();
}

void GameUpdate()
{
    // Toggle Fullscreen
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
    {
        WindowHandlerToggleFullscreen();
        RendererFitVirtualRectToScreen();
    }
}

void GameDeInit()
{
    CloseWindow();
}