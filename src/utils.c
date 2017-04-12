#include "utils.h"

int controls()
{
	memset(&pad, 0, sizeof(SceCtrlData));
	sceCtrlPeekBufferPositive(0, &pad, 1);

	pressed = pad.buttons & ~old_pad.buttons;
	
	old_pad = pad;
	return 0;
}

void setBilinearFilter(int enabled, vita2d_texture * texture)
{
	if (enabled == 1)
		vita2d_texture_set_filters(texture, SCE_GXM_TEXTURE_FILTER_LINEAR, SCE_GXM_TEXTURE_FILTER_LINEAR);
}

vita2d_texture * loadPngWithFilter(const void *buffer)
{
	vita2d_texture *texture = vita2d_load_PNG_buffer(buffer);
	setBilinearFilter(1, texture);
	
	return texture;
}

void endDrawing() 
{
	vita2d_end_drawing();
	vita2d_common_dialog_update();
	vita2d_swap_buffers();
	sceDisplayWaitVblankStart();
}