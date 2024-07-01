#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "raylib.h"
#include "renderer.h"
#include "window_handler.h"
#include "asset_loader.h"

void Setup()
{
    WindowHandlerInit();

    RendererInit();

    LoadGameTextures();
    InitAudioDevice();
    LoadGameAudio();

    // Set Seed
    srand(time(0));

    SetTargetFPS(initFps);
}