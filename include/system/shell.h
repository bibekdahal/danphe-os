#pragma once
#include <system/system.h>

void shell_handle_input(uint8_t code, uint8_t down);
void init_shell();
void process_command(char* command);