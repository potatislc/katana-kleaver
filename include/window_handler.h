#ifndef BALL_GAME_WINDOW_HANDLER_H
#define BALL_GAME_WINDOW_HANDLER_H

#define WINDOW_TITLE "Ball Game"

#define DEFAULT_SCREEN_WIDTH(display) ((GetMonitorHeight(display) <= 1408) ? 800 : 1280)
#define DEFAULT_SCREEN_HEIGHT(display) ((GetMonitorHeight(display) <= 1408) ? 880 : 1408)
#define WINDOW_MODES_AMOUNT (2)

typedef enum WindowMode
{
    WM_BORDERLESS_FULL_WINDOWED,
    WM_FULLSCREEN,
    WM_WINDOWED
} WindowMode;

void WindowHandlerInit();
void WindowHandlerSetWindowMode(int mode);
void WindowHandlerToggleWindowMode();
WindowMode WindowHandlerGetWindowMode();

#endif //BALL_GAME_WINDOW_HANDLER_H
