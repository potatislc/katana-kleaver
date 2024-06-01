#include "global.h"
#include <math.h>

const Vector2 screenRatio = {
        (float)DEFAULT_SCREEN_WIDTH / (float)VIRTUAL_SCREEN_WIDTH,
        (float)DEFAULT_SCREEN_HEIGHT / (float)VIRTUAL_SCREEN_HEIGHT};
const Color shadowColor = {121, 65, 0, 255};
const Color uiColorYellow = {255,243, 146, 255};
const Color uiColorRed = { 219, 65, 97, 255};
const Color guideColor = {255, 255, 255, 100};

int score = 0;
int hiScore = 0;
int comboScore = 0;

void AddToScore(int val)
{
    score += val;
    comboScore += val;
    hiScore = (int)fmax(score, hiScore);
}

Vector2 Vector2ToVirtualCoords(Vector2 coords)
{
    return (Vector2){coords.x / screenRatio.x, coords.y / screenRatio.x };
}

Vector2 Vector2Round(Vector2 vector)
{
    return (Vector2){ roundf(vector.x), roundf(vector.y) };
}