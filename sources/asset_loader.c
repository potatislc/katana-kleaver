#include "asset_loader.h"

GameTextures gameTextures;
GameAudio gameAudio;

void LoadGameTextures()
{
    gameTextures.titleText = LoadTexture("../assets/textures/title_test.png");
    gameTextures.bgFloorStandard = LoadTexture("../assets/textures/floor.png");
    gameTextures.bgBallClear = LoadTexture("../assets/textures/bg_ball_clear.png");
    gameTextures.tvBorder = LoadTexture("../assets/textures/tv_border.png");
    gameTextures.melonBig = LoadTexture("../assets/textures/melon/melon_big.png");
    gameTextures.melonSmall = LoadTexture("../assets/textures/melon/melon_small.png");
    gameTextures.orange = LoadTexture("../assets/textures/melon/orange.png");
    gameTextures.orangeHalo = LoadTexture("../assets/textures/melon/orange_halo.png");
    gameTextures.melonShadow = LoadTexture("../assets/textures/melon/melon_shadow_big.png");
    gameTextures.samurai = LoadTexture("../assets/textures/samurai/samurai.png");
    gameTextures.samuraiRunSheet = LoadTexture("../assets/textures/samurai/samurai_run.png");
    gameTextures.samuraiShadow = LoadTexture("../assets/textures/samurai/samurai_shadow.png");
    gameTextures.samuraiDieSheet = LoadTexture("../assets/textures/samurai/samurai_die.png");
    gameTextures.particleRound = LoadTexture("../assets/textures/particle/particle_round.png");
    gameTextures.settingsIcon = LoadTexture("../assets/textures/settings_icon.png");
    gameTextures.arrowIcon = LoadTexture("../assets/textures/arrow_icon.png");
    gameTextures.playIcon = LoadTexture("../assets/textures/play_icon.png");
}

void MixGameAudio()
{
    SetSoundVolume(gameAudio.swordSlash, .5f);
    SetSoundVolume(gameAudio.dash, 1.2f);
    SetSoundVolume(gameAudio.melonSplats[0], 1.f);
    SetSoundVolume(gameAudio.melonSplats[1], 1.f);
    SetSoundVolume(gameAudio.melonSplats[2], 1.f);
    SetSoundVolume(gameAudio.deathScream, 1.f);
    SetSoundVolume(gameAudio.footstep, .3f);
    SetSoundVolume(gameAudio.ballSpawn, .6f);
    SetSoundVolume(gameAudio.orangeSpawn, .6f);
    SetSoundVolume(gameAudio.postBallClarity, .3f);
    SetSoundVolume(gameAudio.bonusScore, .7f);
    SetSoundVolume(gameAudio.loseCombo, .1f);
    SetSoundVolume(gameAudio.wipes[0], .5f);
    SetSoundVolume(gameAudio.wipes[1], .5f);


    SetSoundPitch(gameAudio.ballSpawn, 1.2f);
    SetSoundPitch(gameAudio.orangeSpawn, 1.05f);
    SetSoundPitch(gameAudio.postBallClarity, 1.05f);

    SetMusicVolume(gameAudio.mainTheme, .4f);
}

void LoadGameAudio()
{
    gameAudio.swordSlash = LoadSound("../assets/sfx/sword_slash.wav");
    gameAudio.dash = LoadSound("../assets/sfx/dash.wav");
    gameAudio.melonSplats[0] = LoadSound("../assets/sfx/melon_splat_1.wav");
    gameAudio.melonSplats[1] = LoadSound("../assets/sfx/melon_splat_2.wav");
    gameAudio.melonSplats[2] = LoadSound("../assets/sfx/melon_splat_3.wav");
    gameAudio.deathScream = LoadSound("../assets/sfx/death_scream.wav");
    gameAudio.footstep = LoadSound("../assets/sfx/samurai_footstep.wav");
    gameAudio.ballSpawn = LoadSound("../assets/sfx/ball_spawn.wav");
    gameAudio.orangeSpawn = LoadSound("../assets/sfx/orange_spawn.wav");
    gameAudio.postBallClarity = LoadSound("../assets/sfx/post_ball_clarity.wav");
    gameAudio.bonusScore = LoadSound("../assets/sfx/bonus_score.wav");
    gameAudio.loseCombo = LoadSound("../assets/sfx/lose_combo.wav");
    gameAudio.wipes[0] = LoadSound("../assets/sfx/wipe_1.wav");
    gameAudio.wipes[1] = LoadSound("../assets/sfx/wipe_2.wav");

    gameAudio.mainTheme = LoadMusicStream("../assets/bgm/ball_music.mp3");

    MixGameAudio();
}