#include <system/shell.h>

#define MAX_INPUT_LENGTH 256
#define MAX_COMMAND_LENGTH 20


uint8_t taking_input;
uint8_t input_offset;
uint8_t input[MAX_INPUT_LENGTH];

// Commands
char commands[][MAX_COMMAND_LENGTH] = {
    "echo",
};
extern void (*command_handlers[])(char** argv, int argc);

void process_command(char* command) {
    int argc = 0;
    uint32_t i = 0; uint32_t new_word = 1;

    // Get the count of arguments
    while (command[i] != '\0') {
        if (command[i] == ' ') {
            new_word = 1;
        } else {
            if (new_word == 1) {
                argc++;
                new_word = 0;
            }
        }
        i++;
    }

    if (argc == 0)
        return;

    char** argv = (char**)malloc(argc * sizeof(char*));

    // Get the arguments
    char* temp = command;
    for (i=0; i<argc; ++i) {
        while (*temp == ' ')
            temp++;
        
        int len = 0;
        while (temp[len] != ' ' && temp[len] != '\0') {
            len++;
        }
        len++;
        
        argv[i] = (char*)malloc(len * sizeof(char));
        
        len = 0;
        while (*temp != ' ' && *temp != '\0') {
            argv[i][len++] = *temp;
            temp++;
        }
        argv[i][len] = '\0';
    }

    // Find the best matching command
    int num_commands = sizeof(commands) / MAX_COMMAND_LENGTH;
    for (int i=0; i<num_commands; ++i) {
        if (strcmp(commands[i], argv[0]) == 0) {
            // Call the command handler
            command_handlers[i](argv, argc);
            break;
        }
    }

    if (i == num_commands) {
        puts(argv[0]); puts(": command not found\n");
    }

    for (i=0; i<argc; ++i) {
        free(argv[i]);
    }
    free(argv);
}


void init_shell() {
    taking_input = 0;
    puts("shell $ ");
    input_offset = 0;
    taking_input = 1;
}


void shell_handle_input(uint8_t code, uint8_t down) {
    // Don't let erase more than that was typed
    if (down && us_map[code] == '\b') {
        if (input_offset == 0)
            return;
        else
            input_offset--;
    }

    // Print the character
    uint8_t ch = print_key_input(code, down);

    // Store the input
    if (ch == '\t') {
        input[input_offset++] = ' ';
        input[input_offset++] = ' ';
        input[input_offset++] = ' ';
        input[input_offset++] = ' ';
    }
    else if (ch == '\n') {
        // End of input
        taking_input = 0;
        input[input_offset] = '\0';
        process_command((char*)input);
        init_shell();
    }
    else if (ch != 0) {
        input[input_offset++] = ch;
    }
}


void echo(char** argv, int argc) {
    for (int i=1; i<argc; ++i) {
        puts(argv[i]); puts(" ");
    }
    puts("\n");
}

void (*command_handlers[])(char** argv, int argc) = {
    echo
};