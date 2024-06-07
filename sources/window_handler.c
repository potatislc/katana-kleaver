#include "window_handler.h"
#include "raylib.h"

int windowMode = -1;
bool windowIsBorderless = false;

void WindowHandlerInit()
{
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, WINDOW_TITLE);
}

void ToggleBorderLessWindowedOverride()
{
    ToggleBorderlessWindowed();
    windowIsBorderless = !windowIsBorderless;
}

WindowMode WindowHandlerGetWindowMode()
{
    return windowMode;
}

void WindowHandlerSetWindowMode(int mode)
{
    windowMode = mode;

    int display = GetCurrentMonitor();

    switch (windowMode)
    {
        case WM_BORDERLESS_FULL_WINDOWED:
            if (IsWindowFullscreen()) ToggleFullscreen();
            if (!windowIsBorderless) ToggleBorderLessWindowedOverride();
            SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
            break;
        case WM_FULLSCREEN:
            SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
            if (!IsWindowFullscreen()) ToggleFullscreen();
            break;
        case WM_WINDOWED:
            if (IsWindowFullscreen()) ToggleFullscreen();
            if (windowIsBorderless) ToggleBorderLessWindowedOverride();
            SetWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
            break;
    }
}

void WindowHandlerToggleWindowMode()
{
    windowMode++;
    if (windowMode > WINDOW_MODES_AMOUNT) windowMode = 0;

    WindowHandlerSetWindowMode(windowMode);
}