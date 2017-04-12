#include "main.h"
#include "utils.h"

extern unsigned char _binary_res_lives_png_start;
extern unsigned char _binary_res_lives16p_png_start;

void exitServices() {
	vita2d_free_texture(_lives16p);
	vita2d_free_texture(_lives);
	vita2d_free_pvf(font);

	vita2d_fini();
	sceKernelExitProcess(0);
}

void initVars() {
	lives = 5;
	_time = 960;
	diff = 10;
	stages = 0;	
}

void updatediff() {
	diff = 10;
	/*switch (stages) {
		case 10: diff = 9; break;
		case 30: diff = 8; break;
		case 50: diff = 7; break;
		case 70: diff = 6; break;
		case 100: diff = 5; break;
		case 150: diff = 4; break;
		case 200: diff = 3; break;
	}*/
}

void initServices() {
	srand(time(NULL));
	
	vita2d_init();
	vita2d_set_clear_color(RGBA8(255, 255, 255, 255));
	
	/*sf2d_start_frame(GFX_TOP, GFX_LEFT);
	sf2d_end_frame();
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
	sf2d_end_frame();
	sf2d_swapbuffers();*/
	
	_lives = loadPngWithFilter(&_binary_res_lives_png_start);
	_lives16p = loadPngWithFilter(&_binary_res_lives16p_png_start);
	font = vita2d_load_default_pvf();
	//vita2d_pvf_draw_text(font, 0, 0, RGBA8(0, 0, 0, 0), 4.0f, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	
	initVars();
}

void fillPattern() {
	for (unsigned int i = 0; i < 16; i++)
		pattern[i] = false;
	
	pattern[rand() % 16] = true;
	
	color[0] = rand() % (255 - 2*diff) + diff;
	color[1] = rand() % (255 - 2*diff) + diff;
	color[2] = rand() % (255 - 2*diff) + diff;
}

void endgame() {
	char tmp[26], tmp2[20], tmp3[20];
	snprintf(tmp, 24, "Levels completed: %d!", stages);
	snprintf(tmp2, 20, "Best score: %d", maxStages);
	snprintf(tmp3, 20, "Games played: %d", matches);
	
	while(1) {
		controls();
		
		if (pressed & SCE_CTRL_CROSS)
			return;
		
		vita2d_start_drawing();
		vita2d_clear_screen();
			
		vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 4.0f, "End game")) / 2, 80, RGBA8(0,0,0,255), 4.0f, "End game!");
		vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, tmp)) / 2, 136, RGBA8(0,0,0,255), 1.8f, tmp); //+30
		vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.2f, "Press X to restart")) / 2, 260, RGBA8(200,0,0,200), 1.2f, "Press X to restart");
	
		vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, tmp3)) / 2, 176, RGBA8(0,0,0,200), 1.8f, tmp2);
		vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, tmp3)) / 2, 216, RGBA8(0,0,0,200), 1.8f, tmp3); //+24
		
		endDrawing();
	}
}

void menu_start() {
	char *instr = "Try to pick the different color square!";
	
	vita2d_start_drawing();
	vita2d_clear_screen();

	vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 4.0f, "PICKR")) / 2, 80, RGBA8(0,0,0,255), 4.0f, "PICKR");
	vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, "Press X to start a new game")) / 2, 135, RGBA8(0,0,0,200), 1.8f, "Press X to start a new game");
	vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.2f, "Press O to exit")) / 2, 170, RGBA8(0,0,0,200), 1.2f, "Press O to exit");
	vita2d_pvf_draw_text(font, 948 - vita2d_pvf_text_width(font, 1.2f, ver), 530, RGBA8(0,0,0,200), 1.2f, ver);

	vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.2f, instr)) / 2, 220, RGBA8(200,0,0,200), 1.2f, instr);
	vita2d_pvf_draw_text(font, 5, 507, RGBA8(0,0,0,150), 1.2f, "Based on Sean M. Tracey's idea");
	vita2d_pvf_draw_text(font, 5, 530, RGBA8(0,0,0,180), 1.2f, "Made with       by Bernardo Giordano, ported by Joel16");
	vita2d_draw_texture(_lives16p, 125, 515);
	
	endDrawing();
}

/*void level() {
	int x, y;
	bool win = false;
	char stagestr[10];
	snprintf(stagestr, 10, "Level %d", stages);
	
	_time = 960;
	fillPattern();
	diff = (rand() % 2) ? diff : -diff;
	
	while(1 && _time > 0 && lives > 0) {
		controls();
		touchPosition touch;
		hidTouchRead(&touch);
		
		if (hidKeysDown() & KEY_TOUCH) {
			int x_start, y_start = 21;
			for (unsigned int i = 0; i < 4; i++) {
				x_start = 65;
				for (unsigned int j = 0; j < 4; j++) {
					if ((touch.px > x_start) && (touch.px < (x_start + 45)) && (touch.py > y_start) && (touch.py < (y_start + 45))) {
						if (pattern[i*4+j])
							win = true;
						else
							lives--;
					}
					x_start += 48;
				}
				y_start += 48;
			}
		}
		
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			for (unsigned int i = 0, max = (lives < 5) ? 5 : lives; i < max; i++) {
				if (i < lives)
					vita2d_draw_texture(_lives, 3 + i*27, 215);
				else
					vita2d_draw_texture_blend(_lives, 3 + i*27, 215, RGBA8(50,50,50,200));
			}
			vita2d_pvf_draw_text(font, 396 - vita2d_pvf_text_width(font, 1.8f, stagestr), 21.8f, RGBA8(200,0,0,200), 1.8f, stagestr);
		sf2d_end_frame();
		
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			y = 21;
			for (unsigned int i = 0; i < 4; i++) {
				x = 65;
				for (unsigned int j = 0; j < 4; j++) {
					sf2d_draw_rectangle(x, y, 45, 45, (pattern[i*4+j]) ? RGBA8(color[0] - diff, color[1] - diff, color[2] - diff, 255) : RGBA8(color[0], color[1], color[2], 255));
					x += 48;
				}
				y += 48;
			}
			sf2d_draw_rectangle(0, 230, _time, 10, (_time > 100) ? RGBA8(0,255,0,255) : RGBA8(255,0,0,255));
		sf2d_end_frame();
		sf2d_swapbuffers();

		if (--_time == 0 && lives > 0) {
			_time = 960;
			lives--;
		}
		
		if (win) {
			if (++stages % 10 == 0 && lives < 10)
				lives++;
			updatediff();
			maxStages = (stages >= maxStages) ? stages : maxStages;
			level();
		}
	}
}*/

int main() {
	maxStages = 0;
	matches = 0;
	initServices();
	
	while(1) {
		controls();
		
		if (pressed & SCE_CTRL_CIRCLE)
			break;
		
		if (pressed & SCE_CTRL_CROSS) {
			//level();
			matches++;
			endgame();
			initVars();
		}
		
		menu_start();
	}
	
	exitServices();
	return 0;
}