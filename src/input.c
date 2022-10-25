#include "input.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define cursorforward(x) printf("\033[%dC", (x))
#define cursorbackward(x) printf("\033[%dD", (x))

#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108
#define KEY_BKSPC   0x007f

static struct termios term, oterm;
// https://stackoverflow.com/questions/33025599/move-the-cursor-in-a-c-program

static int get_char(void) {
    int c = 0;
    
    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    return c;
}

static int kb_hit(void) {
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}


static int kb_esc(void) {
    int c;

    if (!kb_hit()) return KEY_ESCAPE;
    c = get_char();
    if (c == '[') {
        switch (get_char()) {
            case 'A':
                c = KEY_UP;
                break;
            case 'B':
                c = KEY_DOWN;
                break;
            case 'C':
                c = KEY_LEFT;
                break;
            case 'D':
                c = KEY_RIGHT;
                break;
            default:
                c = 0;
                break;
        }
    } else {
        c = 0;
    }

    if (c == 0) while (kb_hit()) get_char();
    return c;
}

static int get_key(void) {
    int c;

    c = get_char();
    return (c == KEY_ESCAPE) ? kb_esc() : c;
}

int get_input(char *buffer) {
    int c;
    int input_count = 0;

    // input loop
    while (1) {
        // get char from user
        c = get_key();
        // check which char was entered
        if (c == KEY_ENTER || c == KEY_ESCAPE || c == KEY_UP || c == KEY_DOWN) {
            break;
        } else if (c == KEY_RIGHT) {
            cursorbackward(1);
            if (input_count > 0) {
                input_count--;
            }
        } else if (c == KEY_LEFT) {
            cursorforward(1);
            input_count++;
        // check for backspace
        } else if (c == KEY_BKSPC) {
            for (int i = input_count; buffer[i]; i++) {
                buffer[i] = buffer[i + 1];
            }
            if (input_count > 0) {
                putchar('\b');
                putchar(' ');
                putchar('\b');
                input_count--;
            }
        // print char as long as it is a symbol, number or letter
        } else if (c > 31) {
            putchar(c);
            buffer[input_count] = c;
            input_count++;
        }
    }
    putchar('\n');
    return 0;
}

void parse_input(char *input, char** input_array, int char_count) {
    int str_count = 0, arr_count = 0, chr_count = 0;
    char* string_buffer = malloc(128);
    for (int i = 0; input[i]; i++) {
        chr_count++;
        if (chr_count > char_count) {
            break;
        }
        if (input[i] == ' ') {
            // finish string, push to array
            string_buffer[str_count] = '\0';
            strcpy(input_array[arr_count], string_buffer);
            // reset string buffer
            for (int j = 0; j < str_count+1; j++) {
                string_buffer[j] = 0;
            }
            str_count = 0;
            arr_count++;
        } else if (input[i] == '\n') {
            break;
        } else {
            // push to string buffer, increase count
            string_buffer[str_count] = input[i];
            str_count++;
        }
    }
    // finish string, push to array
    string_buffer[str_count] = '\0';
    strcpy(input_array[arr_count], string_buffer);
    free(string_buffer);
}

void trim_input(char *input_buffer, char *clean_input, int *word_count, int *char_count) {
    // ignore trailing and leading whitespace, ignore consecutive spaces
    int clean_count = 0;
    char prev_char;
    for (int i = 0; input_buffer[i]; i++) {
        if (i == 0 && input_buffer[i] == ' ') {
            prev_char = ' ';
            continue;
        }
        if (input_buffer[i] == ' ' && prev_char != ' ') {
            (*word_count)++;
        }
        if (!(input_buffer[i] == ' ' && prev_char == ' ') && input_buffer[i] != '\n') {
            (*char_count)++;
            clean_input[clean_count] = input_buffer[i];
            clean_count++;
        }
        if (input_buffer[i] != '\n') {
            prev_char = input_buffer[i];
        }
    }
    if (prev_char == 32) {
        (*word_count)--;
        clean_input[clean_count-1] = 0;
    }
}   