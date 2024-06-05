#ifndef BALL_GAME_GAME_H
#define BALL_GAME_GAME_H

#include <stdbool.h>

#define BALL_SPAWN_DELAY_LONG (3.0)
#define BALL_SPAWN_DELAY_SHORT (1.0)

void GameInit();
void GameRun();
void GameEnd();
void GameRestart();
void GameDeInit();

extern bool gameOver;

#endif //BALL_GAME_GAME_H
