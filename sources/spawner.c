#include "spawner.h"
#include "raylib.h"
#include "raymath.h"
#include "game.h"
#include "renderer.h"
#include "player.h"
#include "ball.h"
#include "global.h"

double spawnDelay = BALL_SPAWN_DELAY_FIRST;
double timeSinceLastSpawn;
ListNode *spawnQueueHead = NULL;

int secondBallLimit = 2;
int thirdBallLimit = 3;

void PlaceBallSpawnPoint(float radius, bool avoidPlayer, int type)
{
    timeSinceLastSpawn = GetTime();

    const Vector2 minInitPos = {radius, radius};
    const Vector2 maxInitPos = {VIRTUAL_SCREEN_WIDTH - radius, VIRTUAL_SCREEN_HEIGHT - radius};
    Ball *newBall = BallInit(
            Vector2RandomRange(minInitPos, maxInitPos),
            radius, type);

    if (avoidPlayer)
    {
        float distanceToPlayer = Vector2Distance(playerRef->position, newBall->position);
        const float collisionMargin = radius + playerRef->radius;
        // Theoretically, in an alternate universe, the game could freeze here for a while
        while (distanceToPlayer <= collisionMargin)
        {
            BallSetPosition(newBall, Vector2RandomRange(minInitPos, maxInitPos));
            distanceToPlayer = Vector2Distance(playerRef->position, newBall->position);
        }
    }

    BallSpawnPoint *newBallSpawn = BallSpawnPointInit(newBall, ballSpawnTime);
    ListNodePush(&ballSpawnPointHead, newBallSpawn);
}

void SpawnerInit()
{
    timeSinceLastSpawn = GetTime();
}

void SpawnBalls()
{
    if (ballNbrCount_All.spawned == 0)
    {
        PlaceBallSpawnPoint(RADIUS_LARGE, true, TYPE_MELON);
        // ListNodePush(&spawnQueueHead, &(SpawnData){RADIUS_LARGE, true, TYPE_MELON});
        spawnDelay = BALL_SPAWN_DELAY_LONG;
    }

    if (ballNbrCount_All.destroyed > 6)
    {
        PlaceBallSpawnPoint(RADIUS_LARGE, true, TYPE_MELON);
    }

    if (ballNbrCount_All.destroyed > 20 && spawnDelay == BALL_SPAWN_DELAY_LONG)
    {
        if (NbrOfBallsOnScreen(ballNbrCount_All) <= secondBallLimit)
        {
            PlaceBallSpawnPoint(RADIUS_LARGE, false, TYPE_MELON);

            if (NbrOfBallsOnScreen(ballNbrCount_All) == thirdBallLimit && ballNbrCount_All.destroyed > 50)
            {
                PlaceBallSpawnPoint(RADIUS_LARGE, false, TYPE_MELON);
            }
        }

        if (NbrOfBallsOnScreen(ballNbrCount_All) > 5)
        {
            PlaceBallSpawnPoint(RADIUS_MEDIUM, false, TYPE_ORANGE);
        }
    }
}