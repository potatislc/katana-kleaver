#include "game.h"
#include "renderer.h"

SplashMessage splashMessageIndex = 0;

double messageDurations[SM_LENGTH] = {2, 3};

void SplashScreenInit()
{

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