#include "global.h"
#include <math.h>

const float screenRatio = (float)SCREEN_WIDTH / (float)VIRTUAL_SCREEN_WIDTH;
const Color shadowColor = {121, 65, 0, 255};
bool freezeBalls = false;

Vector2 to_virtual_coords_vector2(Vector2 coords)
{
    return (Vector2){coords.x / screenRatio, coords.y / screenRatio };
}

Vector2 round_vector2(Vector2 vector)
{
    return (Vector2){ roundf(vector.x), roundf(vector.y) };
}