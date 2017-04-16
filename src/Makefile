TARGET 		:= PickrVITA
PROJECT_TITLE := Pickr VITA
TITLE_ID 	:= PICKRV016
SOURCES 	:= src

OBJS = fs.o main.o touch.o utils.o \
	   res/lives.o res/lives16p.o \

PREFIX   = arm-vita-eabi
CC       = $(PREFIX)-gcc
CXX      = $(PREFIX)-g++
CFLAGS  	= -Wl,-q -g -Wall -Werror -O3
CXXFLAGS = $(CFLAGS) -std=c++11 -fno-rtti -fno-exceptions
ASFLAGS  = $(CFLAGS)

INCLUDE_DIR := include
INCDIR = $(INCLUDE_DIR)

LIBDIR = lib
LDFLAGS = 
LIBS := -lvita2d -lSceDisplay_stub -lSceGxm_stub \
		-lSceSysmodule_stub -lSceCtrl_stub -lSceTouch_stub -lScePvf_stub \
		-lSceCommonDialog_stub -lfreetype -lpng -ljpeg -lz -lm -lc \
		-lSceAudio_stub -lSceAudiodec_stub lib/libVitaAudio.a

all: $(TARGET).vpk

%.vpk: eboot.bin
	vita-mksfoex -d PARENTAL_LEVEL=1 -s APP_VER=01.10 -s TITLE_ID=$(TITLE_ID) "$(PROJECT_TITLE)" param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin \
		--add ../sce_sys/icon0.png=sce_sys/icon0.png \
		--add ../sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png \
		--add ../sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png \
		--add ../sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \
		--add res/game_over.wav=files/game_over.wav \
		--add res/nope.wav=files/nope.wav \
	$(TARGET).vpk
	
eboot.bin: $(TARGET).velf
	vita-make-fself -s -c $< $@

%.velf: %.elf
	$(PREFIX)-strip -g $<
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.o: %.png
	$(PREFIX)-ld -r -b binary -o $@ $^

clean:
	@rm -rf $(TARGET).vpk $(TARGET).velf $(TARGET).elf $(TARGET).unstripped.elf $(OBJS) \
		eboot.bin param.sfo
