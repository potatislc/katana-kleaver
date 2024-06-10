#ifndef BALL_GAME_SPRITE_H
#define BALL_GAME_SPRITE_H

#include "raylib.h"

typedef struct
{
    Texture2D sheet;
    Rectangle frameRect;
    int frame;
    int frameCount;
    float animProgress;
    int fps;
    Vector2 drawingOffset;
} Sprite;

Sprite *SpriteInit(Texture2D spriteSheet, Rectangle frameRect, int startingFrame, bool centered);
void SpriteSetFrame(Sprite *sprite, int frame);
void SpriteAnimate(Sprite *sprite, float speed, bool looping);
void SpriteDraw(Sprite sprite, Vector2 position, Vector2 scale, float rotation);

#endif //BALL_GAME_SPRITE_H
