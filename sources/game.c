#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "raylib.h"
#include "player.h"
#include "window_handler.h"
#include "asset_loader.h"
#include "renderer.h"
#include "ball.h"
#include "score_handler.h"
#include "particle.h"
#include "global.h"
#include "raymath.h"

// Move these to spawner.c
double spawnDelay = BALL_SPAWN_DELAY_FIRST;
double timeSinceLastSpawn;

bool gameOver = false;

void GameInit()
{
    WindowHandlerInit();

    RendererInit();

    LoadGameTextures();
    InitAudioDevice();
    LoadGameAudio();

    // Set Seed
    srand(time(0));

    SetTargetFPS(60);

    WindowHandlerSetWindowMode(WM_BORDERLESS_FULL_WINDOWED);
    RendererFitVirtualRectToScreen();

    playerRef = PlayerInit(virtualScreenCenter, &ballHead);
    timeSinceLastSpawn = GetTime();
}

void PlaceBallSpawnPoint(float radius, bool avoidPlayer)
{
    timeSinceLastSpawn = GetTime();

    const Vector2 minInitPos = {radius, radius};
    const Vector2 maxInitPos = {VIRTUAL_SCREEN_WIDTH - radius, VIRTUAL_SCREEN_HEIGHT - radius};
    Ball *newBall = BallInit(
            Vector2RandomRange(minInitPos, maxInitPos),
            radius);

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

void Update()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) && gameOver)
    {
        GameRestart();
        ScoreHandlerResetScore();
    }

    if (GetTime() > timeSinceLastSpawn+spawnDelay && !gameOver)
    {
        if (ballNbrCount_All.spawned == 0)
        {
            PlaceBallSpawnPoint(RADIUS_LARGE, true);
            spawnDelay = BALL_SPAWN_DELAY_LONG;
        }

        if (ballNbrCount_All.destroyed > 6)
        {
            PlaceBallSpawnPoint(RADIUS_LARGE, true);
        }

        if (ballNbrCount_All.destroyed > 20 && NbrOfBallsOnScreen(ballNbrCount_All) <= 2 && spawnDelay == BALL_SPAWN_DELAY_LONG)
        {
            PlaceBallSpawnPoint(RADIUS_LARGE, false);
        }

        if (NbrOfBallsOnScreen(ballNbrCount_Small) > 6)
        {
            // Spawn Orange
        }
    }

    if (!gameOver) SetUiProgressBarMidToEnds(&spawnProgressBar, GetTime() - timeSinceLastSpawn, spawnDelay);

    if (!freezeBalls)
    {
        BallsUpdate();
    }

    PlayerUpdate(playerRef);

    ParticlesUpdate();

    // Toggle Fullscreen
    if ((IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) || IsKeyPressed(KEY_F11))
    {
        WindowHandlerToggleWindowMode();
        RendererFitVirtualRectToScreen();
    }
}

void GameRun()
{
    while (!WindowShouldClose())
    {
        Update();
        RenderToTarget(gameOver);
        RenderToScreen();
    }

    GameDeInit();
}

void GameEnd()
{
    gameOver = true;
    freezeBalls = true;
    ScoreHandlerLoseCombo();
    ListRemoveAllNodes(&ballSpawnPointHead);
}

void GameRestart()
{
    Player *newPlayer = PlayerReset(playerRef, virtualScreenCenter, &ballHead);
    playerRef = newPlayer;

    BallDeSpawnAll();
    ListRemoveAllNodes(&ballSpawnPointHead);
    BallNbrCountReset(&ballNbrCount_All);

    ScoreHandlerResetScore();

    gameOver = false;

    freezeBalls = false;

    timeSinceLastSpawn = GetTime();
    spawnDelay = BALL_SPAWN_DELAY_SHORT;

    RendererClearBackgroundPaint();
}

void GameDeInit()
{
    CloseWindow();
}