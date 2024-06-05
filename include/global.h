#ifndef BALL_GAME_GLOBAL_H
#define BALL_GAME_GLOBAL_H

#include "raylib.h"

#define ENOUGH ((CHAR_BIT * sizeof(int) - 1) / 3 + 2)

extern const Color shadowColor;
extern const Color uiColorYellow;
extern const Color uiColorRed;
extern const Color guideColor;

Vector2 Vector2ToVirtualCoords(Vector2 coords);
Vector2 Vector2Round(Vector2 vector);
void SoundPanToWorld(Sound sound, Vector2 sourcePos, float intensity);
Vector2 Vector2ClampInsideScreen(Vector2 position, float radius);
Vector2 LengthDirToVector2(float length, float dir);

#endif //BALL_GAME_GLOBAL_H
