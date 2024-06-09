#ifndef BALL_GAME_PARTICLE_H
#define BALL_GAME_PARTICLE_H

#include "raylib.h"
#include "linked_list.h"

extern ListNode *particleHead;
extern ListNode *particleRedrawHead;

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

void ParticleCreate(Particle *particle);
void ParticleUpdate(Particle *particle);
void ParticleDraw(Particle particle);
void ParticleDrawAlpha(Particle particle, int alpha);
void ParticlesUpdate();

// Particle Presets
Particle *ParticlePresetJuice(Vector2 position, Color juiceColor);
Particle *ParticlePresetPlayerBlood(Vector2 position);

#endif //BALL_GAME_PARTICLE_H
