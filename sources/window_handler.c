#include "window_handler.h"
#include "raylib.h"
#include "renderer.h"

int windowMode = -1;
bool windowIsBorderless = false;

const double defaultWindowScale = .8;
const double windowHeightScaleFactor = 1.1;

int DefaultWindowWidth(int display)
{
    return (int)(GetMonitorHeight(display) * defaultWindowScale);
}

int DefaultWindowHeight(int display)
{
    return (int)(GetMonitorHeight(display) * defaultWindowScale * windowHeightScaleFactor);
}

void WindowHandlerInit()
{
    int display = GetCurrentMonitor();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DefaultWindowWidth(display), DefaultWindowHeight(display), WINDOW_TITLE);
    Image icon = LoadImage("../assets/textures/melon/melon_big.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    WindowHandlerSetWindowMode(WM_WINDOWED);
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

void WindowHandlerSetWindowMode(WindowMode mode)
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
            SetWindowSize(DefaultWindowWidth(display), DefaultWindowHeight(display));
            SetWindowPosition((GetMonitorWidth(display) - GetScreenWidth()) / 2,
                              (GetMonitorHeight(display) - GetScreenHeight()) / 2);
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

char *GetWindowModeAsString()
{
    switch (windowMode) {
        case WM_BORDERLESS_FULL_WINDOWED:
            return "Borderless";
        case WM_FULLSCREEN:
            return "Fullscreen";
        case WM_WINDOWED:
            return "Windowed";
        default:
            return "w h a t?";
    }
}