#include "asset_loader.h"

GameTextures gameTextures;
GameAudio gameAudio;

void LoadGameTextures()
{
    gameTextures.titleText = LoadTexture(ASSETS_PATH "textures/title_test_2.png");
    gameTextures.bgFloorStandard = LoadTexture(ASSETS_PATH "textures/floor.png");
    gameTextures.bgBallClear = LoadTexture(ASSETS_PATH "textures/bg_ball_clear.png");
    gameTextures.tvBorder = LoadTexture(ASSETS_PATH "textures/tv_border.png");
    gameTextures.melonBig = LoadTexture(ASSETS_PATH "textures/melon/melon_big.png");
    gameTextures.melonSmall = LoadTexture(ASSETS_PATH "textures/melon/melon_small.png");
    gameTextures.orange = LoadTexture(ASSETS_PATH "textures/melon/orange.png");
    gameTextures.orangeHalo = LoadTexture(ASSETS_PATH "textures/melon/orange_halo.png");
    gameTextures.melonShadow = LoadTexture(ASSETS_PATH "textures/melon/melon_shadow_big.png");
    gameTextures.samurai = LoadTexture(ASSETS_PATH "textures/samurai/samurai.png");
    gameTextures.samuraiRunSheet = LoadTexture(ASSETS_PATH "textures/samurai/samurai_run.png");
    gameTextures.samuraiShadow = LoadTexture(ASSETS_PATH "textures/samurai/samurai_shadow.png");
    gameTextures.samuraiDieSheet = LoadTexture(ASSETS_PATH "textures/samurai/samurai_die.png");
    gameTextures.particleRound = LoadTexture(ASSETS_PATH "textures/particle/particle_round.png");
    gameTextures.settingsIcon = LoadTexture(ASSETS_PATH "textures/icons/settings_icon.png");
    gameTextures.arrowIcon = LoadTexture(ASSETS_PATH "textures/icons/arrow_icon.png");
    gameTextures.playIcon = LoadTexture(ASSETS_PATH "textures/icons/play_icon.png");
    gameTextures.windowIcon = LoadTexture(ASSETS_PATH "textures/icons/window_icon.png");
    gameTextures.bgmIcon = LoadTexture(ASSETS_PATH "textures/icons/bgm_icon.png");
    gameTextures.infoQuit = LoadTexture(ASSETS_PATH "textures/info_quit.png");

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
    SetSoundVolume(gameAudio.gainCombo, .5f);
    SetSoundVolume(gameAudio.gainMultiply, .5f);
    SetSoundVolume(gameAudio.tutorialClear, .2f);
    SetSoundVolume(gameAudio.wipes[0], .5f);
    SetSoundVolume(gameAudio.wipes[1], .5f);


    SetSoundPitch(gameAudio.ballSpawn, 1.2f);
    SetSoundPitch(gameAudio.orangeSpawn, 1.05f);
    SetSoundPitch(gameAudio.postBallClarity, 1.05f);

    SetMusicVolume(gameAudio.mainTheme, DEFAULT_BGM_VOL);
}

void LoadGameAudio()
{
    gameAudio.swordSlash = LoadSound(ASSETS_PATH "sfx/sword_slash.wav");
    gameAudio.dash = LoadSound(ASSETS_PATH "sfx/dash.wav");
    gameAudio.melonSplats[0] = LoadSound(ASSETS_PATH "sfx/melon_splat_1.wav");
    gameAudio.melonSplats[1] = LoadSound(ASSETS_PATH "sfx/melon_splat_2.wav");
    gameAudio.melonSplats[2] = LoadSound(ASSETS_PATH "sfx/melon_splat_3.wav");
    gameAudio.deathScream = LoadSound(ASSETS_PATH "sfx/death_scream.wav");
    gameAudio.footstep = LoadSound(ASSETS_PATH "sfx/samurai_footstep.wav");
    gameAudio.ballSpawn = LoadSound(ASSETS_PATH "sfx/ball_spawn.wav");
    gameAudio.orangeSpawn = LoadSound(ASSETS_PATH "sfx/orange_spawn.wav");
    gameAudio.postBallClarity = LoadSound(ASSETS_PATH "sfx/post_ball_clarity.wav");
    gameAudio.bonusScore = LoadSound(ASSETS_PATH "sfx/bonus_score.wav");
    gameAudio.loseCombo = LoadSound(ASSETS_PATH "sfx/lose_combo.wav");
    gameAudio.gainCombo = LoadSound(ASSETS_PATH "sfx/gain_combo.wav");
    gameAudio.gainMultiply = LoadSound(ASSETS_PATH "sfx/gain_multiply.wav");
    gameAudio.tutorialClear = LoadSound(ASSETS_PATH "sfx/tutorial_clear.wav");
    gameAudio.wipes[0] = LoadSound(ASSETS_PATH "sfx/wipe_1.wav");
    gameAudio.wipes[1] = LoadSound(ASSETS_PATH "sfx/wipe_2.wav");

    gameAudio.mainTheme = LoadMusicStream(ASSETS_PATH "bgm/ball_music.mp3");

    MixGameAudio();
}

bool muted = false;

void MuteUnmuteMusic()
{
    SetMusicVolume(gameAudio.mainTheme, (!muted) ? 0.f : DEFAULT_BGM_VOL);
    muted = !muted;
}