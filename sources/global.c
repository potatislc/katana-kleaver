#include "global.h"
#include <math.h>
#include "renderer.h"

const Color shadowColor = {121, 65, 0, 255};
const Color uiColorYellow = {255,243, 146, 255};
const Color uiColorRed = { 219, 65, 97, 255};
const Color guideColor = {81, 130, 255, 200};

Vector2 Vector2ToVirtualCoords(Vector2 coords)
{
    return (Vector2){
        (coords.x / screenRatio.x) - screenOffset.x / screenRatio.x,
        (coords.y / screenRatio.y) - screenOffset.y / screenRatio.y };
}

Vector2 Vector2Round(Vector2 vector)
{
    return (Vector2){ roundf(vector.x), roundf(vector.y) };
}

void SoundPanToWorld(Sound sound, Vector2 sourcePos, float intensity)
{
    SetSoundPan(sound, .5f + (.5f - (sourcePos.x / VIRTUAL_SCREEN_WIDTH)) * intensity);
}