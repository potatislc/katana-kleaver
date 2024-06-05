#ifndef BALL_GAME_RENDERER_H
#define BALL_GAME_RENDERER_H

#include "raylib.h"

#define VIRTUAL_SCREEN_WIDTH (160)
#define VIRTUAL_SCREEN_HEIGHT (160)
#define VIRTUAL_SCREEN_OFFSET_X (0)
#define VIRTUAL_SCREEN_OFFSET_Y (16)

void RendererInit();
void RendererDrawGame();
void RendererDrawUi();
void RendererDrawDebug();
void RendererFitVirtualRectToScreen();
void RenderToTarget(bool gameOver);
void RenderToScreen();
void RendererClearBackgroundPaint();

extern Vector2 screenRatio;
extern Vector2 screenOffset;
extern Vector2 virtualScreenCenter;

typedef struct
{
    Rectangle source;
    Rectangle dest;
    Vector2 origin;
} TextureTransformRect;

#endif //BALL_GAME_RENDERER_H
