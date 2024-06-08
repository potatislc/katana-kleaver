#include "global.h"
#include <math.h>
#include <stdlib.h>
#include "renderer.h"
#include "raymath.h"

const Color shadowColor = {121, 65, 0, 255};
const Color uiColorYellow = {255,243, 146, 255};
const Color uiColorRed = { 219, 65, 97, 255};
const Color uiColorDarkGray = {50, 50, 50, 255};
const Color guideColor = {81, 130, 255, 200};
const Color orangeColor = {255, 180, 0, 255};

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

Vector2 Vector2ClampInsideScreen(Vector2 position, float radius)
{
    return (Vector2){
            Clamp(position.x, radius, VIRTUAL_SCREEN_WIDTH - radius),
            Clamp(position.y, radius, VIRTUAL_SCREEN_HEIGHT - radius)};
}

Vector2 LengthDirToVector2(float length, float dir)
{
    return (Vector2){cosf(dir) * length, sinf(dir) * length};
}

Vector2 Vector2RandomRange(Vector2 minPos, Vector2 maxPos)
{
    if (minPos.x == maxPos.x && minPos.y == maxPos.y) return minPos;

    Vector2 spawnAreaSize = Vector2Subtract(maxPos, minPos);
    return Vector2Round((Vector2){minPos.x + ((float)rand()/(float)(RAND_MAX)) * spawnAreaSize.x,
                                  minPos.y + ((float)rand()/(float)(RAND_MAX)) * spawnAreaSize.y});
}

Sound SoundPickRandom(Sound *sounds, int length)
{
    return sounds[rand() % length];
}