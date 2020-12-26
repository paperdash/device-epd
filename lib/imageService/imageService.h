#ifndef IMAGE_SERVICE_H
#define IMAGE_SERVICE_H

void imageServiceRenderFile(const char imageFile[], int x, int y, int w, int h, bool dithering);
bool imageServiceUpdateFile(const char format[], const char target[]);

#endif