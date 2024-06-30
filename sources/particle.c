#include "particle.h"
#include "raymath.h"
#include "linked_list.h"
#include "asset_loader.h"
#include "global.h"

ListNode *particleHead = NULL;
ListNode *particleUiHead = NULL;

const Color auraMelon = {219, 65, 97, 100};
const Color auraOrange = {255, 180, 0, 100};

void ParticleCreate(ListNode **listHeadRef, Particle *particle)
{
    ListNodePush(listHeadRef, particle);
}

void ParticleUpdate(ListNode **listHeadRef, Particle *particle)
{
    if (GetTime() >= particle->initTime + particle->lifeTime)
    {
        ListNodeRemove(listHeadRef, particle);
        return;
    }

    particle->velocity = Vector2MoveTowards(particle->velocity, Vector2Zero(), particle->drag);
    particle->velocity.y += particle->gravity;
    particle->position = Vector2Add(particle->position, particle->velocity);
}

void ParticleDraw(Particle particle)
{
    float scale = (float)(1 - (GetTime() - particle.initTime) / particle.lifeTime);
    Vector2 scaledOffset = (Vector2){particle.textureOffset.x * scale, particle.textureOffset.y * scale};
    Vector2 texturePos = Vector2Round(Vector2Subtract(particle.position, scaledOffset));
    DrawTextureEx(particle.texture, texturePos, 0.f, scale, particle.colorTint);
}

void ParticleDrawAlpha(Particle particle, int alpha)
{
    float scale = (float)(1 - (GetTime() - particle.initTime) / particle.lifeTime);
    Vector2 scaledOffset = (Vector2){particle.textureOffset.x * scale, particle.textureOffset.y * scale};
    Vector2 texturePos = Vector2Round(Vector2Subtract(particle.position, scaledOffset));
    Color alphaColor = {particle.colorTint.r, particle.colorTint.g, particle.colorTint.b, alpha};
    DrawTextureEx(particle.texture, texturePos, 0.f, scale, alphaColor);
}

void ParticlesUpdate()
{
    ListNode* currentParticleNode = particleHead;
    while (currentParticleNode != NULL)
    {
        ParticleUpdate(&particleHead, currentParticleNode->data);
        currentParticleNode = currentParticleNode->next;
    }

    currentParticleNode = particleUiHead;
    while (currentParticleNode != NULL)
    {
        ParticleUpdate(&particleUiHead, currentParticleNode->data);
        currentParticleNode = currentParticleNode->next;
    }
}

// Preset Particles
Particle *ParticlePresetJuice(Vector2 position, Color juiceColor)
{
    Particle* particle = (Particle*)malloc(sizeof(Particle));

    particle->initTime = GetTime();
    particle->lifeTime = .4;
    particle->position = position;
    const float randLength = 2.f + (float)((double)rand() / RAND_MAX) * 1.f;
    const float randDir = (float)(((double)rand() / RAND_MAX) * 2 * M_PI);
    particle->velocity = LengthDirToVector2(randLength, randDir);
    particle->gravity = 0.f;
    particle->drag = .1f;
    particle->texture = gameTextures.particleRound;
    particle->textureOffset = (Vector2){(float)particle->texture.width / 2.f, (float)particle->texture.height / 2.f};
    particle->colorTint = juiceColor;
    particle->scaleAnim = 1.f;

    return particle;
}

Particle *ParticlePresetPlayerBlood(Vector2 position)
{
    Particle *particle = ParticlePresetJuice(position, playerBloodColor);
    particle->lifeTime = 1;

    return particle;
}

Particle *ParticlePresetAura(Vector2 position, Color color)
{
    Particle* particle = (Particle*)malloc(sizeof(Particle));

    particle->initTime = GetTime();
    particle->lifeTime = .4;
    const float randLength = 2.f + (float)((double)rand() / RAND_MAX) * 8.f;
    const float randDir = (float)(((double)rand() / RAND_MAX) * 2 * M_PI);
    particle->position = Vector2Add(position, LengthDirToVector2(randLength, randDir));
    particle->gravity = -.05f;
    particle->velocity = Vector2Zero();
    particle->drag = 0.f;
    particle->texture = gameTextures.particlePixel;
    particle->textureOffset = (Vector2){(float)particle->texture.width / 2.f, (float)particle->texture.height / 2.f};
    particle->colorTint = color;
    particle->scaleAnim = 1.f;

    return particle;
}