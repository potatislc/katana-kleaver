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
    WindowHandlerToggleFullscreen();

    RendererInit();

    LoadGameTextures();
    InitAudioDevice();
    LoadGameAudio();

    // Set Seed
    srand(time(0));

    GameRun();
}

void GameRun()
{
    while (!WindowShouldClose())
    {
        GameUpdate();
    }

    GameDeInit();
}

void GameUpdate()
{

}

void GameDeInit()
{

}