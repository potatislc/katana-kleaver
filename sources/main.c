#include "game.h"
#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

int main(void)
{
    Setup();
    #ifndef PLATFORM_WEB
    SplashScreenInit();
    SplashScreenRun();
    #endif
    GameInit();
    #ifdef PLATFORM_WEB
    emscripten_set_main_loop(GameRun, initFps, 1);
    #else
    GameRun();
    #endif
    GameDeInit();

    return 0;
}