#include "window_handler.h"
#include "raylib.h"

void WindowHandlerInit()
{
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, WINDOW_TITLE);
}

void WindowHandlerToggleFullscreen()
{
    if (IsWindowFullscreen())
    {
        ToggleFullscreen();
        SetWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    }
    else
    {
        int display = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        ToggleFullscreen();
    }
}