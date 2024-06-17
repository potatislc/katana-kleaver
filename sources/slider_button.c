#include "slider_button.h"
#include "raymath.h"

float SliderButtonInput(SliderButton *sliderButton, bool inputCondition, Vector2 point)
{
    if (!inputCondition || !CheckCollisionPointRec(point, sliderButton->rectangle))
    {
        return sliderButton->percent;
    }

    sliderButton->percent = (Clamp((point.x - sliderButton->rectangle.x) / sliderButton->rectangle.width, 0.f, 1.f));
    return sliderButton->percent;
}

void SliderButtonDraw(SliderButton sliderButton)
{
    DrawRectangleRec(sliderButton.rectangle, LIGHTGRAY);
    float circleRadius = sliderButton.rectangle.height / 2.f;
    Vector2 circlePos = {sliderButton.percent * sliderButton.rectangle.width, sliderButton.rectangle.y + circleRadius};
    DrawCircleV(circlePos, circleRadius, WHITE);
}