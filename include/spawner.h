#ifndef BALL_GAME_SPAWNER_H
#define BALL_GAME_SPAWNER_H

#include <stdbool.h>
#include "linked_list.h"

#define BALL_SPAWN_DELAY_FIRST (4.0)
#define BALL_SPAWN_DELAY_LONG (3.0)
#define BALL_SPAWN_DELAY_SHORT (1.0)

typedef struct
{
    float radius;
    bool avoidPlayer;
    int type;
} SpawnData;

extern double spawnDelay;
extern double timeSinceLastSpawn;
extern ListNode *spawnQueueHead;

void SpawnerInit();
void SpawnerUpdate();
void SpawnFromQueue();
void SpawnerPlaceBallSpawnPoint(float radius, bool avoidPlayer, int type);

#endif //BALL_GAME_SPAWNER_H
