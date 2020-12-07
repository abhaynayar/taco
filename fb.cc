extern "C" {
    #include "serial.h"
    #include "io.h"
    #include "fb.h"
}

FrameBuffer::FrameBuffer() {
    base = (char*) 0x000B8000;

    row = 0;
    col = 0;

    // Color: white on black
    fg = 0;
    bg = 15;
}

/** write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void FrameBuffer::write_cell(unsigned int i, char c) {
    base[i] = c;
    base[i+1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

/** move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *  @param pos The new position of the cursor
 */

void FrameBuffer::move_cursor() {
    int pos = row*SCREEN_WIDTH + col;
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);

    col++; // NOTE: No need to update row (not sure)
}

void FrameBuffer::write(char *str, unsigned int len) {

    for (unsigned int i=0; i<len; ++i) {
        int pos = row*SCREEN_WIDTH + col;

        if (str[i] == '\n') {
            row++;
            col = 0;
            continue;
        }

        FrameBuffer::write_cell(pos*2, str[i]);
        FrameBuffer::move_cursor();
    }

}

void FrameBuffer::clear_screen() {
    for (int i=0; i<SCREEN_WIDTH*SCREEN_HEIGHT; ++i) {
        FrameBuffer::write_cell(i*2, ' ');
    }
}