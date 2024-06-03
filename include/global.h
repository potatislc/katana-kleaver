#ifndef BALL_GAME_GLOBAL_H
#define BALL_GAME_GLOBAL_H

#include "raylib.h"

#define DEFAULT_SCREEN_WIDTH (960) // Moved to: window_handler.h
#define DEFAULT_SCREEN_HEIGHT (1056) // Moved to: window_handler.h
#define VIRTUAL_SCREEN_WIDTH (160) // Moved to: renderer.h
#define VIRTUAL_SCREEN_HEIGHT (160) // Moved to: renderer.h
#define VIRTUAL_SCREEN_OFFSET_X (0) // Moved to: renderer.h
#define VIRTUAL_SCREEN_OFFSET_Y (16) // Moved to: renderer.h
#define MIN_BALL_RADIUS (4)
#define BALL_TOO_SMALL_FOR_CLEAN_SPLIT (8)
#define ENOUGH ((CHAR_BIT * sizeof(int) - 1) / 3 + 2)

extern Vector2 screenRatio; // Moved to: renderer.h
extern Vector2 screenOffset; // Moved to: renderer.h
extern const Color shadowColor;
extern const Color uiColorYellow;
extern const Color uiColorRed;
extern const Color guideColor;

extern bool gameOver;
extern int score;
extern int hiScore;
extern int comboScore;

void EndGame();
void AddToScore(int val);
void LoseCombo();

Vector2 Vector2ToVirtualCoords(Vector2 coords);
Vector2 Vector2Round(Vector2 vector);

#endif //BALL_GAME_GLOBAL_H
