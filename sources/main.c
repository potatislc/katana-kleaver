#include "game.h"
#if defined(PLATFORM_WEB)
#include <emscripten.h>
#endif

int main(void)
{
    Setup();
    #if !defined(PLATFORM_WEB)
        SplashScreenInit();
        SplashScreenRun();
    #endif
    GameInit();
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(GameRunWeb, initFps, 1);
    #else
        GameRun();
    #endif
    GameDeInit();

    return 0;
}