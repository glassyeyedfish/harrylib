#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "hcore.h"

/* -------------------------------------------------------------------------- */
Texture tex;

/* -------------------------------------------------------------------------- */
void 
main_loop(void)
{
    beginDrawing();

    clearBackground();
    drawTexture(tex);

    endDrawing();

    pollEvents();
}

/* -------------------------------------------------------------------------- */
int main (int argc, char* args[]) 
{
    openWindow(512, 448, "SDL Test");
    loadTexture(&tex, "res/guy.png");

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while(!windowShouldClose()) {
        main_loop();
        SDL_Delay(16);
    }
#endif

    closeWindow();
    return 0;
}
