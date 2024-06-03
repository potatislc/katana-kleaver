#include "renderer.h"
#include "window_handler.h"
#include "asset_loader.h"

Vector2 virtualScreenCenter = {VIRTUAL_SCREEN_WIDTH / 2.0f, VIRTUAL_SCREEN_HEIGHT / 2.0f };

RenderTexture2D virtualRenderTarget;

Camera2D worldSpaceCamera = { 0 };

Rectangle sourceRec = { 0.f, 0.f, 0.f, 0.f};
Rectangle destRec = { 0.f, 0.f, 0.f, 0.f};
Vector2 origin = {0.f};

void RendererInit()
{
    virtualRenderTarget = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT+VIRTUAL_SCREEN_OFFSET_Y);
    sourceRec = (Rectangle){ 0.f, 0.f, (float)virtualRenderTarget.texture.width, -(float)virtualRenderTarget.texture.height };
    worldSpaceCamera.zoom = 1.0f;
    RendererFitVirtualRectToScreen();
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

    destRec = (Rectangle){
            screenOffset.x,
            screenOffset.y,
            (float)newWidth,
            (float)GetScreenHeight()};
}

void DrawShadows()
{

}

void DrawEntities()
{

}

void DrawBackground()
{
    // Draw floor
    DrawTexture(gameTextures.floorStandard, 0, 0, WHITE);
}

void DrawUi()
{

}

void RenderToTarget()
{
    BeginTextureMode(virtualRenderTarget);
        BeginMode2D(worldSpaceCamera);
            DrawBackground();
            DrawShadows();
            DrawEntities();
            DrawUi();
        EndMode2D();
    EndTextureMode();
}

void RenderToScreen()
{
    BeginDrawing();
        ClearBackground(BLACK);

        // Draw world camera to screen
        DrawTexturePro(virtualRenderTarget.texture, sourceRec, destRec, origin, 0.0f, WHITE);

        // Debug stuff
        DrawFPS(10, 10);
    EndDrawing();
}