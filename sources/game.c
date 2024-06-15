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
#include "global.h"

const int initFps = 60;
int targetFps = 60;
int gameState = GAME_TITLE;
unsigned int frameCounter = 0;

CircularButton *startButton;
CircularButton *settingsButton;
CircularButton *backButton; // Back changes depending on state

void StartGame()
{
    gameState = GAME_PLAY;
    playerRef->stateExecute = STATE_EXEC_PLAYER_MOVE;
    freezePlayer = false;
    SpawnerInit();
}

void OpenSettings()
{
    gameState = GAME_SETTINGS;
}

void GoBack()
{
    gameState = GAME_TITLE;
}

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

    Vector2 startBtnPos = {24, VIRTUAL_SCREEN_HEIGHT - 24};
    startButton = CircularButtonInit(startBtnPos, 16, gameTextures.melonBig, StartGame);

    Vector2 settingBtnPos = {VIRTUAL_SCREEN_WIDTH - 24, startBtnPos.y};
    settingsButton = CircularButtonInit(settingBtnPos, 16, gameTextures.settingsIcon, OpenSettings);

    Vector2 backBtnPos = {24, VIRTUAL_SCREEN_HEIGHT - 24};
    backButton = CircularButtonInit(backBtnPos, 16, gameTextures.arrowIcon, GoBack);
}

void SpeedUpFpsEffect()
{
    targetFps++;
    SetTargetFPS(targetFps);

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

    Vector2 mousePos = Vector2ToVirtualCoords(GetMousePosition());

    switch(gameState)
    {
        case GAME_TITLE:
        {
            bool startCondition = IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsPointInsideCircularButton(*startButton, mousePos);
            CircularButtonDownOnCondition(startButton, startCondition);

            bool settingsCondition = IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsPointInsideCircularButton(*settingsButton, mousePos);
            CircularButtonDownOnCondition(settingsButton, settingsCondition);
            break;
        }

        case GAME_PLAY:
        {
            SetUiProgressBarMidToEnds(&spawnProgressBar, GetTime() - timeSinceLastSpawn, spawnDelay);
            SpawnerUpdate();
            break;
        }

        case GAME_OVER:
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsBallClearingFinished() && targetFps == initFps) {
                BallClearerBegin(NULL, initFps);
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (RendererGetPaintPercentage() < .5f && ListLength(&ballHead) == 0) {
                    if (!RendererIsRingTransitionActive()) RendererPlayRingTransition();
                }
            }
            break;
        }

        case GAME_SETTINGS:
        {
            bool backCondition = IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsPointInsideCircularButton(*backButton, mousePos);
            CircularButtonDownOnCondition(backButton, backCondition);
            break;
        }
    }

    if (!IsBallClearingFinished())
    {
        ListRemoveAllNodes(&ballSpawnPointHead);
        if (IsSoundPlaying(gameAudio.ballSpawn)) StopSound(gameAudio.ballSpawn);
        if (IsSoundPlaying(gameAudio.orangeSpawn)) StopSound(gameAudio.orangeSpawn);
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

    ScoreHandlerAddToScoreFromBonusPool();

    // Toggle Fullscreen
    if ((IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) || IsKeyPressed(KEY_F11))
    {
        WindowHandlerToggleWindowMode();
        RendererFitVirtualRectToScreen();
    }

    // Toggle Debug Draw
    if (IsKeyPressed(KEY_D) && IsKeyDown(KEY_LEFT_SHIFT))
    {
        debugDrawing = !debugDrawing;
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
    ListRemoveAllNodes(&spawnQueueHead);
    targetFps = 2;
}

void GameRestart()
{
    Player *newPlayer = PlayerReset(playerRef, virtualScreenCenter, &ballHead);
    playerRef = newPlayer;

    playerRef->stateExecute = STATE_EXEC_PLAYER_MOVE;

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