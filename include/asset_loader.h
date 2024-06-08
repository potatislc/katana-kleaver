#ifndef BALL_GAME_ASSET_LOADER_H
#define BALL_GAME_ASSET_LOADER_H

#include "raylib.h"

#define MELON_SOUNDS_LENGTH (3)

typedef struct
{
    Texture2D floorStandard;
    Texture2D melonBig;
    Texture2D melonSmall;
    Texture2D melonShadow;
    Texture2D samurai;
    Texture2D samuraiShadow;
    Texture2D particleRound;
} GameTextures;

typedef struct
{
    Sound swordSlash;
    Sound dash;
    Sound melonSplats[MELON_SOUNDS_LENGTH];
} GameAudio;

extern GameTextures gameTextures;
extern GameAudio gameAudio;

void LoadGameTextures(); // Load all of it at once, the game is small
void LoadGameAudio();

#endif //BALL_GAME_ASSET_LOADER_H
