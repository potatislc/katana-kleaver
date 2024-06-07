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

void PlaceBallSpawnPoint()
{
    timeSinceLastSpawn = GetTime();

    float testRadius = RADIUS_LARGE;
    Ball *newBall = BallInit(
            (Vector2) {testRadius, testRadius},
            (Vector2) {VIRTUAL_SCREEN_WIDTH - testRadius, VIRTUAL_SCREEN_HEIGHT - testRadius},
            testRadius);

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
            PlaceBallSpawnPoint();
            spawnDelay = BALL_SPAWN_DELAY_LONG;
        }

        if (ballNbrCount_All.destroyed > 6)
        {
            PlaceBallSpawnPoint();
        }

        if (ballNbrCount_All.destroyed > 13 && NbrOfBallsOnScreen(ballNbrCount_All) <= 2 && spawnDelay == BALL_SPAWN_DELAY_LONG)
        {
            PlaceBallSpawnPoint();
        }

        if (NbrOfBallsOnScreen(ballNbrCount_Small) > 6)
        {
            // Spawn Orange
        }
    }

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