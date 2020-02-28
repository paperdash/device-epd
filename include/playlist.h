#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <WString.h>

void setupPlaylist();
void loopPlaylist();

void PlaylistNextFace();
String PlaylistGetCurrentFace();
int32_t PlaylistGetRemainingTimeMs();
void PlaylistResetTimer();

#endif