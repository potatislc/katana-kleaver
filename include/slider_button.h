#ifndef BALL_GAME_SLIDER_BUTTON_H
#define BALL_GAME_SLIDER_BUTTON_H

#include "raylib.h"

typedef struct
{
    Rectangle rectangle;
    float percent;
} SliderButton;

float SliderButtonInput(SliderButton *sliderButton, bool inputCondition, Vector2 point);
void SliderButtonDraw(SliderButton sliderButton);

#endif //BALL_GAME_SLIDER_BUTTON_H
