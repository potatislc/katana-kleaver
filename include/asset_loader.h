#ifndef BALL_GAME_ASSET_LOADER_H
#define BALL_GAME_ASSET_LOADER_H

#include "raylib.h"

#define MELON_SOUNDS_LENGTH (3)
#define WIPE_SOUNDS_LENGTH (2)

#define DEFAULT_BGM_VOL (.5f)

typedef struct
{
    Texture2D titleText;
    Texture2D bgFloorStandard;
    Texture2D bgBallClear;
    Texture2D tvBorder;
    Texture2D melonBig;
    Texture2D melonSmall;
    Texture2D melonShadow;
    Texture2D orange;
    Texture2D orangeHalo;
    Texture2D samurai;
    Texture2D samuraiRunSheet;
    Texture2D samuraiDieSheet;
    Texture2D samuraiShadow;
    Texture2D samuraiTarget;
    Texture2D particleRound;
    Texture2D particlePixel;
    Texture2D settingsIcon;
    Texture2D arrowIcon;
    Texture2D playIcon;
    Texture2D katanaIcon;
    Texture2D infoQuit;
    Texture2D infoMiss;
    Texture2D windowIcon;
    Texture2D bgmIcon;
    Texture2D mouseIcon;
    Texture2D medals;
} GameTextures;

typedef struct
{
    Sound swordSlash;
    Sound dash;
    Sound melonSplats[MELON_SOUNDS_LENGTH];
    Sound deathScream;
    Sound footstep;
    Sound ballSpawn;
    Sound orangeSpawn;
    Sound postBallClarity;
    Sound bonusScore;
    Sound loseCombo;
    Sound gainCombo;
    Sound gainMultiply;
    Sound medalGot;
    Sound tutorialClear;
    Sound tutorialText;
    Sound wipes[WIPE_SOUNDS_LENGTH];
    Music mainTheme;
} GameAudio;

extern GameTextures gameTextures;
extern GameAudio gameAudio;

void LoadGameTextures(); // Load all of it at once, the game is small
void LoadGameAudio();

void MuteUnmuteMusic();
extern bool muted;

#endif //BALL_GAME_ASSET_LOADER_H
