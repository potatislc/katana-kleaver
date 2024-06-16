#ifndef BALL_GAME_CIRCULAR_BUTTON_H
#define BALL_GAME_CIRCULAR_BUTTON_H

#include "raylib.h"

typedef struct
{
    bool highlighted;
    bool pressed;
    Vector2 position;
    float radius;
    Texture2D texture;
    void (*onPressFunc)();
    void (*onReleaseFunc)();
} CircularButton;

CircularButton *CircularButtonInit(Vector2 position, float radius, Texture2D texture, void (*onReleaseFunc)());
bool IsPointInsideCircularButton(CircularButton button, Vector2 point);
bool CircularButtonMousePress(CircularButton *circularButton, MouseButton mouseButton, Vector2 mousePos);
bool CircularButtonMouseRelease(CircularButton *circularButton, MouseButton mouseButton, Vector2 mousePos);
void CircularButtonPress(CircularButton *circularButton);
void CircularButtonRelease(CircularButton *circularButton);
void CircularButtonDraw(CircularButton circularButton);

#endif //BALL_GAME_CIRCULAR_BUTTON_H
