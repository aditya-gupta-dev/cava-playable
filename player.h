#include <pthread.h>
#include "miniaudio.h"

typedef struct { 
  char *song_file_name;
  pthread_t thread_id;
  ma_engine engine; 
  ma_result result; 
} MusicPlayer;

void init_music_player(const char *song_file_name); 
