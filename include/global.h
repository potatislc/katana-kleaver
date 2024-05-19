#ifndef BALL_GAME_GLOBAL_H
#define BALL_GAME_GLOBAL_H

#include "raylib.h"

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (1280)
#define VIRTUAL_SCREEN_WIDTH (160)
#define VIRTUAL_SCREEN_HEIGHT (160)
#define MIN_BALL_RADIUS (4)

extern const float screenRatio;
extern const Color shadowColor;
extern bool freezeBalls;

extern int score;
extern int hiScore;
extern int comboScore;

void AddToScore(int val);

Vector2 Vector2ToVirtualCoords(Vector2 coords);
Vector2 Vector2Round(Vector2 vector);

#endif //BALL_GAME_GLOBAL_H
