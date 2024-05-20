#include <stdio.h>
#include "raylib.h"
#include "ball.h"
#include "player.h"
#include "global.h"
#include "linked_list.h"
#include "asset_loader.h"
#include <string.h>


#define WINDOW_TITLE "Ball Game"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    Vector2 vScreenCenter = {VIRTUAL_SCREEN_WIDTH / 2.0f, VIRTUAL_SCREEN_HEIGHT / 2.0f };

    RenderTexture2D target = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT+VIRTUAL_SCREEN_OFFSET_Y);

    Camera2D worldSpaceCamera = { 0 };
    worldSpaceCamera.zoom = 1.0f;

    Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
    Rectangle destRec = {-screenRatio, -screenRatio, SCREEN_WIDTH + (screenRatio * 2), SCREEN_HEIGHT + (screenRatio * 2) };

    Vector2 origin = { 0.0f, 0.0f };

    LoadGameTextures();

    ListNode *ballHead = NULL;

    Player *player = (Player*) malloc(sizeof(Player));
    PlayerInit(player, vScreenCenter, &ballHead);

    // ToggleFullscreen(); -- Wtfff

    SetTargetFPS(60);
    //----------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        if (IsKeyPressed(KEY_SPACE))
        {
            float testRadius = 32.0f;
            Ball *newBall = (Ball *)malloc(sizeof(Ball));
            BallInit(newBall,
                     (Vector2) {VIRTUAL_SCREEN_WIDTH/2 - testRadius/2, VIRTUAL_SCREEN_HEIGHT/2 - testRadius/2},
                     testRadius);

            ListNodePush(&ballHead, newBall);

            int ballAmount = 0;
            ListNode* currentBall = ballHead;
            while (currentBall != NULL)
            {
                ballAmount++;
                printf("Ball data: %p", currentBall->data);
                printf("\n");
                currentBall = currentBall->next;
            }

            printf("Ball Spawned: %d\n", ballAmount);
            printf("Ball Head ptr: %p\n", &ballHead);
            printf("Ball Head from Player ptr: %p\n", player->ballHeadRef);
        }

        if (!freezeBalls)
        {
            // Updating linked list of balls
            ListNode* currentBallNode = ballHead;
            while (currentBallNode != NULL)
            {
                BallCollisionBall(currentBallNode->data, ballHead);
                BallMove(currentBallNode->data);
                BallCollisionScreen(currentBallNode->data);
                currentBallNode = currentBallNode->next;
            }
        }

        PlayerUpdate(player, ballHead);
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        BeginTextureMode(target);
            ClearBackground((Color){ 121, 121, 121, 255});

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
            ClearBackground(DARKGRAY);

            // Draw world camera to screen
            DrawTexturePro(target.texture, sourceRec, destRec, origin, 0.0f, WHITE);

            DrawFPS(10, 10);
        EndDrawing();
        //-----------------------------------------------------

        // Toggle Fullscreen
        //-----------------------------------------------------
        // check for alt + enter
        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
        {
            // see what display we are on right now
            int display = GetCurrentMonitor();

            if (IsWindowFullscreen())
            {
                // if we are full screen, then go back to the windowed size
                SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
            }
            else
            {
                // if we are not full screen, set the window size to match the monitor we are on
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
            }

            // toggle the state
            ToggleFullscreen();
        }
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}
