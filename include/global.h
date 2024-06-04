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

#endif //BALL_GAME_GLOBAL_H
