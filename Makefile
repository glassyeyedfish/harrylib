# Vars
PLATFORM 		= PLATFORM_DESKTOP
SRC_FILES 		= test/src/*.c src/*.c
INCLUDE_DIRS 	= -Iinclude

.PHONY: all

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
CC = gcc
CFLAGS  = -Wall -std=c99 -lSDL2 -lSDL2_image -lSDL2_ttf
OUT_FILE = test/build/program
EXTRA_CMD =
endif

ifeq ($(PLATFORM),PLATFORM_WEB)
CC = emcc
CFLAGS  = -Wall -pedantic -std=c99 -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 \
		-s SDL2_IMAGE_FORMATS=['png'] -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 \
		--preload-file test/res
OUT_FILE = test/build/index.html
# EXTRA_CMD = cp default.html test/build/index.html
EXTRA_CMD = 
endif

all: $(SOURCE_FILES)
	$(CC) $(SRC_FILES) $(INCLUDE_DIRS) $(CFLAGS) -o $(OUT_FILE)
	$(EXTRA_CMD)
	
