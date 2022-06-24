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

#ifndef HLOG_H
#define HLOG_H

/* -------------------------------------------------------------------------- */
enum {
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_NONE
};

enum {
    KEY_BKSP    = 8,
    KEY_TAB     = 9,
    KEY_RETURN  = 13,
    KEY_ESC     = 27,
    KEY_SPACE   = 32,
    KEY_0       = 48,
    KEY_1, KEY_2, KEY_3,
    KEY_4, KEY_5, KEY_6,
    KEY_7, KEY_8, KEY_9,
    KEY_A       = 97,
    KEY_B, KEY_C, KEY_D, KEY_E, KEY_F,
    KEY_G, KEY_H, KEY_I, KEY_J, KEY_K,
    KEY_L, KEY_M, KEY_N, KEY_O, KEY_P,
    KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U,
    KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
    KEY_RIGHT   = 278,
    KEY_LEFT, KEY_DOWN, KEY_UP
};

/* -------------------------------------------------------------------------- */
// Windowing
typedef struct {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int width;
    int height;
} Window;

typedef struct {
    SDL_Texture* texture;
} Texture;

/* -------------------------------------------------------------------------- */
void logLog(int level, const char* label, const char* file, int line, 
        const char* fmt, ...);
void setLogLevel(int level);

bool openWindow(int width, int height, const char* title);
void closeWindow();
bool windowShouldClose();

void pollEvents(void);
bool isKeyDown(int key);
bool isKeyPressed(int key);
bool isKeyUp(int key);
bool isKeyReleased(int key);

bool loadTexture(Texture* texture, const char* path);
void drawTexture(Texture texture);

void beginDrawing();
void endDrawing();
void clearBackground();

/* -------------------------------------------------------------------------- */
#define logTrace(...) logLog(LOG_LEVEL_TRACE, \
        "TRACE", __FILE__, __LINE__, __VA_ARGS__)
#define logDebug(...) logLog(LOG_LEVEL_DEBUG, \
        "DEBUG", __FILE__, __LINE__, __VA_ARGS__)
#define logInfo(...) logLog(LOG_LEVEL_INFO, \
        "INFO ", __FILE__, __LINE__, __VA_ARGS__)
#define logWarn(...) logLog(LOG_LEVEL_WARN, \
        "WARN ", __FILE__, __LINE__, __VA_ARGS__)
#define logError(...) logLog(LOG_LEVEL_ERROR, \
        "ERROR", __FILE__, __LINE__, __VA_ARGS__)
#define logFatal(...) logLog(LOG_LEVEL_FATAL, \
        "FATAL", __FILE__, __LINE__, __VA_ARGS__)

#endif
