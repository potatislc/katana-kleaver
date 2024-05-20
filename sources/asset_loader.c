#include "raylib.h"

Texture2D melonBig;
Texture2D melonSmall;
Texture2D floorStandard;

Texture2D samurai;
Texture2D samuraiShadow;

void LoadGameTextures()
{
    floorStandard = LoadTexture("../assets/floor.png");
    melonBig = LoadTexture("../assets/melon_big.png");
    melonSmall = LoadTexture("../assets/melon_small.png");
    samurai = LoadTexture("../assets/samurai/samurai.png");
    samuraiShadow = LoadTexture("../assets/samurai/samurai_shadow.png");
}