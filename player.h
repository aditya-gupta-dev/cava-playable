#include "miniaudio.h"
#include <pthread.h>

typedef struct {
    char *song_file_name;
    pthread_t thread_id;
    ma_engine engine;
    ma_sound sound;
} MusicPlayer;

void init_music_player(const char *song_file_name);
