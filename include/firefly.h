#ifndef KATANA_KLEAVER_FIREFLY_H
#define KATANA_KLEAVER_FIREFLY_H

#include "raylib.h"

#define FIREFLY_COUNT (40)
#define FIREFLY_WAVE_LAYERS (3)

extern const double fireflyColorSpeed;

extern const float fireflyMinSpeed;
extern const float fireflyMaxSpeed;
extern const float fireflyMinLength;
extern const float fireflyMaxLength;

typedef struct
{
    Vector2 position;
    float phase;
    Vector2 speeds[FIREFLY_WAVE_LAYERS];
    Vector2 lengths[FIREFLY_WAVE_LAYERS];
} Firefly;

extern Firefly fireflies[FIREFLY_COUNT];

void FirefliesInit();
void FirefliesDraw();

#endif //KATANA_KLEAVER_FIREFLY_H
