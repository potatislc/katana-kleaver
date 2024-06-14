#ifndef BALL_GAME_CIRCULAR_BUTTON_H
#define BALL_GAME_CIRCULAR_BUTTON_H

#include "raylib.h"

typedef struct
{
    bool highlighted;
    bool pressed;
    float radius;
    Texture2D texture;
    void (*onPressFunc)();
    void (*onReleaseFunc)();
} CircularButton;

bool IsPointInsideCircularButton(CircularButton *button, Vector2 point);
void CircularButtonPressOnCondition(CircularButton *button, bool condition);

#endif //BALL_GAME_CIRCULAR_BUTTON_H
