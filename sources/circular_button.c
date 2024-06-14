#include "circular_button.h"

void CircularButtonPressOnCondition(CircularButton *button, bool condition)
{
    if (condition != button->pressed)
    {
        (condition ? button->onPressFunc : button->onReleaseFunc)();
    }

    button->pressed = condition;
}