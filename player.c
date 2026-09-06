#define MINIAUDIO_IMPLEMENTATION

#include "miniaudio.h"
#include "player.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

static struct termios orig_termios;

void disable_raw_mode(void) { tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios); }

void enable_raw_mode(void) {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

int kbhit(void) {
  struct timeval tv = {0L, 0L};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

void *music_player_callback(void *arg) {
  int is_playing = 1;
  ma_result result;
  MusicPlayer *player = (MusicPlayer *)arg;

  result = ma_engine_init(NULL, &player->engine);
  if (result != MA_SUCCESS) {
    return NULL;
  }

  result = ma_sound_init_from_file(&player->engine, player->song_file_name, 0,
                                   NULL, NULL, &player->sound);
  if (result != MA_SUCCESS) {
    return NULL;
  }

  ma_sound_start(&player->sound);
  enable_raw_mode();
  struct timespec sleep_dur = {0, 1000000000 / 1000};
  while (1) {
    if (kbhit()) {
      char c;
      if (read(STDIN_FILENO, &c, 1) > 0) {
        if (c == ' ') {
          if (is_playing == 1) {
            ma_sound_stop(&player->sound);
            is_playing = 0;
          } else {
            ma_sound_start(&player->sound);
            is_playing = 1;
          }
        }
      }
    }
    nanosleep(&sleep_dur, NULL); 
  }

  return NULL;
}

void init_music_player(const char *song_file_name) {
  MusicPlayer *player = (MusicPlayer *)malloc(sizeof(MusicPlayer));
  player->song_file_name = strdup(song_file_name);
  pthread_create(&player->thread_id, NULL, music_player_callback,
                 (void *)player);
}
