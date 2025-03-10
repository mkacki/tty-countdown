#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // For sleep()
#include <string.h>
#include <sys/ioctl.h>  // For terminal width and height
#include <signal.h>

/** Get the terminal's width in columns */
int get_terminal_width() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
    return 80;  // Default to 80 if we can't get the width
}

/** Get the terminal's height in rows */
int get_terminal_height() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_row;
    }
    return 24;  // Default to 24 if we can't get the height
}

/** Clear the terminal screen */
void clear_screen() {
    printf("\033[2J\033[1;1H");  // ANSI escape codes to clear and move cursor to top-left
}

/** Define a 7x13 representation for a digit using solid blocks */
void display_digit(int digit, char display[7][32]) {
    const char *digits[10][7] = {
        // 0: Top, bottom, and all verticals
        {"   ██████   ",
         "  ██    ██  ",
         "  ██    ██  ",
         "  ██    ██  ",
         "  ██    ██  ",
         "  ██    ██  ",
         "   ██████   "},
        // 1: Right vertical bar
        {"      ██    ",
         "     ███    ",
         "      ██    ",
         "      ██    ",
         "      ██    ",
         "      ██    ",
         "    ██████  "},
        // 2: Top, middle, bottom with diagonals
        {"   ██████   ",
         "  ██    ██  ",
         "        ██  ",
         "   ██████   ",
         "  ██        ",
         "  ██        ",
         "  ███████   "},
        // 3: Top, middle, bottom
        {"   ██████   ",
         "  ██    ██  ",
         "        ██  ",
         "   ██████   ",
         "        ██  ",
         "  ██    ██  ",
         "   ██████   "},
        // 4: Left vertical and middle
        {"  ██    ██  ",
         "  ██    ██  ",
         "  ██    ██  ",
         "  ████████  ",
         "        ██  ",
         "        ██  ",
         "        ██  "},
        // 5: Top, middle, bottom with left vertical
        {"  ███████   ",
         "  ██        ",
         "  ██        ",
         "  ███████   ",
         "        ██  ",
         "  ██    ██  ",
         "   ██████   "},
        // 6: All except top-right
        {"   ██████   ",
         "  ██        ",
         "  ██        ",
         "   ██████   ",
         "  ██    ██  ",
         "  ██    ██  ",
         "   ██████   "},
        // 7: Top and right vertical
        {"  ██████    ",
         "      ██    ",
         "      ██    ",
         "      ██    ",
         "      ██    ",
         "      ██    ",
         "      ██    "},
        // 8: All segments
        {"   ██████   ",
         "  ██    ██  ",
         "  ██    ██  ",
         "   ██████   ",
         "  ██    ██  ",
         "  ██    ██  ",
         "   ██████   "},
        // 9: All except bottom-left
        {"   ██████   ",
         "  ██    ██  ",
         "  ██    ██  ",
         "   ███████  ",
         "        ██  ",
         "        ██  ",
         "    █████   "},
    };
    for (int i = 0; i < 7; i++) {
        strcpy(display[i], digits[digit][i]);
    }
}

/** Print the timer with centered alignment */
void print_timer(int hours, int minutes, int seconds, int left_padding) {
    char disp[8][7][32];  // 8 elements: HH:MM:SS (6 digits + 2 colons)
    // Break down time into digits
    int h1 = hours / 10;
    int h2 = hours % 10;
    int m1 = minutes / 10;
    int m2 = minutes % 10;
    int s1 = seconds / 10;
    int s2 = seconds % 10;

    // Populate display array
    display_digit(h1, disp[0]);
    display_digit(h2, disp[1]);
    // Colon between hours and minutes
    for (int i = 0; i < 7; i++) {
        strcpy(disp[2][i], (i == 2 || i == 4) ? "     ██     " : "            ");
    }
    display_digit(m1, disp[3]);
    display_digit(m2, disp[4]);
    // Colon between minutes and seconds
    for (int i = 0; i < 7; i++) {
        strcpy(disp[5][i], (i == 2 || i == 4) ? "     ██     " : "            ");
    }
    display_digit(s1, disp[6]);
    display_digit(s2, disp[7]);

    // Print each row with padding
    for (int row = 0; row < 7; row++) {
        printf("%*s", left_padding, "");  // Left padding for centering
        for (int digit = 0; digit < 8; digit++) {
            printf("%s ", disp[digit][row]);  // Space between elements
        }
        printf("\n");
    }
}

/** Run the countdown with a message */
void countdown(int total_seconds, const char *message) {
    printf("\033[?25l");  // Hide cursor
    while (total_seconds >= 0) {
        clear_screen();
        int terminal_width = get_terminal_width();
        int terminal_height = get_terminal_height();
        int timer_width = (13 + 1) * 8 - 1;  // 13 chars per element, 8 elements, spaces
        int left_padding = (terminal_width - timer_width) / 2;
        if (left_padding < 0) left_padding = 0;
        int content_height = 9;  // 7 for clock, 1 empty, 1 for message
        int top_padding = (terminal_height - content_height) / 2;
        if (top_padding < 0) top_padding = 0;

        // Add top padding for vertical centering
        for (int i = 0; i < top_padding; i++) {
            printf("\n");
        }

        int hours = total_seconds / 3600;
        int minutes = (total_seconds % 3600) / 60;
        int seconds = total_seconds % 60;

        printf("\033[32m");  // Set color to green
        print_timer(hours, minutes, seconds, left_padding);
        printf("\n");  // Empty line
        int message_padding = (terminal_width - strlen(message)) / 2;
        if (message_padding < 0) message_padding = 0;
        printf("%*s%s\n", message_padding, "", message);
        printf("\033[0m");  // Reset color

        sleep(1);
        total_seconds--;
    }

    // Display "Time's up!" when done
    clear_screen();
    int terminal_width = get_terminal_width();
    int terminal_height = get_terminal_height();
    const char *end_message = "Time's up!";
    int end_padding = (terminal_width - strlen(end_message)) / 2;
    int end_top_padding = (terminal_height - 1) / 2;
    for (int i = 0; i < end_top_padding; i++) {
        printf("\n");
    }
    printf("\033[32m%*s%s\n\033[0m", end_padding, "", end_message);
    printf("\033[?25h");  // Show cursor
}

void sigint_handler(int signum) {
    const char show_cursor[] = "\033[?25h";
    write(STDOUT_FILENO, show_cursor, 6);
    exit(1);
}

int main(int argc, char *argv[]) {

    // Set up SIGINT handler 
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    
    if (argc != 3) {
        printf("Usage: %s HH:MM:SS message\n", argv[0]);
        return 1;
    }

    int hours, minutes, seconds;
    if (sscanf(argv[1], "%d:%d:%d", &hours, &minutes, &seconds) != 3) {
        printf("Invalid time format. Use HH:MM:SS\n");
        return 1;
    }

    int total_seconds = hours * 3600 + minutes * 60 + seconds;
    if (total_seconds < 0) {
        printf("Please enter a valid positive time!\n");
        return 1;
    }

    countdown(total_seconds, argv[2]);
    return 0;
}
