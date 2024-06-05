#include "particle.h"
#include "raymath.h"
#include "linked_list.h"

void ParticleUpdate(Particle *particle)
{
    if (GetTime() >= particle->deathTime)
    {
        ListNodeRemove(&particleHead, particle);
        return;
    }

    particle->velocity = Vector2MoveTowards(particle->velocity, Vector2Zero(), particle->drag);
    particle->position = Vector2Add(particle->position, particle->velocity);
}

void ParticleDraw(Particle particle)
{
    float scale = (float)(1 - GetTime() / particle.deathTime);
    DrawTextureEx(particle.texture, particle.position, 0.f, scale, particle.colorTint);
}