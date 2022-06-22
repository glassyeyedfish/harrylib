CC = gcc

SOURCE_FILES = \
			   src/* \
			   harrylib/src/*

INCLUDE_FLAGS = -Iharrylib/include

PREPROCESSOR_FLAGS = 

COMPILER_FLAGS = -Wall -pedantic -std=c99

LINKER_FLAGS = -lSDL2 -lSDL2_image

OUTPUT_FILE = build/program

default: $(SOURCE_FILES)
	$(CC) $(SOURCE_FILES) $(INCLUDE_FLAGS) $(PREPROCESSOR_FLAGS) \
		$(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUTPUT_FILE)

web: $(SOURCE_FILES)
	emcc $(SOURCE_FILES) $(INCLUDE_FLAGS) -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 \
		-s SDL2_IMAGE_FORMATS=['png'] -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 \
		--preload-file res -o build/index.html
	cp default.html build/index.html
