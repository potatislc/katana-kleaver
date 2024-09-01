#ifndef BALL_GAME_GAME_H
#define BALL_GAME_GAME_H

#include <stdbool.h>
#include "circular_button.h"
#include "slider_button.h"
#include "sprite.h"

#define FRAME_COUNTER_TO_TIME ((double)frameCounter / initFps)
#define IS_GAME_STATE_PLAYABLE (gameState == GAME_PLAY || gameState == GAME_TUTORIAL)

#define BALL_SPAWN_DELAY_FIRST (4.0)
#define BALL_SPAWN_DELAY_LONG (3.0)
#define BALL_SPAWN_DELAY_SHORT (1.0)

void GameInit();
void GameStart();
void GameRun();
void GameEnd();
void GameRestart();
void GameDeInit();
void GameFreezeAllEntities(bool enable);

extern const int initFps;
extern int targetFps;
extern unsigned int frameCounter;

enum GameStates
{
    GAME_TITLE,
    GAME_PLAY,
    GAME_OVER,
    GAME_TUTORIAL,
    GAME_SETTINGS,
    GAME_PLAY_TUTORIAL_POPUP,
    GAME_RESTARTING
};

extern int gameState;
extern CircularButton *startButton;
extern CircularButton *settingsButton;
extern CircularButton *backButton;
extern CircularButton *windowModeButton;
extern CircularButton *muteBgmButton;
extern CircularButton *replayTutButton;
extern CircularButton *muteSfxButton;
extern CircularButton *playTutButton;
extern CircularButton *skipTutButton;

/*
extern SliderButton *bgmVolSlider;
extern SliderButton *sfxVolSlider;
extern SliderButton *screenShakeSlider;
*/

//
//  SETUP
//

void Setup();

//
// SPLASH SCREEN
//

typedef enum
{
    SM_STUDIO_NAME,
    SM_THIS_GAME_USES_MOUSE,
    SM_LENGTH
} SplashMessage;

extern SplashMessage splashMessageIndex;
extern Sprite *splashMouseIcon;

void SplashScreenInit();
void SplashScreenRun();
void SplashScreenEnd();

#endif //BALL_GAME_GAME_H
