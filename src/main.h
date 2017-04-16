#include <vitasdk.h>

#include <vita2d.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vita2d_texture *_lives, *_lives16p, *wifi;
vita2d_pvf *font;

char* ver = "v1.1";
int _time, diff, stages, maxStages, lives, matches, inRow;
bool pattern[16];
unsigned int color[3];