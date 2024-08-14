#include "game.h"
#include "renderer.h"
#include "sprite.h"
#include "asset_loader.h"
#include "firefly.h"

#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

SplashMessage splashMessageIndex = 0;

double messageDurations[SM_LENGTH] = {6, 9};

Sprite *splashMouseIcon;

void SplashScreenInit()
{
    splashMouseIcon = SpriteInit(gameTextures.mouseIcon, (Rectangle){0, 0, 64, 64}, 0, true);
    FirefliesInit();
}

void SplashScreenRun()
{
    while (splashMessageIndex < SM_LENGTH)
    {
        while (GetTime() <= messageDurations[splashMessageIndex])
        {
            RenderSplashScreenToTarget();
            RenderToScreen();

            if (WindowShouldClose() || IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) break;
        }

        splashMessageIndex++;
    }

    SplashScreenEnd();
}

void SplashScreenEnd()
{
#ifdef PLATFORM_WEB
    emscripten_cancel_main_loop();
    GameInit();
    emscripten_set_main_loop(GameRun, initFps, 1);
#endif
}