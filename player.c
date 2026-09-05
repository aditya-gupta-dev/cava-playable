#define MINIAUDIO_IMPLEMENTATION 

#include "miniaudio.h"
#include "player.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *music_player_callback(void *arg) {
  ma_result result; 
  MusicPlayer *player = (MusicPlayer*) arg;

  result = ma_engine_init(NULL, &player->engine); 
  if (result != MA_SUCCESS) { 
    return NULL; 
  }

  result = ma_engine_play_sound(&player->engine, player->song_file_name, NULL);
  if (result != MA_SUCCESS) { 
    ma_engine_uninit(&player->engine); 
    return NULL; 
  }

  return NULL; 
}

void init_music_player(const char *song_file_name) {
  MusicPlayer *player = (MusicPlayer*) malloc(sizeof(MusicPlayer));
  player->song_file_name = strdup(song_file_name);
  pthread_create(&player->thread_id, NULL, music_player_callback, (void*)player); 
} 
