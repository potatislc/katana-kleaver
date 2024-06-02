#include "asset_loader.h"

GameTextures gameTextures;
GameAudio gameAudio;

void LoadGameTextures()
{
    gameTextures.floorStandard = LoadTexture("../assets/textures/floor.png");
    gameTextures.melonBig = LoadTexture("../assets/textures/melon/melon_big.png");
    gameTextures.melonSmall = LoadTexture("../assets/textures/melon/melon_small.png");
    gameTextures.melonShadow = LoadTexture("../assets/textures/melon/melon_shadow_big.png");
    gameTextures.samurai = LoadTexture("../assets/textures/samurai/samurai.png");
    gameTextures.samuraiShadow = LoadTexture("../assets/textures/samurai/samurai_shadow.png");
}

void MixGameAudio()
{
    SetSoundVolume(gameAudio.swordSlash, .6f);
}

void LoadGameAudio()
{
    gameAudio.swordSlash = LoadSound("../assets/sfx/sword_slash.wav");
    gameAudio.dash = LoadSound("../assets/sfx/dash.wav");

    MixGameAudio();
}