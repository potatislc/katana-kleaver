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
#include "spawner.h"
#include "raymath.h"

const int initFps = 60;
int targetFps = 60;
int gameState = GAME_TITLE;
unsigned int frameCounter = 0;

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

    Vector2 playerStartPos = Vector2Add(virtualScreenCenter, (Vector2){0.f, 48.f});
    playerRef = PlayerInit(playerStartPos, &ballHead);

    freezePlayer = true;
}

void SpeedUpFpsEffect()
{
    SetTargetFPS(targetFps);
    targetFps++;

    if (targetFps < 20) targetFps = 20;

    SetMusicPitch(gameAudio.mainTheme, (1.f/(float)initFps) * (float)targetFps);
}

void StartGame()
{
    gameState = GAME_PLAY;
    freezePlayer = false;
    SpawnerInit();
}

void Update()
{
    UpdateMusicStream(gameAudio.mainTheme);

    if (targetFps < initFps)
    {
        SpeedUpFpsEffect();
    }

    switch(gameState)
    {
        case GAME_TITLE:
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) StartGame();
            break;
        case GAME_PLAY:
            SetUiProgressBarMidToEnds(&spawnProgressBar, GetTime() - timeSinceLastSpawn, spawnDelay);
            SpawnerUpdate();
            break;
        case GAME_OVER:
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsBallClearingFinished() && targetFps == initFps)
            {
                BallClearerBegin(NULL, initFps);
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (RendererGetPaintPercentage() < .5f && ListLength(&ballHead) == 0)
                {
                    RendererPlayRingTransition();
                }
            }
            break;
    }

    if (!IsBallClearingFinished())
    {
        ListRemoveAllNodes(&ballSpawnPointHead);
        if (frameCounter % 6 == 0) BallClearerUpdate();
    }

    if (frameCounter % 20 == 0)
    {
        SpawnFromQueue();
    }

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
        RenderToTarget();
        RenderToScreen();
    }

    GameDeInit();
}

void GameEnd()
{
    gameState = GAME_OVER;
    // freezeBalls = true;
    ScoreHandlerLoseCombo();
    ListRemoveAllNodes(&ballSpawnPointHead);
    targetFps = 2;
}

void GameRestart()
{
    Player *newPlayer = PlayerReset(playerRef, virtualScreenCenter, &ballHead);
    playerRef = newPlayer;

    BallClearerForceFinish();

    BallDeSpawnAll();
    ListRemoveAllNodes(&ballSpawnPointHead);
    BallNbrCountReset(&ballNbrCount_All);

    ScoreHandlerResetScore();

    gameState = GAME_PLAY;

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