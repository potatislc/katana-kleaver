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

const int initFps = 60;
int targetFps = 60;
bool gameOver = false;
unsigned int frameCounter = 0;

int secondBallLimit = 2;
int thirdBallLimit = 3;

void GameInit()
{
    WindowHandlerInit();

    RendererInit();

    LoadGameTextures();
    InitAudioDevice();
    LoadGameAudio();

    PlayMusicStream(gameAudio.mainTheme);

    // Set Seed
    srand(time(0));

    SetTargetFPS(initFps);

    WindowHandlerSetWindowMode(WM_BORDERLESS_FULL_WINDOWED);
    RendererFitVirtualRectToScreen();

    playerRef = PlayerInit(virtualScreenCenter, &ballHead);
    timeSinceLastSpawn = GetTime();
}

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

void SpeedUpFpsEffect()
{
    SetTargetFPS(targetFps);
    targetFps++;

    if (targetFps < 20) targetFps = 20;

    SetMusicPitch(gameAudio.mainTheme, (1.f/(float)initFps) * (float)targetFps);
}

void Update()
{
    UpdateMusicStream(gameAudio.mainTheme);

    if (targetFps < initFps)
    {
        SpeedUpFpsEffect();
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) && gameOver && targetFps == initFps)
    {
        GameRestart();
        ScoreHandlerResetScore();
    }

    if (!IsBallClearingFinished())
    {
        ListRemoveAllNodes(&ballSpawnPointHead);
        if (frameCounter % 6 == 0) BallClearerUpdate();
    }

    if (GetTime() > timeSinceLastSpawn+spawnDelay && !gameOver)
    {
        if (ballNbrCount_All.spawned == 0)
        {
            PlaceBallSpawnPoint(RADIUS_LARGE, true, TYPE_MELON);
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

    if (!gameOver) SetUiProgressBarMidToEnds(&spawnProgressBar, GetTime() - timeSinceLastSpawn, spawnDelay);

    if (!freezeBalls)
    {
        BallsUpdate();
    }

    if (!freezePlayer)
    {
        PlayerUpdate(playerRef);
    }

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
        frameCounter++;
        Update();
        RenderToTarget(gameOver);
        RenderToScreen();
    }

    GameDeInit();
}

void GameEnd()
{
    gameOver = true;
    // freezeBalls = true;
    ScoreHandlerLoseCombo();
    ListRemoveAllNodes(&ballSpawnPointHead);
    targetFps = 2;
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

void GameFreezeAllEntities(bool enable)
{
    freezeBalls = enable;
    freezePlayer = enable;
}