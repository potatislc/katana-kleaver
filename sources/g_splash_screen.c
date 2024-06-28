#include "game.h"
#include "renderer.h"
#include "sprite.h"
#include "asset_loader.h"
#include "firefly.h"

SplashMessage splashMessageIndex = 0;

double messageDurations[SM_LENGTH] = {4.5, 8};

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
        }

        splashMessageIndex++;
    }

    SplashScreenEnd();
}

void SplashScreenEnd()
{

}