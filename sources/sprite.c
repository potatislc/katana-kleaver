#include "sprite.h"
#include <stdlib.h>
#include "raymath.h"
#include "global.h"

Sprite *SpriteInit(Texture2D spriteSheet, Rectangle frameRect, int startingFrame, bool centered)
{
    Sprite *sprite = (Sprite*)malloc(sizeof(Sprite));

    sprite->sheet = spriteSheet;
    sprite->frameRect = frameRect;
    sprite->frame = startingFrame;
    SpriteSetFrame(sprite, startingFrame);
    sprite->frameCount = spriteSheet.width / (int)frameRect.width;
    sprite->animProgress = (float)startingFrame / (float)sprite->frameCount;
    sprite->drawingOffset = (centered) ? (Vector2){frameRect.width / 2, frameRect.height / 2} : Vector2Zero();

    return sprite;
}

void SpriteSetFrame(Sprite *sprite, int frame)
{
    sprite->frame = frame;
    sprite->frameRect.x = sprite->frameRect.width * (float)frame;
}

void SpriteAnimate(Sprite *sprite, float speed, bool looping)
{
    sprite->animProgress += speed;

    if (!looping)
    {
        sprite->animProgress = fmaxf(0.f, fminf(sprite->animProgress, 1.f));
        SpriteSetFrame(sprite, (int)(sprite->animProgress * (float)(sprite->frameCount-1)));
    }
    else
    {
        sprite->animProgress = fmodf(sprite->animProgress + 1.f, 1.f);
        SpriteSetFrame(sprite, (int)(sprite->animProgress * (float)sprite->frameCount));
    }
}

void SpriteDraw(Sprite sprite, Vector2 position, Vector2 scale, float rotation)
{
    Rectangle source = {sprite.frameRect.x, sprite.frameRect.y, sprite.frameRect.width * scale.x, sprite.frameRect.height * scale.y};
    position = Vector2Round(position);
    Rectangle dest = {position.x, position.y, sprite.frameRect.width, sprite.frameRect.height};
    Vector2 scaledOffset = {fabsf(sprite.drawingOffset.x * scale.x), fabsf(sprite.drawingOffset.y * scale.y)};

    DrawTexturePro(sprite.sheet, source, dest, scaledOffset, rotation, WHITE);
}