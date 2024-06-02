#include "asset_loader.h"

GameTextures gameTextures;

void LoadGameTextures()
{
    gameTextures.floorStandard = LoadTexture("../assets/floor.png");
    gameTextures.melonBig = LoadTexture("../assets/melon_big.png");
    gameTextures.melonSmall = LoadTexture("../assets/melon_small.png");
    gameTextures.melonShadow = LoadTexture("../assets/melon_shadow_big.png");
    gameTextures.samurai = LoadTexture("../assets/samurai/samurai.png");
    gameTextures.samuraiShadow = LoadTexture("../assets/samurai/samurai_shadow.png");
}