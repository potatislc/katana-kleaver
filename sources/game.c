#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "game.h"
#include "raylib.h"
#include "window_handler.h"
#include "asset_loader.h"
#include "renderer.h"
#include "ball.h"
#include "score_handler.h"

// Move these to spawner.c
double spawnDelay = BALL_SPAWN_DELAY_LONG;
double timeSinceLastSpawn;
int ballsSpawned = 0;

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
    spawnDelay = BALL_SPAWN_DELAY_LONG;

    float testRadius = 32.0f;
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
        PlaceBallSpawnPoint();
        spawnDelay -= .15;
        spawnDelay += ballCount * ballCount * .01;
        spawnDelay = fmax(.5, spawnDelay);
    }

    if (!freezeBalls)
    {
        BallsUpdate();
    }

    PlayerUpdate(playerRef);

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
    ListNodeRemoveAll(&ballSpawnPointHead);
}

void GameRestart()
{
    Player *newPlayer = PlayerReset(playerRef, virtualScreenCenter, &ballHead);
    playerRef = newPlayer;

    BallDeSpawnAll();
    ListNodeRemoveAll(&ballSpawnPointHead);

    ScoreHandlerResetScore();

    gameOver = false;

    freezeBalls = false;

    timeSinceLastSpawn = GetTime();
    spawnDelay = BALL_SPAWN_DELAY_SHORT;
}

void GameDeInit()
{
    CloseWindow();
}