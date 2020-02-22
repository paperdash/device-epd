#ifndef IMAGE_PNG_H
#define IMAGE_PNG_H

// @see http://www.libpng.org/pub/png/spec/1.2/PNG-Structure.html
const char ImageHeaderPNG[] = "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A";

void setupImagePNG();
void pngOpenFramebuffer();
void pngWriteFramebuffer(int offset, uint8_t bitmap[], int c);
void pngFlushFramebuffer();

#endif