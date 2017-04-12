#include <vitasdk.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vita2d.h>

SceCtrlData pad, old_pad;
unsigned int pressed;

int controls();
vita2d_texture * loadPngWithFilter(const void *buffer);
void endDrawing();