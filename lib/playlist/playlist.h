#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <WString.h>

void setupPlaylist();
void loopPlaylist();

void playlistNextFace();
String playlistGetCurrentFace();
int32_t playlistGetRemainingTimeMs();
void playlistResetTimer();
void playlistShow(const char name[]);
void playlistNextSwitchIn(int32_t seconds);

#endif