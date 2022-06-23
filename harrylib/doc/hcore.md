# Core Documentation
Heavy work in progress.

## Enums

### Keys
```c
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
```

### Log Levels
These are the levels used by the logging system from lowest to highest priority.
```c
enum {
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_NONE
};
```

## Structs

### Window
The single window used for rendering.
- SDL_Window* window
- SDL_Renderer* renderer

### Texture
Used to store texture data.
- SDL_Texture* texture
