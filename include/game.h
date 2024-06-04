#ifndef BALL_GAME_GAME_H
#define BALL_GAME_GAME_H

#include "player.h"

#define BALL_SPAWN_DELAY_LONG (3.0)
#define BALL_SPAWN_DELAY_SHORT (1.0)

void GameInit();
void GameRun();
void GameRestart();
void GameDeInit();

#endif //BALL_GAME_GAME_H
