#include "particle.h"
#include "raymath.h"
#include "linked_list.h"
#include "asset_loader.h"
#include "global.h"

ListNode *particleHead = NULL;
ListNode *particleRedrawHead = NULL;

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
    Vector2 scaledOffset = (Vector2){particle.textureOffset.x * scale, particle.textureOffset.y * scale};
    Vector2 texturePos = Vector2Round(Vector2Subtract(particle.position, scaledOffset));
    DrawTextureEx(particle.texture, texturePos, 0.f, scale, particle.colorTint);
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
    particle->velocity = LengthDirToVector2(2.f, (float)(((double)rand() / RAND_MAX) * 2 * M_PI));
    particle->gravity = 0.f;
    particle->drag = .1f;
    particle->texture = gameTextures.particleRound;
    particle->textureOffset = (Vector2){(float)particle->texture.width / 2.f, (float)particle->texture.height / 2.f};
    particle->colorTint = uiColorRed;
    particle->scaleAnim = 1.f;

    return particle;
}