#ifndef BALL_GAME_GLOBAL_H
#define BALL_GAME_GLOBAL_H

#include "raylib.h"

#define ENOUGH ((CHAR_BIT * sizeof(int) - 1) / 3 + 2)

extern const Color shadowColor;
extern const Color uiColorYellow;
extern const Color uiColorRed;
extern const Color guideColor;

enum StandardRadius
{
    RADIUS_SMALL = 8,
    RADIUS_MEDIUM = 16,
    RADIUS_LARGE = 32
};

Vector2 Vector2ToVirtualCoords(Vector2 coords);
Vector2 Vector2Round(Vector2 vector);
Vector2 Vector2ClampInsideScreen(Vector2 position, float radius);
Vector2 LengthDirToVector2(float length, float dir);
Vector2 Vector2RandomRange(Vector2 minPos, Vector2 maxPos);

#define DEFAULT_SOUND_PAN_INTENSITY (.5f)

void SoundPanToWorld(Sound sound, Vector2 sourcePos, float intensity);
Sound SoundPickRandom(Sound *sounds, int length);

#endif //BALL_GAME_GLOBAL_H
