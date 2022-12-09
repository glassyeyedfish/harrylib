#include <stdio.h>

#define HARRYLIB_IMPLEMENTATION
#include "../harrylib.h"

#define WINDOW_TITLE "Harrylib Test"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

/*
TODO:

- Fix bug where player shoots down screen, probably something wrong with
  event handling code.
*/

hl_font_t* font;

void
main_init(void) {
    font = hl_load_bitmap_font("font.png", 8, 8);
}

void
main_loop(void) {
    hl_poll_events();
    hl_begin_draw();

    hl_clear(HL_BLACK);

    hl_draw_text(font, "$2", 40, 40, HL_ORANGE);

    hl_end_draw();
}

void
main_exit(void) {
    hl_unload_font(font);
}

int
main(void) {
    hl_result_t r;

    printf("%c\n", 1);

    r = hl_open_window(
        WINDOW_TITLE,
        WINDOW_WIDTH, 
        WINDOW_HEIGHT
    );
    if (r.flag == HL_RESULT_ERROR) {
        fprintf(stderr, "[harrylib] error: %s", r.err);
    } else {
        main_init();

        while(!hl_window_should_close()) {
            main_loop();
            hl_delay(16);
        }

        main_exit();
    }

    hl_close_window();

    return 0;
}