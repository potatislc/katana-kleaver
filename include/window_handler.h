#ifndef BALL_GAME_WINDOW_HANDLER_H
#define BALL_GAME_WINDOW_HANDLER_H

#define WINDOW_TITLE "Ball Game"

#define DEFAULT_SCREEN_WIDTH(display) ((GetMonitorHeight(display) <= 1408) ? 800 : 960)
#define DEFAULT_SCREEN_HEIGHT(display) ((GetMonitorHeight(display) <= 1408) ? 880 : 1056)
#define WINDOW_MODES_AMOUNT (2)

typedef enum WindowMode
{
    WM_BORDERLESS_FULL_WINDOWED,
    WM_FULLSCREEN,
    WM_WINDOWED
} WindowMode;

void WindowHandlerInit();
void WindowHandlerSetWindowMode(WindowMode mode);
void WindowHandlerToggleWindowMode();
WindowMode WindowHandlerGetWindowMode();
char *GetWindowModeAsText();

#endif //BALL_GAME_WINDOW_HANDLER_H
