#include "particle.h"
#include "raymath.h"
#include "linked_list.h"
#include "asset_loader.h"
#include "global.h"

ListNode *particleHead = NULL;

void ParticleCreate(Particle *particle)
{
    ListNodePush(&particleHead, particle);
}

void ParticleUpdate(Particle *particle)
{
    if (GetTime() >= particle->initTime + particle->lifeTime)
    {
        ListNodeRemove(&particleHead, particle);
        return;
    }

    particle->velocity = Vector2MoveTowards(particle->velocity, Vector2Zero(), particle->drag);
    particle->velocity = (Vector2){particle->velocity.x, particle->velocity.y + particle->gravity};
    particle->position = Vector2Add(particle->position, particle->velocity);
}

void ParticleDraw(Particle particle)
{
    float scale = (float)(1 - (GetTime() - particle.initTime) / particle.lifeTime);
    DrawTextureEx(particle.texture, Vector2Round(particle.position), 0.f, scale, particle.colorTint);
}

void ParticlesUpdate()
{
    ListNode* currentParticleNode = particleHead;
    while (currentParticleNode != NULL)
    {
        ParticleUpdate(currentParticleNode->data);
        currentParticleNode = currentParticleNode->next;
    }
}

// Preset Particles
Particle *ParticlePresetRedJuice(Vector2 position)
{
    Particle* particle = (Particle*)malloc(sizeof(Particle));

    particle->initTime = GetTime();
    particle->lifeTime = .5;
    particle->position = position;
    particle->velocity = Vector2Zero();
    particle->gravity = 0.f;
    particle->drag = .1f;
    particle->texture = gameTextures.particleRound;
    particle->colorTint = uiColorRed;
    particle->scaleAnim = 1.f;

    return particle;
}