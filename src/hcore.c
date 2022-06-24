/* ========================================================================== *\
 *
 *  hcore.c
 *  Lisence is at the bottom of the file.
 *
\* ========================================================================== */

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "hcore.h"

/*
 * GLOBAL STATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -------------------------------------------------------------------------- */
// Logging
static int logLevel = LOG_LEVEL_TRACE;

// Windowing
SDL_Window* g_Window = NULL;
SDL_Renderer* g_Renderer = NULL;

// Input
#define MAX_EVENT_KEY_STATES 512
#define EVENT_KEY_MODULO 1073741625
static bool eventClosingWindowID = NULL;
static bool eventCurrentKeyState[MAX_EVENT_KEY_STATES] = { false };
static bool eventPreviousKeyState[MAX_EVENT_KEY_STATES] = { false };


/*
 * LOGGING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -------------------------------------------------------------------------- */
void setLogLevel(int level)
{
    logLevel = level;
}

/* -------------------------------------------------------------------------- */
void logLog(
        int level,
        const char* label, 
        const char* file, 
        int line, 
        const char* fmt, ...)
{
    if (level >= logLevel) {
        time_t rawtime;
        char buf[10];
        va_list ap;

        time(&rawtime);
        strftime(buf, 10, "%H:%M:%S", localtime(&rawtime));
        printf("%s [%s] %s:%d -- ", buf, label, file,
                line);

        va_start(ap, fmt);
        vfprintf(stdout, fmt, ap);
        va_end(ap);

        printf("\n");
    }
}



/*
 * WINDOWING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -------------------------------------------------------------------------- */
bool 
openWindow(int width, int height, const char* title) 
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        logError("Failed to init SDL! SDL Error: %s", SDL_GetError());
        success = false;
    }

    if (success) {
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            logError("Failed to init SDL_image! IMG Error: %s", 
                    IMG_GetError());
            success = false;
        }
    }

    g_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    if (success) {
        if (g_Window == NULL) {
            logError("Failed to create window! SDL Error: %s", SDL_GetError());
            success = false;
        }    
    }

    if (success) {
        g_Renderer = SDL_CreateRenderer(g_Window, -1, 
                SDL_RENDERER_ACCELERATED);
        if (g_Renderer == NULL) {
            logError("Failed to create g_Renderer for window! SDL Error: %s",
                    SDL_GetError());
            success = false;
        } else {
            SDL_SetRenderDrawColor(g_Renderer, 0x0F, 0x0F, 0x0F, 0xFF);
        }
    }

    if (success) {
        logTrace("Opened window: (%d, %d) '%s'", width, height, title);
    }

    return success;
}

/* -------------------------------------------------------------------------- */
void
closeWindow(void)
{
    SDL_DestroyRenderer(g_Renderer);
    SDL_DestroyWindow(g_Window);
    IMG_Quit();
    SDL_Quit();
    logTrace("Closed window!");
}

/* -------------------------------------------------------------------------- */
bool 
windowShouldClose()
{
    if (SDL_GetWindowID(g_Window) == eventClosingWindowID) {
        eventClosingWindowID = NULL;
        return true;
    } else {
        return false;
    }
}



/*
 * INPUT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -------------------------------------------------------------------------- */
void 
pollEvents(void) 
{
    SDL_Event event;

    for (int i = 0; i < MAX_EVENT_KEY_STATES; i++) {
        eventPreviousKeyState[i] = eventCurrentKeyState[i];
    }
    while(SDL_PollEvent(&event) != 0) {
        switch(event.type) {
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        eventClosingWindowID = event.window.windowID;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYDOWN:
                for (int i = 0; i < MAX_EVENT_KEY_STATES; i++) {
                    if (event.key.keysym.sym % 1073741625 == i) {
                        eventCurrentKeyState[i] = true;
                    }
                }
                break;
            case SDL_KEYUP:
                for (int i = 0; i < MAX_EVENT_KEY_STATES; i++) {
                    if (event.key.keysym.sym % 1073741625 == i) {
                        eventCurrentKeyState[i] = false;
                    }
                }
                break;
            default:
                break;
        }
    }
}

/* -------------------------------------------------------------------------- */
bool
isKeyUp(int key)
{
    return !eventCurrentKeyState[key];
}

/* -------------------------------------------------------------------------- */
bool
isKeyPressed(int key)
{
    if (!eventPreviousKeyState[key] && eventCurrentKeyState[key]) {
        return true;
    } else {
        return false;
    }
}

/* -------------------------------------------------------------------------- */
bool
isKeyDown(int key)
{
    return eventCurrentKeyState[key];
}

/* -------------------------------------------------------------------------- */
bool
isKeyReleased(int key)
{
    if (eventPreviousKeyState[key] && !eventCurrentKeyState[key]) {
        return true;
    } else {
        return false;
    }
}




/*
 * BASIC DRAWING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -------------------------------------------------------------------------- */
void
beginDrawing()
{
}

/* -------------------------------------------------------------------------- */
void 
endDrawing()
{
    SDL_RenderPresent(g_Renderer);
}

/* -------------------------------------------------------------------------- */
void
clearBackground(Color color)
{
    SDL_SetRenderDrawColor(g_Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(g_Renderer);
}

/*
 * TEXTURES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -------------------------------------------------------------------------- */
bool
loadTexture(Texture* texture, const char* path)
{
    bool success = true;
    texture->texture = NULL;
    SDL_Surface* img = IMG_Load(path);
    if (img == NULL) {
        logError("Failed to load image! IMG Error: %s", IMG_GetError());
        success = false;
    }

    if (success) {
        texture->texture = SDL_CreateTextureFromSurface(g_Renderer, img);
        if (texture->texture == NULL) {
            logError("Failed to create texture from %s! IMG Error: %s", 
                    path, IMG_GetError());
            success = false;
        } else {
            SDL_FreeSurface(img);
            logTrace("Loaded texture: '%s'", path);
        }
    }

    return success;
}

/* -------------------------------------------------------------------------- */
void
drawTexture(Texture texture)
{
    SDL_RenderCopy(g_Renderer, texture.texture, NULL, NULL);
}
/* ========================================================================== *\
 *
 *  This is free and unencumbered software released into the public domain.
 *    
 *  Anyone is free to copy, modify, publish, use, compile, sell, or
 *  distribute this software, either in source code form or as a compiled
 *  binary, for any purpose, commercial or non-commercial, and by any
 *  means.
 *     
 *  In jurisdictions that recognize copyright laws, the author or authors
 *  of this software dedicate any and all copyright interest in the
 *  software to the public domain. We make this dedication for the benefit
 *  of the public at large and to the detriment of our heirs and
 *  successors. We intend this dedication to be an overt act of
 *  relinquishment in perpetuity of all present and future rights to this
 *  software under copyright law.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 *     
 *  For more information, please refer to <http://unlicense.org/>
 *
\* ========================================================================== */
