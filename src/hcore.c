/* ========================================================================== *\
 *
 *  hcore.c
 *  Lisence is at the bottom of the file.
 *
\* ========================================================================== */

#include "hcore.h"

/*
 * GLOBAL STATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -------------------------------------------------------------------------- */
// Logging
static uint8_t logLevel = LOG_LEVEL_TRACE;

// Windowing
SDL_Window* g_Window = NULL;
SDL_Renderer* g_Renderer = NULL;

// Input
#define MAX_EVENT_KEY_STATES 512
#define EVENT_KEY_MODULO 1073741625
static bool eventClosingWindowID = NULL;
static bool eventCurrentKeyState[MAX_EVENT_KEY_STATES] = { false };
static bool eventPreviousKeyState[MAX_EVENT_KEY_STATES] = { false };

// Util
uint64_t currentTime = 0;
uint64_t previousTime = 0;
uint64_t delta = 0;
uint64_t accumulatedTime = 0;
uint32_t frames = 0;
uint32_t fps = 0;
uint32_t targetFPS = 60;
uint32_t msPerFrame = 0;


/*
 * LOGGING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -------------------------------------------------------------------------- */
void setLogLevel(uint8_t level)
{
    logLevel = level;
}

/* -------------------------------------------------------------------------- */
void logLog(
        uint8_t level,
        const char* label, 
        const char* file, 
        uint32_t line, 
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
openWindow(uint32_t width, uint32_t height, const char* title) 
{
    bool success = true;

    msPerFrame = (uint32_t) ((1.0 / targetFPS) * 1000.0);

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

    if (success) {
        if (TTF_Init() < -1) {
            logError("Failed to init SDL_ttf! TTF Error: %s", TTF_GetError());
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
windowShouldClose(void)
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
isKeyUp(uint32_t key)
{
    return !eventCurrentKeyState[key];
}

/* -------------------------------------------------------------------------- */
bool
isKeyPressed(uint32_t key)
{
    if (!eventPreviousKeyState[key] && eventCurrentKeyState[key]) {
        return true;
    } else {
        return false;
    }
}

/* -------------------------------------------------------------------------- */
bool
isKeyDown(uint32_t key)
{
    return eventCurrentKeyState[key];
}

/* -------------------------------------------------------------------------- */
bool
isKeyReleased(uint32_t key)
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
beginDrawing(void)
{
}

/* -------------------------------------------------------------------------- */
void 
endDrawing(void)
{
    SDL_RenderPresent(g_Renderer);

    // FPS Junk
    uint64_t syncTime = msPerFrame - (SDL_GetTicks64() - currentTime);
    if (syncTime > msPerFrame) { syncTime = msPerFrame; }
    SDL_Delay(syncTime);
    previousTime = currentTime;
    currentTime = SDL_GetTicks64();
    delta = currentTime - previousTime;

    frames++;
    accumulatedTime += delta;

    if (accumulatedTime > 1000) {
        fps = frames;
        accumulatedTime -= 1000;
        frames = 0;
    }
}

/* -------------------------------------------------------------------------- */
void
clearBackground(Color color)
{
    SDL_SetRenderDrawColor(g_Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(g_Renderer);
}

/* -------------------------------------------------------------------------- */
bool
setDrawScale(uint32_t x, uint32_t y) 
{
    bool success = true;
    if (SDL_RenderSetScale(g_Renderer, (float) x, (float) y) < 0) {
        logError("Failed to scale renderer! SDL Error: %s", SDL_GetError());
        success = false;
    }
    return success;
}

/*
 * UTIL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */


double 
getDeltaTime(void)
{
    return delta;
}

uint32_t 
getFPS(void)
{
    return fps;
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
