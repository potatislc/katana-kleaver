#include "global.h"
#include <math.h>

const float screenRatio = (float)SCREEN_WIDTH / (float)VIRTUAL_SCREEN_WIDTH;

Vector2 toVirtualCoords(Vector2 coords)
{
    return (Vector2){coords.x / screenRatio, coords.y / screenRatio };
}

Vector2 roundVector2(Vector2 vector)
{
    return (Vector2){ roundf(vector.x), roundf(vector.y) };
}