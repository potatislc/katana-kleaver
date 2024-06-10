#ifndef BALL_GAME_SPRITE_H
#define BALL_GAME_SPRITE_H

#include "raylib.h"

typedef struct
{
    Texture2D sheet;
    Rectangle frameRect;
    int frame;
    int frameCount;
    int fps;
} Sprite;

Sprite *SpriteInit(Texture2D spriteSheet, Rectangle frameRect, int startingFrame);
void SpriteSetFrame(Sprite *sprite, int frame);
void SpriteAnimate(Sprite *sprite, float speed, bool looping);
void SpriteDraw(Sprite sprite, Vector2 position);

#endif //BALL_GAME_SPRITE_H
