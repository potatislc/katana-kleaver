#ifndef BALL_GAME_PARTICLE_H
#define BALL_GAME_PARTICLE_H

#include "raylib.h"
#include "linked_list.h"

extern ListNode *particleHead;

typedef struct Particle
{
    double initTime;
    double lifeTime;
    Vector2 position;
    Vector2 velocity;
    float gravity;
    float drag;
    Texture2D texture;
    Color colorTint;
    float scaleAnim;
} Particle;

void ParticleCreate(Particle *particle);
void ParticleUpdate(Particle *particle);
void ParticleDraw(Particle particle);
void ParticlesUpdate();

// Particle Presets
Particle *ParticlePresetRedJuice(Vector2 position);

#endif //BALL_GAME_PARTICLE_H
