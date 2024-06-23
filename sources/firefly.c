#include <stdlib.h>
#include <math.h>
#include "firefly.h"
#include "renderer.h"
#include "global.h"
#include "raymath.h"

const double fireflyColorSpeed = 2;
const float fireflyMinSpeed = .1f;
const float fireflyMaxSpeed = .2f;
const float fireflyMinLength = 16;
const float fireflyMaxLength = 96;

Firefly fireflies[FIREFLY_COUNT];

#define RAND_0_TO_1 ((float)rand() / RAND_MAX)

void FirefliesInit()
{
    for (int i = 0; i < FIREFLY_COUNT; i++)
    {
        fireflies[i].position = (Vector2){RAND_0_TO_1 * VIRTUAL_SCREEN_WIDTH, RAND_0_TO_1 * VIRTUAL_SCREEN_HEIGHT };
        fireflies[i].phase = RAND_0_TO_1 * 20;

        for (int j = 0; j < FIREFLY_WAVE_LAYERS; j++)
        {
            float layer = (float)j+1;

            fireflies[i].lengths[j] = (Vector2){
                (fireflyMinLength + RAND_0_TO_1*fireflyMaxLength)/layer,
                (fireflyMinLength + RAND_0_TO_1*fireflyMaxLength)/layer };
            fireflies[i].speeds[j] = (Vector2){
                (fireflyMinSpeed + RAND_0_TO_1*fireflyMaxSpeed)*layer,
                (fireflyMinSpeed + RAND_0_TO_1*fireflyMaxSpeed)*layer };
        }
    }
}

void FirefliesDraw()
{
    for (int i = 0; i < FIREFLY_COUNT; i++)
    {
        Color color = uiColorYellow;
        color.a = fmax(0, sin(GetTime() * fireflyColorSpeed + fireflies[i].phase) * 200);

        Vector2 resPosition = fireflies[i].position;
        for (int j = 0; j < FIREFLY_WAVE_LAYERS; j++)
        {
            Vector2 layerVec = (Vector2){cosf((float)GetTime() * fireflies[i].speeds[j].x) * fireflies[i].lengths[j].x,
                                         sinf((float)GetTime() * fireflies[i].speeds[j].y) * fireflies[i].lengths[j].y
                                         };
            resPosition = Vector2Add(resPosition, layerVec);
        }

        DrawCircleV(resPosition, 1, color);
    }
}
