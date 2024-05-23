#ifndef BALL_GAME_GLOBAL_H
#define BALL_GAME_GLOBAL_H

#include "raylib.h"

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (1408)
#define VIRTUAL_SCREEN_WIDTH (160)
#define VIRTUAL_SCREEN_HEIGHT (160)
#define VIRTUAL_SCREEN_OFFSET_X (0)
#define VIRTUAL_SCREEN_OFFSET_Y (16)
#define MIN_BALL_RADIUS (4)
#define BALL_TOO_SMALL_FOR_CLEAN_SPLIT (8)
#define ENOUGH ((CHAR_BIT * sizeof(int) - 1) / 3 + 2)

extern const float screenRatio;
extern const Color shadowColor;
extern const Color uiColorYellow;
extern const Color uiColorRed;

extern int score;
extern int hiScore;
extern int comboScore;

void AddToScore(int val);

Vector2 Vector2ToVirtualCoords(Vector2 coords);
Vector2 Vector2Round(Vector2 vector);

#endif //BALL_GAME_GLOBAL_H
