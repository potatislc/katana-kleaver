#include "global.h"

const float screenRatio = (float)SCREEN_WIDTH / (float)VIRTUAL_SCREEN_WIDTH;

Vector2 ToVirtualCoords(Vector2 coords)
{
    return (Vector2){coords.x / screenRatio, coords.y / screenRatio };
}