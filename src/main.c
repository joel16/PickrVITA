#include "fs.h"
#include "main.h"
#include "touch.h"
#include "utils.h"

#include "include/vitaAudio.h"

vitaAudio game_over, nope;

extern unsigned char _binary_res_lives_png_start;
extern unsigned char _binary_res_lives16p_png_start;

void exitServices() 
{
	vita2d_free_texture(_lives16p);
	vita2d_free_texture(_lives);
	vita2d_free_pvf(font);
	vita2d_fini();
	vitaAudioFree(&game_over);
	vitaAudioFree(&nope);
	sceKernelExitProcess(0);
}

void initVars() 
{
	lives = 5;
	_time = 960;
	diff = 10;
	stages = 0;	
	inRow = 0;
}

void updateDiff() 
{
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

void initServices() 
{
	srand(time(NULL));
	
	vita2d_init();
	vita2d_set_clear_color(RGBA8(245, 245, 245, 255));
	
	_lives = loadPngWithFilter(&_binary_res_lives_png_start);
	_lives16p = loadPngWithFilter(&_binary_res_lives16p_png_start);
	font = vita2d_load_default_pvf();
	
	initVars();
	initTouch();
}

void fillPattern() 
{
	for (unsigned int i = 0; i < 16; i++)
		pattern[i] = false;
	
	pattern[rand() % 16] = true;
	
	color[0] = rand() % (255 - 2*diff) + diff;
	color[1] = rand() % (255 - 2*diff) + diff;
	color[2] = rand() % (255 - 2*diff) + diff;
}

void saveHighScore()
{
	char buf[10], buf2[10];
	int tempScore = 0;
	
	if (!(dirExists("ux0:/data/PickrVITA")))
	{
		SceUID dir = sceIoDopen("ux0:/data/PickrVITA");
		sceIoMkdir("ux0:/data/PickrVITA", 0777);
		sceIoDclose(dir);
		
		if (!(fileExists("ux0:/data/PickrVITA/highscore.txt")))
		{
			sprintf(buf, "%d", maxStages);
			writeFile("ux0:/data/PickrVITA/highscore.txt", buf, 10);
		}
	}
	
	readFile("ux0:/data/PickrVITA/highscore.txt", buf2, 10);
	sscanf(buf2, "%d", &tempScore);
	
	if (maxStages > tempScore)
	{
		sprintf(buf, "%d", maxStages);
		writeFile("ux0:/data/PickrVITA/highscore.txt", buf, 10);
	}
}

int getHighScore()
{
	char buf[10];
	int score;
	
	if (fileExists("ux0:/data/PickrVITA/highscore.txt"))
	{
		readFile("ux0:/data/PickrVITA/highscore.txt", buf, 5);
		sscanf(buf, "%d", &score);
	}
	
	return score;
}

void gameOver() 
{
	char tmp[26], tmp2[20], tmp3[20];
	saveHighScore();
	snprintf(tmp, 24, "You scored %d!", stages);
	snprintf(tmp2, 20, "Best score: %d", getHighScore());
	snprintf(tmp3, 20, "Games played: %d", matches);
	
	vitaAudioPlayWavSound(&game_over, "app0:files/game_over.wav");
	
	while(1) 
	{
		controls();
		
		if (pressed & SCE_CTRL_CROSS)
			return;
		if (pressed & SCE_CTRL_CIRCLE)
			exitServices();
		
		vita2d_start_drawing();
		vita2d_clear_screen();
			
		vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 4.0f, "GAME OVER")) / 2, 80, RGBA8(200, 0, 0, 255), 4.0f, "GAME OVER");
		vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, tmp)) / 2, 136, RGBA8(128, 128, 128, 255), 1.8f, tmp);
		vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, tmp2)) / 2, 176, RGBA8(128, 128, 128, 255), 1.8f, tmp2);
		vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, tmp3)) / 2, 216, RGBA8(128, 128, 128, 255), 1.8f, tmp3);
		vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.2f, "Press X to try again")) / 2, 260, RGBA8(128, 128, 128, 255), 1.2f, "Press X to try again");
		vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.2f, "Press O to exit")) / 2, 295, RGBA8(128, 128, 128, 255), 1.2f, "Press O to exit");
		
		endDrawing();
	}
}

void mainMenu() 
{
	char *instr = "One of these colours is not like the other one!";
	char *instr_2 = "Which one?";
	
	vita2d_start_drawing();
	vita2d_clear_screen();

	vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 4.0f, "Pickr")) / 2, 80, RGBA8(0, 0, 0, 200), 4.0f, "Pickr");
	
	vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.2f, instr)) / 2, 130, RGBA8(128, 128, 128, 255), 1.2f, instr);
	vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.2f, instr_2)) / 2, 165, RGBA8(128, 128, 128, 255), 1.2f, instr_2);
	
	vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, "Press X to start new game (with timer)")) / 2, 220, RGBA8(128, 128, 128, 255), 1.8f, "Press X to start new game (with timer)");
	vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, "Press [ ] to start new game (without timer)")) / 2, 270, RGBA8(128, 128, 128, 255), 1.8f, "Press [ ] to start new game (without timer)");
	vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.2f, "Press O to exit")) / 2, 300, RGBA8(128, 128, 128, 255), 1.2f, "Press O to exit");
	vita2d_pvf_draw_text(font, 948 - vita2d_pvf_text_width(font, 1.2f, ver), 530, RGBA8(0, 0, 0, 255), 1.2f, ver);

	
	vita2d_pvf_draw_text(font, 5, 507, RGBA8(0,0,0,150), 1.2f, "Based on Sean M. Tracey's idea");
	vita2d_pvf_draw_text(font, 5, 530, RGBA8(0,0,0,180), 1.2f, "Made with       by Bernardo Giordano, ported by Joel16");
	vita2d_draw_texture(_lives16p, 125, 515);
	
	endDrawing();
}

void level(bool timer) 
{
	int x, y;
	bool win = false;
	bool pause = false;
	char stagestr[10], rowstr[13];
	snprintf(stagestr, 10, "Score %d", stages);
	snprintf(rowstr, 13, "%d in a row", inRow);
	
	_time = 960;
	fillPattern();
	diff = (rand() % 2) ? diff : -diff;
	
	while(1 && _time > 0 && lives > 0) 
	{
		controls();
		touchUpdate();
		
		if (touchCheckIsPressed()) 
		{
			int x_start, y_start = 67;
			for (unsigned int i = 0; i < 4; i++) 
			{
				x_start = 276;
				for (unsigned int j = 0; j < 4; j++) 
				{
					if ((touchGetX() > x_start) && (touchGetX() < (x_start + 100)) && (touchGetY() > y_start) && (touchGetY() < (y_start + 100))) 
					{
						if (pause == false)
						{
							if (pattern[i*4+j])
								win = true;
							else
							{
								vitaAudioPlayWavSound(&nope, "app0:files/nope.wav");
								lives--;
								inRow = 0;
							}
						}
					}
					x_start += 103;
				}
				y_start += 103;
			}
		}
		
		if (pressed & SCE_CTRL_START)
		{
			if (pause == true)
				pause = false;
			else 
				pause = true;
		}
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		for (unsigned int i = 0, max = (lives < 5) ? 5 : lives; i < max; i++) 
		{
			if (i < lives)
				vita2d_draw_texture(_lives, 10 + i*27, 10);
			else
				vita2d_draw_texture_tint(_lives, 10 + i*27, 10, RGBA8(50, 50, 50, 200));
		}
		vita2d_pvf_draw_text(font, 945 - vita2d_pvf_text_width(font, 1.8f, stagestr), 35, RGBA8(128, 128, 128, 255), 1.8f, stagestr);
		vita2d_pvf_draw_text(font, 945 - vita2d_pvf_text_width(font, 1.8f, rowstr), 70, RGBA8(128, 128, 128, 255), 1.8f, rowstr);
		
		y = 67;
		for (unsigned int i = 0; i < 4; i++) 
		{
			x = 276;
			for (unsigned int j = 0; j < 4; j++) 
			{
				vita2d_draw_rectangle(x, y, 100, 100, (pattern[i*4+j]) ? RGBA8(color[0] - diff, color[1] - diff, color[2] - diff, 255) : RGBA8(color[0], color[1], color[2], 255));
				x += 103;
			}
			y += 103;
		}
		
		if (timer == true)
			vita2d_draw_rectangle(0, 524, _time, 20, (_time > 100) ? RGBA8(0, 255, 0, 255) : RGBA8(255, 0, 0, 255));
		
		if (pause == true)
		{
			vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(128, 128, 128, 130));
			vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, "Paused")) / 2, 200, RGBA8(200, 0, 0, 255), 1.8f, "Paused");	
			vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, "Press start to continue")) / 2, 260, RGBA8(0, 0, 0, 255), 1.8f, "Press start to continue");
			vita2d_pvf_draw_text(font, (960 - vita2d_pvf_text_width(font, 1.8f, "Press O to exit")) / 2, 300, RGBA8(0, 0, 0, 255), 1.8f, "Press O to exit");
			
			if (pressed & SCE_CTRL_CIRCLE)
				lives = 0;
		}
		
		endDrawing();

		if (timer == true)
		{
			if (pause == false)
			{
				if (--_time == 0 && lives > 0) 
				{
					vitaAudioPlayWavSound(&nope, "app0:files/nope.wav");
					_time = 960;
					lives--;
					inRow = 0;
				}
			}
		}
		
		if (win) 
		{
			if (++stages % 10 == 0 && lives < 10)
				lives++;			
			updateDiff();
			maxStages = (stages >= maxStages) ? stages : maxStages;
			inRow++;
			
			if (timer == true)
				level(true);
			else 
				level(false);
		}
	}
}

int main(int argc, char *argv[])
{
	maxStages = 0;
	matches = 0;
	initServices();
	
	while(1) 
	{
		controls();
		
		if (pressed & SCE_CTRL_CIRCLE)
			break;
		
		else if (pressed & SCE_CTRL_CROSS) 
		{
			level(true);
			matches++;
			gameOver();
			initVars();
		}
		
		else if (pressed & SCE_CTRL_SQUARE) 
		{
			level(false);
			matches++;
			gameOver();
			initVars();
		}
		
		mainMenu();
	}
	
	exitServices();
	return 0;
}