#include "game.h"

int main(void)
{
    Setup();
    SplashScreenInit();
    SplashScreenRun();
    GameInit();
    GameRun();

    return 0;
}