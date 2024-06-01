#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "ball.h"
#include "player.h"
#include "global.h"
#include "linked_list.h"
#include "asset_loader.h"

#define WINDOW_TITLE "Ball Game"

static void ToggleFullscreenWindow()
{
    if (IsWindowFullscreen())
    {
        ToggleFullscreen();
        SetWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    }
    else
    {
        int display = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        ToggleFullscreen();
        // SetWindowPosition(GetMonitorWidth(display)/2-GetScreenWidth()/2, 0);
    }

    screenRatio = (Vector2){
            (float)GetScreenWidth() / (float)VIRTUAL_SCREEN_WIDTH,
            (float)GetScreenHeight() / (float)VIRTUAL_SCREEN_HEIGHT};
}

int main(void)
{
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, WINDOW_TITLE);

    Vector2 vScreenCenter = {VIRTUAL_SCREEN_WIDTH / 2.0f, VIRTUAL_SCREEN_HEIGHT / 2.0f };

    RenderTexture2D target = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT+VIRTUAL_SCREEN_OFFSET_Y);

    Camera2D worldSpaceCamera = { 0 };
    worldSpaceCamera.zoom = 1.0f;

    Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
    Rectangle destRec = {-screenRatio.x, -screenRatio.y, (float)GetScreenWidth() + (screenRatio.x * 2), (float)GetScreenHeight() + (screenRatio.y * 2) };

    Vector2 origin = { 0.0f, 0.0f };

    LoadGameTextures();

    Player *player = PlayerInit(vScreenCenter, &ballHead);

    // Set Seed
    srand(time(0));

    SetTargetFPS(60);
    ToggleFullscreenWindow();
    //----------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        if (IsKeyPressed(KEY_SPACE))
        {
            float testRadius = 32.0f;
            Ball *newBall = BallInit(
                     (Vector2) {testRadius, testRadius},
                     (Vector2) {VIRTUAL_SCREEN_WIDTH - testRadius, VIRTUAL_SCREEN_HEIGHT - testRadius},
                     testRadius);

            BallSpawnPoint *newBallSpawn = BallSpawnPointInit(newBall, ballSpawnTime);
            ListNodePush(&ballSpawnPointHead, newBallSpawn);
        }

        if (!freezeBalls)
        {
            // Updating linked list of balls
            ListNode* currentBallNode = ballHead;
            while (currentBallNode != NULL)
            {
                BallUpdate(currentBallNode->data);
                currentBallNode = currentBallNode->next;
            }

            ListNode* currentBallSpawnPointNode = ballSpawnPointHead;
            while (currentBallSpawnPointNode != NULL)
            {
                BallSpawnPointUpdate(currentBallSpawnPointNode->data);
                currentBallSpawnPointNode = currentBallSpawnPointNode->next;
            }
        }

        PlayerUpdate(player);
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        BeginTextureMode(target);

            // Draw Game World
            BeginMode2D(worldSpaceCamera);
                // Draw floor
                DrawTexture(floorStandard, 0, 0, WHITE);

                // Draw Shadows
                PlayerDrawShadow(*player);

                ListNode* currentBallNode = ballHead;
                while (currentBallNode != NULL)
                {
                    BallDrawShadow(*(Ball*)currentBallNode->data);
                    currentBallNode = currentBallNode->next;
                }

                // Draw characters
                currentBallNode = ballHead;
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

                PlayerDraw(*player);

                // Draw UI
                DrawRectangle(0, VIRTUAL_SCREEN_HEIGHT, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_OFFSET_Y, BLACK);
                char scoreText[32];
                sprintf(scoreText, "Score: %d", score);
                DrawText(scoreText, 5, VIRTUAL_SCREEN_HEIGHT + 3, 8, uiColorYellow);

                if (comboScore > 1)
                {
                    char comboText[16];
                    sprintf(comboText, "x%d", comboScore);
                    DrawText(comboText, 64+5, VIRTUAL_SCREEN_HEIGHT + 3, 8, uiColorRed);
                }
            EndMode2D();
        EndTextureMode();

        // Draw to Screen
        BeginDrawing();
            ClearBackground(BLACK);

            // Draw world camera to screen
            DrawTexturePro(target.texture, sourceRec, destRec, origin, 0.0f, WHITE);

            // DrawFPS(10, 10);
        EndDrawing();
        //-----------------------------------------------------

        // Toggle Fullscreen
        //-----------------------------------------------------
        // check for alt + enter
        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
        {
            ToggleFullscreenWindow();

            int windowHeight = GetScreenHeight();
            float aspectRatio = (float)DEFAULT_SCREEN_WIDTH / (float)DEFAULT_SCREEN_HEIGHT;
            int newWidth = (int)(windowHeight * aspectRatio); // Calculate the new width based on the aspect ratio
            printf("New Width: %9.6f\n", aspectRatio);

            // Calculate position to center the texture on the screen
            int posX = (GetScreenWidth() - newWidth) / 2;
            int posY = 0; // Since we want the height to match the window's height, y-position is 0

            destRec = (Rectangle){
                    (float)posX,
                    (float)posY,
                (float)newWidth,
                (float)GetScreenHeight()};
        }
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}
