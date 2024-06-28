#ifndef BALL_GAME_RENDERER_H
#define BALL_GAME_RENDERER_H

#include "raylib.h"

#define VIRTUAL_SCREEN_WIDTH (160)
#define VIRTUAL_SCREEN_HEIGHT (160)
#define VIRTUAL_SCREEN_OFFSET_X (0)
#define VIRTUAL_SCREEN_OFFSET_Y (16)

void RendererInit();
void RenderSplashScreenToTarget();
void RendererDrawGame();
void RendererDrawUi();
void RendererDrawDebug();
void RendererFitVirtualRectToScreen();
void RenderToTarget();
void RenderToScreen();
void RendererClearBackgroundPaint();
float RendererGetPaintPercentage();
void RendererPlayRingTransition(void (*transitionFunction)());
bool RendererIsRingTransitionActive();

extern Vector2 screenRatio;
extern Vector2 screenOffset;
extern Vector2 virtualScreenCenter;
extern float eraserRadius;

extern bool debugDrawing;

typedef struct {
    float width;
    float start;
    float end;
} UiProgressBar;

extern UiProgressBar spawningProgressBar;
void SetUiProgressBarLToR(UiProgressBar *uiProgressBar, double current, double end);
void SetUiProgressBarMidToEnds(UiProgressBar *uiProgressBar, double current, double end);

typedef struct
{
    Rectangle source;
    Rectangle dest;
    Vector2 origin;
} TextureTransformRect;

#endif //BALL_GAME_RENDERER_H
