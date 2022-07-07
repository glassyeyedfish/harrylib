/* ========================================================================== *\
 *
 *  hshapes.c
 *  Lisence is at the bottom of the file.
 *
\* ========================================================================== */

#include "hcore.h"
#include "hshapes.h"

/* -------------------------------------------------------------------------- */
extern SDL_Renderer* g_Renderer;

/* -------------------------------------------------------------------------- */
void
drawRect(int32_t x, int32_t y, int32_t w, int32_t h, Color color)
{
    SDL_Rect sdlrect = { x, y, w, h };
    SDL_SetRenderDrawColor(g_Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(g_Renderer, &sdlrect);
}

/* -------------------------------------------------------------------------- */
void
fillRect(int32_t x, int32_t y, int32_t w, int32_t h, Color color)
{
    SDL_Rect sdlrect = { x, y, w, h };
    SDL_SetRenderDrawColor(g_Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(g_Renderer, &sdlrect);
}

/* -------------------------------------------------------------------------- */
void 
drawPixel(int32_t x, int32_t y, Color color)
{
    SDL_SetRenderDrawColor(g_Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(g_Renderer, x, y);
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

