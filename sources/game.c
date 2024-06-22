#include <stdlib.h>
#include <stdio.h>
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
#include "storage.h"
#include "tutorial.h"

const int initFps = 60;
int targetFps = 60;
int gameState = GAME_TITLE;
unsigned int frameCounter = 0;

CircularButton *startButton;
CircularButton *settingsButton;
CircularButton *backButton;
CircularButton *windowModeButton;
CircularButton *muteBgmButton;
CircularButton *muteSfxButton;

/*
SliderButton *bgmVolSlider;
SliderButton *sfxVolSlider;
SliderButton *screenShakeSlider;
*/

bool IsFloorCleaned()
{
    return RendererGetPaintPercentage() < .5f && ListLength(&ballHead) == 0;
}

void GameStart()
{
    if (!IsFloorCleaned() && gameState != GAME_TUTORIAL)
    {
        gameState = GAME_OVER;
        return;
    }

    freezePlayer = false;

    playerRef->stateExecute = STATE_EXEC_PLAYER_MOVE;

    BallClearerForceFinish();

    BallDeSpawnAll();
    ListRemoveAllNodes(&ballSpawnPointHead);
    BallNbrCountReset(&ballNbrCount_All);

    ScoreHandlerResetScore();

    freezeBalls = false;

    RendererClearBackgroundPaint();

    if (tutorialStateIndex < TUTORIAL_LENGTH)
    {
        TutorialBegin();
        return;
    }

    gameState = GAME_PLAY;
    SpawnerInit();
    spawnDelay = BALL_SPAWN_DELAY_FIRST;
}

void OpenSettings()
{
    gameState = GAME_SETTINGS;
}

void GoBackToTitle()
{
    gameState = GAME_TITLE;
    if (playerRef != NULL && playerRef->stateExecute != STATE_EXEC_PLAYER_DEAD) playerRef->stateExecute = STATE_EXEC_PLAYER_IDLE;
}

void ToggleWindowMode()
{
    WindowHandlerToggleWindowMode();
    RendererFitVirtualRectToScreen();
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

    ScoreHandlerSetHiScore(LoadStorageValue(STORAGE_POSITION_HISCORE));
    tutorialStateIndex = LoadStorageValue(STORAGE_POSITION_TUTORIAL_STATE_INDEX);

    Vector2 startBtnPos = {24, VIRTUAL_SCREEN_HEIGHT - 24};
    startButton = CircularButtonInit(startBtnPos, 16, gameTextures.playIcon, GameStart);

    Vector2 settingBtnPos = {VIRTUAL_SCREEN_WIDTH - 24, startBtnPos.y};
    settingsButton = CircularButtonInit(settingBtnPos, 16, gameTextures.settingsIcon, OpenSettings);

    Vector2 backBtnPos = {24, VIRTUAL_SCREEN_HEIGHT - 24};
    backButton = CircularButtonInit(backBtnPos, 16, gameTextures.arrowIcon, GoBackToTitle);

    Vector2 windowModeBtnPos = {24, 16 + 24};
    windowModeButton = CircularButtonInit(windowModeBtnPos, 16, gameTextures.windowIcon, ToggleWindowMode);

    Vector2 muteBgmBtnPos = {24, 24 + 24 + 32};
    muteBgmButton = CircularButtonInit(muteBgmBtnPos, 16, gameTextures.bgmIcon, MuteUnmuteMusic);

    /*
    const Rectangle sliderRect = {24, 32, VIRTUAL_SCREEN_WIDTH - 24, 24};
    bgmVolSlider = (SliderButton*) malloc(sizeof(SliderButton*));
    bgmVolSlider->rectangle = sliderRect;
    bgmVolSlider->percent = DEFAULT_BGM_VOL;
    */
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
            CircularButtonMousePress(startButton, MOUSE_BUTTON_LEFT, mousePos);
            CircularButtonMouseRelease(startButton, MOUSE_BUTTON_LEFT, mousePos);

            CircularButtonMousePress(settingsButton, MOUSE_BUTTON_LEFT, mousePos);
            CircularButtonMouseRelease(settingsButton, MOUSE_BUTTON_LEFT, mousePos);
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
            CircularButtonMousePress(backButton, MOUSE_BUTTON_LEFT, mousePos);
            CircularButtonMouseRelease(backButton, MOUSE_BUTTON_LEFT, mousePos);

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsBallClearingFinished() && targetFps == initFps && !backButton->pressed)
            {
                BallClearerBegin(NULL, initFps);
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (RendererGetPaintPercentage() < .5f && ListLength(&ballHead) == 0)
                {
                    if (!RendererIsRingTransitionActive()) RendererPlayRingTransition(GameRestart);
                    // GameRestart();
                }
            }

            if (IsKeyPressed(KEY_ESCAPE)) GoBackToTitle();
            break;
        }

        case GAME_SETTINGS:
        {
            CircularButtonMousePress(backButton, MOUSE_BUTTON_LEFT, mousePos);
            CircularButtonMouseRelease(backButton, MOUSE_BUTTON_LEFT, mousePos);

            CircularButtonMousePress(windowModeButton, MOUSE_BUTTON_LEFT, mousePos);
            CircularButtonMouseRelease(windowModeButton, MOUSE_BUTTON_LEFT, mousePos);

            CircularButtonMousePress(muteBgmButton, MOUSE_BUTTON_LEFT, mousePos);
            CircularButtonMouseRelease(muteBgmButton, MOUSE_BUTTON_LEFT, mousePos);

            // SetMusicVolume(gameAudio.mainTheme, SliderButtonInput(bgmVolSlider, IsMouseButtonDown(MOUSE_BUTTON_LEFT), mousePos));

            if (IsKeyPressed(KEY_ESCAPE)) GoBackToTitle();
            break;
        }

        case GAME_TUTORIAL:
        {
            TutorialUpdate();
            if (IsKeyPressed(KEY_ESCAPE)) GoBackToTitle();
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
        LIST_MAP_DATA(ballHead, BallUpdate, (Ball *));
        LIST_MAP_DATA(ballSpawnPointHead, BallSpawnPointUpdate, (BallSpawnPoint *));
    }

    if (!freezePlayer)
    {
        PlayerUpdate(playerRef);
    }

    ParticlesUpdate();

    bool instantAdd = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gameState != GAME_PLAY && targetFps == initFps;
    ScoreHandlerAddToScoreFromBonusPool(instantAdd);

    // Toggle Fullscreen
    if ((IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) || IsKeyPressed(KEY_F11))
    {
        ToggleWindowMode();
    }

    // Toggle Debug Draw
    if (IsKeyPressed(KEY_D) && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)))
    {
        debugDrawing = !debugDrawing;
    }

    // Reset Tutorial
    if (IsKeyPressed(KEY_T) && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)))
    {
        if (gameState != GAME_TUTORIAL) TutorialReset();
    }
}

void GameRun()
{
    while (1)
    {
        frameCounter++;
        Update();
        RenderToTarget();
        RenderToScreen();

        if (WindowShouldClose() && (gameState == GAME_TITLE || !IsKeyPressed(KEY_ESCAPE))) break;
    }

    GameDeInit();
}

void GameEnd()
{
    if (gameState == GAME_OVER) return;
    gameState = GAME_OVER;
    ScoreHandlerLoseCombo();
    ScoreHandlerAddToScoreFromBonusPool(true);
    SaveStorageValue(STORAGE_POSITION_HISCORE, ScoreHandlerGetHiScore());
    ListRemoveAllNodes(&ballSpawnPointHead);
    ListRemoveAllNodes(&spawnQueueHead);
    targetFps = 2;
}

void GameRestart()
{
    /*
    gameState = GAME_RESTARTING;
    playerRef->stateExecute = STATE_EXEC_PLAYER_REVIVE;
    targetFps = 20;
    return;
    */

    Player *newPlayer = PlayerReset(playerRef, virtualScreenCenter, &ballHead);
    playerRef = newPlayer;

    GameStart();

    spawnDelay = BALL_SPAWN_DELAY_SHORT;
}

void GameDeInit()
{
    ScoreHandlerLoseCombo();
    ScoreHandlerAddToScoreFromBonusPool(true);
    SaveStorageValue(STORAGE_POSITION_HISCORE, ScoreHandlerGetHiScore());
    CloseWindow();
}

void GameFreezeAllEntities(bool enable)
{
    freezeBalls = enable;
    freezePlayer = enable;
}