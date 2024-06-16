#include <malloc.h>
#include "circular_button.h"
#include "raymath.h"

void DefaultOnPress()
{
    // Do nothing
}

void DefaultOnRelease()
{
    // Do nothing
}


CircularButton *CircularButtonInit(Vector2 position, float radius, Texture2D texture, void (*onReleaseFunc)())
{
    CircularButton *circularButton = (CircularButton*)malloc(sizeof(CircularButton));
    circularButton->highlighted = false;
    circularButton->pressed = false;
    circularButton->position = position;
    circularButton->radius = radius;
    circularButton->texture = texture;
    circularButton->onPressFunc = DefaultOnPress;
    circularButton->onReleaseFunc = onReleaseFunc;

    return circularButton;
}

bool CircularButtonMousePress(CircularButton *circularButton, MouseButton mouseButton, Vector2 mousePos)
{
    if (IsPointInsideCircularButton(*circularButton, mousePos))
    {
        if (IsMouseButtonPressed(mouseButton))
        {
            CircularButtonPress(circularButton);
            return true;
        }
    }
    else
    {
        circularButton->pressed = false;
    }

    return false;
}

bool CircularButtonMouseRelease(CircularButton *circularButton, MouseButton mouseButton, Vector2 mousePos)
{
    bool condition = (IsMouseButtonReleased(mouseButton) && IsPointInsideCircularButton(*circularButton, mousePos) && circularButton->pressed);
    if (condition) CircularButtonRelease(circularButton);
    return condition;
}
void CircularButtonPress(CircularButton *circularButton)
{
    circularButton->pressed = true;
    circularButton->onPressFunc();
}

void CircularButtonRelease(CircularButton *circularButton)
{
    circularButton->pressed = false;
    circularButton->onReleaseFunc();
}

void CircularButtonDraw(CircularButton circularButton)
{
    Vector2 halfDim = {(float)circularButton.texture.width / 2, (float)circularButton.texture.height / 2};
    float scale = circularButton.radius / halfDim.x;
    Vector2 center = {halfDim.x * scale, halfDim.y * scale};
    float coolRot = sinf((float)GetTime() * 4.f) * 25.f;

    Rectangle textureRect = {0, 0, (float)circularButton.texture.width, (float)circularButton.texture.height};
    Rectangle scaledRect = {circularButton.position.x, circularButton.position.y, (float)circularButton.texture.width * scale, (float)circularButton.texture.width * scale};
    DrawTexturePro(circularButton.texture, textureRect, scaledRect, center, coolRot, WHITE);
}

bool IsPointInsideCircularButton(CircularButton circularButton, Vector2 point)
{
    return Vector2Distance(circularButton.position, point) <= circularButton.radius;
}