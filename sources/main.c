#include "game.h"
#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

int main(void)
{
    Setup();
    SplashScreenInit();
#ifdef PLATFORM_WEB
    emscripten_set_main_loop(SplashScreenRun, 0, 1); // Handles game init and running as well
#else
    SplashScreenRun();
#endif
    GameInit();
    GameRun();
    GameDeInit();

    return 0;
}