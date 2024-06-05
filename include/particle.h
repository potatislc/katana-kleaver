#ifndef BALL_GAME_PARTICLE_H
#define BALL_GAME_PARTICLE_H

#include "raylib.h"
#include "linked_list.h"

ListNode *particleHead;

typedef struct Particle
{
    double deathTime; // InitTime+LifeTime
    Vector2 position;
    Vector2 velocity;
    float gravity;
    float drag;
    Texture2D texture;
    Color colorTint;
    float scaleAnim;
} Particle;

Particle *ParticleInit(double lifeTime );
void ParticleUpdate(Particle *particle);
void ParticleDraw(Particle particle);

// Particle Presets
Particle *InitParticleBlood(Vector2 position);

#endif //BALL_GAME_PARTICLE_H
