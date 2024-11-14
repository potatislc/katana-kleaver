#include "window_handler.h"
#include "raylib.h"
#include "renderer.h"
#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

int windowMode = -1;
bool windowIsBorderless = false;

#ifdef PLATFORM_WEB
const double defaultWindowScale = .6;
#else
const double defaultWindowScale = .8;
#endif
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
    SetConfigFlags(FLAG_MSAA_4X_HINT);
#ifdef PLATFORM_WEB
    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);
    attr.antialias = EM_FALSE;  // Disable anti-aliasing
    attr.alpha = EM_TRUE;       // Example: Enable alpha channel

    // Create WebGL context with the specified attributes
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attr);

    // Make the context current
    emscripten_webgl_make_context_current(context);
#endif
    InitWindow(DefaultWindowWidth(display), DefaultWindowHeight(display), WINDOW_TITLE);
    Image icon = LoadImage(ASSETS_PATH"textures/melon/melon_big.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
#ifdef PLATFORM_DESKTOP
    WindowHandlerSetWindowMode(WM_WINDOWED);
    MaximizeWindow();
#endif
    // For trailer
    // DisableCursor();
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

#ifdef PLATFORM_DESKTOP
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
#endif
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