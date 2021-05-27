#pragma once

struct TerminalHandle;

struct TerminalHandle* term_handle_init();
void term_handle_clean(struct TerminalHandle* term);

int term_echo_off(struct TerminalHandle* term);
int term_echo_on(struct TerminalHandle* term);
