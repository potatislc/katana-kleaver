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
#include "firefly.h"

#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

const int initFps = 60;
int targetFps = 60;
int gameState = GAME_TITLE;
unsigned int frameCounter = 0;

CircularButton *startButton;
CircularButton *settingsButton;
CircularButton *backButton;
CircularButton *windowModeButton;
CircularButton *muteBgmButton;
CircularButton *replayTutButton;

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
    if (playerRef->stateExecute == STATE_EXEC_PLAYER_DEAD)
    {
        gameState = GAME_OVER;
        ScoreHandlerUnlockMedals(ScoreHandlerGetScore());
        return;
    }

    freezePlayer = false;

    playerRef->stateExecute = STATE_EXEC_PLAYER_MOVE;

    BallClearerForceFinish();

    BallDeSpawnAll();
    ListRemoveAllNodes(&ballSpawnPointHead);
    BallNbrCountReset(&ballNbrCount_All);
    ListRemoveAllNodes(&spawnQueueHead);
    ListRemoveAllNodes(&particleHead);
    ListRemoveAllNodes(&particleUiHead);

    SaveStorageValue(STORAGE_POSITION_HISCORE, ScoreHandlerGetHiScore());
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
    ScoreHandlerUnlockMedals(ScoreHandlerGetHiScore());
}

void ToggleWindowMode()
{
    WindowHandlerToggleWindowMode();
    RendererFitVirtualRectToScreen();
}

void ReplayTutorial()
{
    TutorialReset();
    GameStart();
}

void GameInit()
{
    PlayMusicStream(gameAudio.mainTheme);

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

    Vector2 windowModeBtnPos = {24, 24};
    windowModeButton = CircularButtonInit(windowModeBtnPos, 16, gameTextures.windowIcon, ToggleWindowMode);

    Vector2 muteBgmBtnPos = {24, 24 + 36};
    muteBgmButton = CircularButtonInit(muteBgmBtnPos, 16, gameTextures.bgmIcon, MuteUnmuteMusic);

    Vector2 replayTutPos = {24, 24 + 36 + 36};
    replayTutButton = CircularButtonInit(replayTutPos, 16, gameTextures.orange, ReplayTutorial);

    /*
    const Rectangle sliderRect = {24, 32, VIRTUAL_SCREEN_WIDTH - 24, 24};
    bgmVolSlider = (SliderButton*) malloc(sizeof(SliderButton*));
    bgmVolSlider->rectangle = sliderRect;
    bgmVolSlider->percent = DEFAULT_BGM_VOL;
    */

    ScoreHandlerUnlockMedals(ScoreHandlerGetHiScore());
}

void SpeedUpFpsEffect()
{
    targetFps++;
#ifndef PLATFORM_WEB
    SetTargetFPS(targetFps);
#endif
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

            ScoreHandlerRevealMedals();
            break;
        }

        case GAME_PLAY:
        {
            SetUiProgressBarMidToEnds(&spawningProgressBar, GetTime() - timeSinceLastSpawn, spawnDelay);
            SpawnerUpdate();
            break;
        }

        case GAME_OVER:
        {
            CircularButtonMousePress(backButton, MOUSE_BUTTON_LEFT, mousePos);
            CircularButtonMouseRelease(backButton, MOUSE_BUTTON_LEFT, mousePos);

            if (targetFps == initFps)
            {
                if (!ScoreHandlerMedalRevealFinished()) ScoreHandlerRevealMedals();

                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsBallClearingFinished() && !backButton->pressed)
                {
                    BallClearerBegin(NULL, initFps);
                }
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (RendererGetPaintPercentage() < .5f && ListLength(&ballHead) == 0)
                {
                    if (!RendererIsRingTransitionActive()) RendererPlayRingTransition(GameRestart);
                    // GameRestart();
                }
            }
            break;
        }

        case GAME_SETTINGS:
        {
            CircularButtonMousePress(backButton, MOUSE_BUTTON_LEFT, mousePos);
            CircularButtonMouseRelease(backButton, MOUSE_BUTTON_LEFT, mousePos);

#ifdef PLATFORM_DESKTOP
            CircularButtonMousePress(windowModeButton, MOUSE_BUTTON_LEFT, mousePos);
            CircularButtonMouseRelease(windowModeButton, MOUSE_BUTTON_LEFT, mousePos);
#endif

            CircularButtonMousePress(muteBgmButton, MOUSE_BUTTON_LEFT, mousePos);
            CircularButtonMouseRelease(muteBgmButton, MOUSE_BUTTON_LEFT, mousePos);

            CircularButtonMousePress(replayTutButton, MOUSE_BUTTON_LEFT, mousePos);
            CircularButtonMouseRelease(replayTutButton, MOUSE_BUTTON_LEFT, mousePos);

            // SetMusicVolume(gameAudio.mainTheme, SliderButtonInput(bgmVolSlider, IsMouseButtonDown(MOUSE_BUTTON_LEFT), mousePos));
            break;
        }

        case GAME_TUTORIAL:
        {
            TutorialUpdate();
            break;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) GoBackToTitle();

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
#ifdef PROFILE_DEBUG
    if (IsKeyPressed(KEY_D) && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)))
    {
        debugDrawing = !debugDrawing;
    }
#endif
}

void GameRun()
{
#ifdef PLATFORM_DESKTOP
    while (1)
    {
        frameCounter++;
        Update();
        RenderToTarget();
        RenderToScreen();

        if (WindowShouldClose() && (gameState == GAME_TITLE || !IsKeyPressed(KEY_ESCAPE))) break;
    }
#elif PLATFORM_WEB
    frameCounter++;
    Update();
    RenderToTarget();
    RenderToScreen();
    // Is this cheating?
    emscripten_cancel_main_loop();
    emscripten_set_main_loop(GameRun, targetFps, 1);
#endif
}

void GameEnd()
{
    if (gameState == GAME_OVER) return;
    ScoreHandlerLoseCombo();
    ScoreHandlerUnlockMedals(ScoreHandlerGetScore() + ScoreHandlerGetBonusScorePool());
    if (gameState != GAME_TUTORIAL) ScoreHandlerUpdateHiScore();
    SaveStorageValue(STORAGE_POSITION_HISCORE, ScoreHandlerGetHiScore());
    gameState = GAME_OVER;
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