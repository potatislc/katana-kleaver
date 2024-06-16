#include "window_handler.h"
#include "raylib.h"
#include "renderer.h"

int windowMode = -1;
bool windowIsBorderless = false;

void WindowHandlerInit()
{
    int display = GetCurrentMonitor();
    InitWindow(DEFAULT_SCREEN_WIDTH(display), DEFAULT_SCREEN_HEIGHT(display), WINDOW_TITLE);
    //SetWindowState(FLAG_WINDOW_UNFOCUSED);
    //SetWindowState(FLAG_WINDOW_UNDECORATED);
    Image icon = LoadImage("../assets/textures/melon/melon_big.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
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
            SetWindowSize(DEFAULT_SCREEN_WIDTH(display), DEFAULT_SCREEN_HEIGHT(display));
            break;
    }

    RendererFitVirtualRectToScreen();
}

void WindowHandlerToggleWindowMode()
{
    windowMode++;
    if (windowMode > WINDOW_MODES_AMOUNT) windowMode = 0;

    WindowHandlerSetWindowMode(windowMode);
}