#include "game.h"
#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

int main(void)
{
    Setup();
    SplashScreenInit();
#ifdef PLATFORM_WEB
    emscripten_set_main_loop(SplashScreenRun, initFps, 1); // Handles game init and running as well
#else
    SplashScreenRun();
#endif
    GameInit();
    GameRun();
    GameDeInit();

    return 0;
}