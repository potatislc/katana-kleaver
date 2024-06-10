#include <stdlib.h>
#include <math.h>
#include "sprite.h"

Sprite *SpriteInit(Texture2D spriteSheet, Rectangle frameRect, int startingFrame)
{
    Sprite *sprite = (Sprite*)malloc(sizeof(Sprite));

    sprite->sheet = spriteSheet;
    sprite->frameRect = frameRect;
    sprite->frame = startingFrame;
    sprite->frameCount = spriteSheet.width / (int)frameRect.width;

    return sprite;
}

void SpriteSetFrame(Sprite *sprite, int frame)
{
    sprite->frame = frame;
    sprite->frameRect.x = sprite->frameRect.width * (float)frame;
}

void SpriteAnimate(Sprite *sprite, float speed, bool looping)
{
    int nextFrame = sprite->frame + 1; // The 1 is temporary

    if (!looping)
    {
        nextFrame = (int)fmax(0, fmin(nextFrame, sprite->sheet.width));
    }
    else
    {
        if (nextFrame > sprite->frameCount)
        {
            nextFrame = 0;
        }
        else if (nextFrame < 0)
        {
            nextFrame = sprite->frameCount;
        }
    }

    SpriteSetFrame(sprite, nextFrame);
}