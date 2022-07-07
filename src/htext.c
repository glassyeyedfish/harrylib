/* ========================================================================== *\
 *
 *  htext.c
 *  Lisence is at the bottom of the file.
 *
\* ========================================================================== */

#include "hcore.h"
#include "htext.h"

/* -------------------------------------------------------------------------- */
extern SDL_Renderer* g_Renderer;

/* -------------------------------------------------------------------------- */
bool
loadFont(Font* font, const char* path, int ptsize)
{
    bool success = true;
    font->font = NULL;

    font->font = TTF_OpenFont(path, ptsize);
    font->ptsize = ptsize;
    if (font->font == NULL) {
        logError("Failed to load font at '%s'! TTF Error: %s", 
                path, TTF_GetError());
        success = false;
    } else {
        logTrace("Loaded font '%s'!", path);
    }

    return success;
}

/* -------------------------------------------------------------------------- */
void 
unloadFont(Font* font)
{
    TTF_CloseFont(font->font);
    font->font = NULL;
    logTrace("Unloaded font!");
}

/* -------------------------------------------------------------------------- */
bool
drawFont(Font* font, const char* text, int x, int y, int ptsize, Color color)
{
    bool success = true;
    SDL_Surface* surf;
    SDL_Texture* tex;
    SDL_Rect rect;

    surf = TTF_RenderText_Solid(font->font, text, 
            (SDL_Color) {color.r, color.g, color.b, color.a});
    if (surf == NULL) {
        logError("Failed to render text to surface. TTF Error: %s", 
                TTF_GetError());
        success = false;
    }

    if (success) {
        tex = SDL_CreateTextureFromSurface(g_Renderer, surf);
        if (tex == NULL) {
            logError("Faild to convert text to texture. SDL Error %s",
                    SDL_GetError());
            success = false;
        }
    }

    if (success) {
        rect = (SDL_Rect) {x, y, 
            surf->w * (ptsize / font->ptsize), 
            surf->h * (ptsize / font->ptsize)};
        SDL_RenderCopy(g_Renderer, tex, NULL, &rect);
    }

    return success;
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

