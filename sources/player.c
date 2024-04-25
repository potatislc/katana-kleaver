#include <player.h>

void drawPlayer(struct Player player)
{
    DrawTextureV(player.texture, player.position, WHITE);
}