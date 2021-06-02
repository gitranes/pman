#include "input/term.h"

#include <termios.h>

#include <stdio.h>
#include <stdlib.h>

struct TerminalHandle
{
    struct termios old;
    struct termios new;
};

struct TerminalHandle* term_handle_init()
{
    struct termios current_state = {0};
    if (tcgetattr(fileno(stdin), &current_state) != 0)
    {
        return NULL;
    }
    struct TerminalHandle* const new_handle =
        calloc(1, sizeof(struct TerminalHandle));
    new_handle->old = current_state;
    new_handle->new = current_state;

    return new_handle;
}

void term_handle_clean(struct TerminalHandle* term)
{
    if (!term)
    {
        return;
    }
    term_echo_on(term);
    free(term);
}

int term_echo_off(struct TerminalHandle* term)
{
    term->new.c_lflag &= ~ECHO; // NOLINT(hicpp-signed-bitwise)
    return tcsetattr (fileno(stdin), TCSAFLUSH, &term->new);
}

int term_echo_on(struct TerminalHandle* term)
{
    return tcsetattr(fileno(stdin), TCSAFLUSH, &term->old);
}
