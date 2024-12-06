#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/serialport.h>

char *title_card = 
    "\n\
       888      d8b                    .d88888b.   .d8888b.  \n\
       888      Y8P                   d88P\" \"Y88b d88P  Y88b \n\
       888                            888     888 Y88b.      \n\
       88888b.  888 88888b.  88888b.  888     888  \"Y888b.   \n\
       888 \"88b 888 888 \"88b 888 \"88b 888     888     \"Y88b. \n\
       888  888 888 888  888 888  888 888     888       \"888 \n\
       888  888 888 888 d88P 888 d88P Y88b. .d88P Y88b  d88P \n\
       888  888 888 88888P\"  88888P\"   \"Y88888P\"   \"Y8888P\"  \n\
                     888      888                             \n\
                     888      888                             \n\
                     888      888                             \n\n";

void kernel_main(void) {
	terminal_initialize();
	printf(title_card);
    printf("This is a test to see how long a string can be before line wraps around. Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum\n");
    printf("Test scrolling function: \n");
    for(char i = 65; i < 80; i++) {
        char str[3];
        str[0] = i;
        str[1] = '\n';
        str[2] = '\0';
        printf(str);
    }

    sp_setup(COM1);

    char *test_str = "Test";
    sp_write(test_str, COM1);
}
