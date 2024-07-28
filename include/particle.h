#ifndef BALL_GAME_PARTICLE_H
#define BALL_GAME_PARTICLE_H

#include "raylib.h"
#include "linked_list.h"

#define CREATE_PARTICLES(listHead, type, amount) \
do                                     \
{                                      \
    for (int i = 0; i < amount; i++)  \
    {                                  \
        ParticleCreate(listHead, type);          \
    }                                  \
} while (0)                            \

extern ListNode *particleHead;
extern ListNode *particleUiHead;
extern ListNode *particleFadeHead;

typedef struct Particle
{
    double initTime;
    double lifeTime;
    Vector2 position;
    Vector2 velocity;
    float gravity;
    float drag;
    Texture2D texture;
    Vector2 textureOffset;
    Color colorTint;
    float scaleAnim;
} Particle;

void ParticleCreate(ListNode **listHeadRef, Particle *particle);
void ParticleUpdate(ListNode **listHeadRef, Particle *particle);
void ParticleDraw(Particle particle);
void ParticleDrawFade(Particle particle);
void ParticlesUpdate();

// Particle Presets
Particle *ParticlePresetJuice(Vector2 position, Color juiceColor);
Particle *ParticlePresetPlayerBlood(Vector2 position);
Particle *ParticlePresetAura(Vector2 position, Color color);
Particle *ParticlePresetMiss(Vector2 position);

// Preset colors
extern const Color auraMelon;
extern const Color auraOrange;

#endif //BALL_GAME_PARTICLE_H
