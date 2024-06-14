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
void CircularButtonDownOnCondition(CircularButton *button, bool condition);
void CircularButtonDraw(CircularButton circularButton);

#endif //BALL_GAME_CIRCULAR_BUTTON_H
