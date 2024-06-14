#include <stdio.h>
#include "renderer.h"
#include "asset_loader.h"
#include "player.h"
#include "window_handler.h"
#include "global.h"
#include "score_handler.h"
#include "particle.h"
#include "raymath.h"
#include "game.h"
#include "camera.h"

Vector2 virtualScreenCenter = {VIRTUAL_SCREEN_WIDTH / 2.0f, VIRTUAL_SCREEN_HEIGHT / 2.0f };

RenderTexture2D virtualRenderTarget;
RenderTexture2D backgroundPaintTarget;
TextureTransformRect virtualRenderRect;
TextureTransformRect backgroundPaintRect;

Vector2 screenOffset;
Vector2 screenRatio;

UiProgressBar spawnProgressBar = {VIRTUAL_SCREEN_WIDTH, 0, 0};

Texture2D *bgTexture;

char getReadyText[] = "- Get Ready! -";
int getReadyTextWidth = 0;

Vector2 titleScreenOffset;

float eraserRadius = 31.f;

typedef struct
{
    float startAngle;
    float endAngle;
    double duration;
    double timeSinceLastTransition;
} RingTransition;

RingTransition ringTrans = {360.f, 360.f, .8, 0};

typedef struct
{
    Color color;
    float radius;
    int blendMode;
    double sprayDuration;
    double timeSinceLastSpray;
} SprayCan;

SprayCan sprayCanFoam = {WHITE, 16.f, BLEND_SUBTRACT_COLORS, .15, 0};

void RendererInit()
{
    virtualRenderTarget = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT+VIRTUAL_SCREEN_OFFSET_Y);
    virtualRenderRect.source = (Rectangle){ 0.f, 0.f, (float)virtualRenderTarget.texture.width, -(float)virtualRenderTarget.texture.height };
    virtualRenderRect.dest = (Rectangle){0.f, 0.f ,0.f, 0.f};
    virtualRenderRect.origin = Vector2Zero();
    worldSpaceCamera.zoom = 1.0f;
    RendererFitVirtualRectToScreen();

    backgroundPaintTarget = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
    backgroundPaintRect.source = (Rectangle){ 0.f, 0.f, (float)backgroundPaintTarget.texture.width, -(float)backgroundPaintTarget.texture.height };
    backgroundPaintRect.dest = (Rectangle){0.f, 0.f ,0.f, 0.f};
    backgroundPaintRect.origin = Vector2Zero();

    bgTexture = &gameTextures.bgFloorStandard;

    getReadyTextWidth = MeasureText(getReadyText, 8);
}

void RendererFitVirtualRectToScreen()
{
    int windowHeight = GetScreenHeight();
    float aspectRatio = (float)DEFAULT_SCREEN_WIDTH / (float)DEFAULT_SCREEN_HEIGHT;
    int newWidth = (int)((float)windowHeight * aspectRatio); // Calculate the new width based on the aspect ratio

    screenOffset = (Vector2){(float)(GetScreenWidth() - newWidth) / 2.f, 0.f};

    screenRatio = (Vector2){
            ((float)newWidth) / (float)VIRTUAL_SCREEN_WIDTH,
            (float)GetScreenHeight() / (float)(VIRTUAL_SCREEN_HEIGHT+VIRTUAL_SCREEN_OFFSET_Y)};

    virtualRenderRect.dest = (Rectangle){
            screenOffset.x,
            screenOffset.y,
            (float)newWidth,
            (float)GetScreenHeight()};
}

void DrawBackground()
{
    if (!IsBallClearingFinished() && gameState == GAME_PLAY)
    {
        if (bgTexture != &gameTextures.bgBallClear)
        {
            bgTexture = &gameTextures.bgBallClear;
            DrawTexture(*bgTexture, 0, 0, WHITE);
        }
    }
    else
    {
        bgTexture = &gameTextures.bgFloorStandard;
        DrawTexture(*bgTexture, 0, 0, WHITE);

        Color paintBgAlphaColor = WHITE;
        paintBgAlphaColor.a = 200;
        DrawTextureRec(backgroundPaintTarget.texture, backgroundPaintRect.source, backgroundPaintRect.origin, paintBgAlphaColor);
    }
}

void DrawShadows()
{
    PlayerDrawShadow(*playerRef);

    ListNode* currentBallNode = ballHead;
    while (currentBallNode != NULL)
    {
        BallDrawShadow(*(Ball*)currentBallNode->data);
        currentBallNode = currentBallNode->next;
    }
}

void DrawEntities()
{
    ListNode* currentBallNode = ballHead;

    while (currentBallNode != NULL)
    {
        BallDraw(*(Ball*)currentBallNode->data);
        currentBallNode = currentBallNode->next;
    }

    ListNode* currentBallSpawnPointNode = ballSpawnPointHead;
    while (currentBallSpawnPointNode != NULL)
    {
        BallSpawnPointDraw(*(BallSpawnPoint*)currentBallSpawnPointNode->data);
        currentBallSpawnPointNode = currentBallSpawnPointNode->next;
    }

    PlayerDraw(*playerRef);
}

void DrawParticles()
{
    ListNode* currentParticleNode = particleHead;

    while (currentParticleNode != NULL) {
        ParticleDraw(*(Particle *) currentParticleNode->data);
        currentParticleNode = currentParticleNode->next;
    }
}

void DrawParticlesToBackgroundPaint()
{
    ListNode* currentParticleNode = particleHead;

    BeginTextureMode(backgroundPaintTarget);
    {
        while (currentParticleNode != NULL) {
            ParticleDraw(*(Particle *) currentParticleNode->data);
            currentParticleNode = currentParticleNode->next;
        }
    }
    EndTextureMode();
}

void RendererClearBackgroundPaint()
{
    BeginTextureMode(backgroundPaintTarget);
    {
        ClearBackground(BLANK);
    }
    EndTextureMode();
}

void EraseBackgroundPaint(Vector2 position, float radius)
{
    BeginTextureMode(backgroundPaintTarget);
    {
        BeginBlendMode(BLEND_SUBTRACT_COLORS);
        {
            DrawCircleV(position, radius, BLANK);
        }
        EndBlendMode();
    }
    EndTextureMode();
}

void SprayBackgroundPaint(Vector2 position, SprayCan sprayCan)
{
    position = (Vector2){position.x + (float)cos(GetTime() * 4) * 4, position.y + (float)sin(GetTime() * 7) * 7};

    BeginTextureMode(backgroundPaintTarget);
    {
        BeginBlendMode(BLEND_SUBTRACT_COLORS);
        {
            DrawCircleV(position, sprayCan.radius, sprayCan.color);
        }
        EndBlendMode();
    }
    EndTextureMode();
}

void SetUiProgressBarLToR(UiProgressBar *uiProgressBar, double current, double end)
{
    // Left to right
    uiProgressBar->start = 0;
    uiProgressBar->end = (float)(current / end) * uiProgressBar->width;
}

void SetUiProgressBarMidToEnds(UiProgressBar *uiProgressBar, double current, double end)
{
    float progress = (float)(current / end);
    float maxLength = uiProgressBar->width / 2.f;
    uiProgressBar->start = maxLength - progress * maxLength;
    uiProgressBar->end = maxLength + progress * maxLength;
}

void DrawUiDeathRing()
{
    Vector2 center = playerRef->position;

    DrawRing(center, (float)(targetFps) * ((float)targetFps / 12.f), 240, 0, 360, 32, BLACK);
}

void DrawUiScore()
{
    DrawText(scoreText, 5, VIRTUAL_SCREEN_HEIGHT + 3, 8, uiColorYellow);

    if (ScoreHandlerGetComboScore() > 1)
    {
        DrawText(comboText, 69, VIRTUAL_SCREEN_HEIGHT + 3, 8, uiColorRed);
    }
}

void DrawUiProgressBar()
{
    const Vector2 progLineStart = (Vector2){spawnProgressBar.start, VIRTUAL_SCREEN_HEIGHT};
    const Vector2 progLineEnd = (Vector2){spawnProgressBar.end, VIRTUAL_SCREEN_HEIGHT};
    DrawLineV(progLineStart, progLineEnd, uiColorDarkGray);
}

void DrawUiGameOver()
{
    DrawText(gameOverText, (int)virtualScreenCenter.x - gameOverTextWidth / 2, (int)virtualScreenCenter.y-8, 8, WHITE);
    if (frameCounter / 15 % 2 == 0) DrawText(restartText, (int)virtualScreenCenter.x - restartTextWidth / 2, (int)virtualScreenCenter.y+64, 8, WHITE);

    scoreTextWidth = MeasureText(scoreText, 8);
    DrawText(scoreText, (int)virtualScreenCenter.x - scoreTextWidth / 2, (int)virtualScreenCenter.y+12, 8, WHITE);

    DrawText(hiScoreText, (int)virtualScreenCenter.x - hiScoreTextWidth / 2, (int)virtualScreenCenter.y+24, 8, WHITE);
}

void DrawUiRingTransition()
{
    double progress = (GetTime() - ringTrans.timeSinceLastTransition) / ringTrans.duration;

    if (progress > 1.f) return;

    ringTrans.endAngle = fminf((float)progress * 2.f * 360.f, 360.f);

    DrawRing(virtualScreenCenter, 0, 240, ringTrans.startAngle, ringTrans.endAngle, 32, BLACK);

    if (ringTrans.endAngle == 360.f)
    {
        if (ringTrans.startAngle == 0.f) GameRestart(); // Hard Coded haha

        ringTrans.startAngle = fmaxf(((float)progress-.5f) * 2.f * 360.f, 0.f);
    }
}

bool RendererIsRingTransitionActive()
{
    return GetTime() <= ringTrans.timeSinceLastTransition + ringTrans.duration;
}

void RendererPlayRingTransition()
{
    ringTrans.timeSinceLastTransition = GetTime();
    ringTrans.startAngle = 0;
    ringTrans.endAngle = 0;

    PlaySound(gameAudio.dash);
}

void DrawUiBallClearerTarget()
{
    if (BallClearerTarget() == NULL) return;

    Ball *targetBall = BallClearerTarget();
    Vector2 center = targetBall->position;
    float radius = targetBall->radius;
    float rotation = (float)GetTime() * 1.5f;
    float rAng = PI / 2;

    Vector2 dirs[4] = {
            {cosf(rotation), sinf(rotation)},
            {cosf(rotation + rAng), sinf(rotation + rAng)},
            {cosf(rotation + PI), sinf(rotation + PI)},
            {cosf(rotation + PI + rAng), sinf(rotation + PI + rAng)}
    };

    Color triColor = orangeColor;
    float triLength = 16;
    float triWidth = 8;

    for (int i = 0; i < 4; i++) {
        Vector2 dir = dirs[i];
        Vector2 dirNext = dirs[(i + 1) % 4];

        Vector2 outerVertPivot = Vector2Add(center, (Vector2){dir.x * (radius + triLength), dir.y * (radius + triLength)});
        Vector2 outerVertCorner = {dirNext.x * triWidth, dirNext.y * triWidth};
        DrawTriangleLines(
                Vector2Add(center, (Vector2){dir.x * radius, dir.y * radius}),
                Vector2Add(outerVertPivot, outerVertCorner),
                Vector2Subtract(outerVertPivot, outerVertCorner),
                triColor);
    }
}

void DrawUi()
{
    // Borders
    DrawRectangle(0, VIRTUAL_SCREEN_HEIGHT, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_OFFSET_Y, BLACK);
    DrawTexture(gameTextures.tvBorder, 0, 0, WHITE);

    Vector2 mousePos = Vector2ClampInsideScreen(Vector2Round(Vector2ToVirtualCoords(GetMousePosition())), 2);

    switch(gameState)
    {
        case GAME_TITLE:
        {
            double speed = GetTime() * 2;
            titleScreenOffset = (Vector2) {(float) cos(speed) * 8, (float) sin(speed * 2) * 4};
            DrawTextureV(gameTextures.titleText, titleScreenOffset, WHITE);
            break;
        }

        case GAME_PLAY:
            if (ballNbrCount_All.spawned == 0 && frameCounter / 30 % 2 == 0)
            {
                DrawText(getReadyText, (int)virtualScreenCenter.x - getReadyTextWidth / 2, (int)virtualScreenCenter.y-32, 8, WHITE);
            }

            DrawUiScore();
            DrawUiProgressBar();

            if (!IsBallClearingFinished()) DrawUiBallClearerTarget();
            break;
        case GAME_OVER:
            if (targetFps != initFps)
            {
                DrawUiDeathRing();
            }
            else
            {
                DrawUiGameOver();
            }

            DrawUiScore();
            DrawUiProgressBar();

            DrawCircleLinesV(mousePos, eraserRadius, guideColor);

            if (targetFps == initFps)
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    EraseBackgroundPaint(mousePos, eraserRadius);
                }
                else if (GetTime() < sprayCanFoam.timeSinceLastSpray + sprayCanFoam.sprayDuration)
                {
                    SprayBackgroundPaint(mousePos, sprayCanFoam);
                }

                if (IsKeyPressed(KEY_S)) sprayCanFoam.timeSinceLastSpray = GetTime();
            }
            break;
    }

    DrawUiRingTransition();

    DrawCircleV(mousePos, 2, guideColor);
}


float RendererGetPaintPercentage()
{
    Image image = LoadImageFromTexture(backgroundPaintTarget.texture);

    Color *pixels = LoadImageColors(image);

    int totalPixels = image.width * image.height;

    int blankPixelCount = 0;
    for (int i = 0; i < totalPixels; i++)
    {
        if (pixels[i].a == 0) blankPixelCount++;
    }

    UnloadImageColors(pixels);
    UnloadImage(image);

    float blankPixelPercentage = (float)(totalPixels - blankPixelCount) / (float)totalPixels * 100.0f;
    return blankPixelPercentage;
}

void RenderToTarget()
{
    DrawParticlesToBackgroundPaint();
    CameraShakeUpdate();

    BeginTextureMode(virtualRenderTarget);
    {
        BeginMode2D(worldSpaceCamera);
        {
            DrawBackground();
            DrawShadows();
            DrawEntities();
            DrawParticles();
        }
        EndMode2D();
        DrawUi();
    }
    EndTextureMode();
}

void DrawDebug()
{
    DrawFPS(10, 10);

    char windowModeText[32];
    sprintf(windowModeText, "WINDOW MODE %d", WindowHandlerGetWindowMode());
    DrawText(windowModeText, 10, 30, 21, DARKGREEN);

    char ballCountText[32];
    sprintf(ballCountText, "BALLS ON SCREEN %d", NbrOfBallsOnScreen(ballNbrCount_All));
    DrawText(ballCountText, 10, 50, 21, DARKGREEN);
}

void RenderToScreen()
{
    BeginDrawing();
    {
        ClearBackground(BLACK);

        // Draw world camera to screen
        DrawTexturePro(virtualRenderTarget.texture, virtualRenderRect.source, virtualRenderRect.dest, virtualRenderRect.origin, 0.0f, WHITE);

        // DrawDebug();
    }
    EndDrawing();
}