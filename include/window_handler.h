#ifndef BALL_GAME_WINDOW_HANDLER_H
#define BALL_GAME_WINDOW_HANDLER_H

#define WINDOW_TITLE "Katana Kleaver"

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
char *GetWindowModeAsString();

extern const double defaultWindowScale;
extern const double windowHeightScaleFactor;
int DefaultWindowWidth(int display);
int DefaultWindowHeight(int display);

#endif //BALL_GAME_WINDOW_HANDLER_H
