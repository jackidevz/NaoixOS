/* ***************************************************************************** */
/* *                                                                           * */
/* *                                                               ::::::::::  * */
/* *  naoixsh.c                                                     :+:+:+:+:+   * */
/* *                                                                 +:+       * */
/* *  By: Jack (Khalid Oumouh)                                      +#+        * */
/* *                                                               +#+         * */
/* *  Created: 2026/07/05 14:38 by Jack                    #+#    #+#          * */
/* *  Updated: 2026/07/19 14:26 by Jack                     ########           * */
/* *                                                                           * */
/* ***************************************************************************** */



#include "naoixsh.h"
#include "vga.h"
#include "keyboard.h"
#include "serial.h"
#include "kstring.h"
#include "version.h"

#define LINE_MAX 128

static int dev_mode = 0;
static unsigned int rng_state = 0xC0FFEEu;

static unsigned int rng_next(void) {
    rng_state = rng_state * 1103515245u + 12345u;
    return (rng_state >> 16) & 0x7FFF;
}

/* Waits in small chunks, polling the keyboard so any keypress skips the
 * rest of the animation. Returns 1 if skipped, 0 if it ran to completion. */
static int wait_ticks(unsigned long chunks) {
    for (unsigned long c = 0; c < chunks; c++) {
        for (volatile unsigned long i = 0; i < 2000; i++) {
            __asm__ volatile ("nop");
        }
        if (keyboard_getchar() >= 0) return 1;
    }
    return 0;
}

static void matrix_rain(int lines, int width, int *skip) {
    static const char charset[] = "01";
    terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
    for (int l = 0; l < lines && !*skip; l++) {
        for (int w = 0; w < width; w++) {
            unsigned int idx = rng_next() % (sizeof(charset) - 1);
            terminal_putchar(charset[idx]);
        }
        terminal_putchar('\n');
        *skip = wait_ticks(600);
    }
}

static void readline(char *buf, int max) {
    int len = 0;
    for (;;) {
        int c = keyboard_getchar();
        if (c < 0) c = serial_getchar();
        if (c < 0) {
            __asm__ volatile ("hlt");
            continue;
        }

        if (c == '\n') {
            terminal_putchar('\n');
            buf[len] = 0;
            return;
        } else if (c == '\b') {
            if (len > 0) {
                len--;
                terminal_putchar('\b');
            }
        } else if (len < max - 1) {
            buf[len++] = (char)c;
            terminal_putchar((char)c);
        }
    }
}

static void print_color_bar(void) {
    for (uint8_t bg = 0; bg < 16; bg++) {
        terminal_setcolor(VGA_BLACK, bg);
        terminal_writestring("  ");
    }
    terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
    terminal_writestring("\n");
}

static void run_jackmasterdev(void) {
    int skip = 0;

    terminal_initialize();
    matrix_rain(12, 50, &skip);

    terminal_initialize();
    terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
    terminal_writestring("[NaoDEV] Initializing developer subsystem...\n");
    if (!skip) skip = wait_ticks(1500);

    terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
    terminal_writestring("[  OK  ] Bypassing shell restrictions\n");
    if (!skip) skip = wait_ticks(1500);

    terminal_writestring("[  OK  ] Loading developer credentials\n");
    if (!skip) skip = wait_ticks(1500);

    terminal_setcolor(VGA_LIGHT_BROWN, VGA_BLACK);
    terminal_writestring("[ WARN ] Unauthorized access detected... just kidding\n");
    if (!skip) skip = wait_ticks(1500);

    terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
    terminal_writestring("[  OK  ] Root access granted\n");
    wait_ticks(2500);

    terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
    terminal_writestring("\n   _____\n");
    terminal_writestring("  /     \\\n");
    terminal_setcolor(VGA_GREEN, VGA_BLACK);
    terminal_writestring(" | () () |");
    terminal_setcolor(VGA_LIGHT_CYAN, VGA_BLACK);
    terminal_writestring("   Master Dev Mode\n");
    terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
    terminal_writestring("  \\  ^  /");
    terminal_setcolor(VGA_LIGHT_MAGENTA, VGA_BLACK);
    terminal_writestring("   \"Welcome to Master Dev Mode.\"\n");
    terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
    terminal_writestring("   |||||\n\n");

    terminal_setcolor(VGA_WHITE, VGA_BLACK);
    terminal_writestring("NaoixOS was built in 4 days by one guy, one phone, and zero sleep.\n");
    terminal_writestring("No IDE. No desktop. Just Termux, stubbornness, and coffee.\n");
    terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
    terminal_writestring("Thanks for digging this deep. That's exactly the spirit this kernel was built with.\n\n");

    terminal_setcolor(VGA_LIGHT_RED, VGA_BLACK);
    terminal_writestring("Developer mode unlocked. Type 'sysinfo' for kernel internals.\n\n");
    terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
    dev_mode = 1;
}

static void print_kv(const char *key, const char *value) {
    terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
    terminal_writestring(key);
    terminal_setcolor(VGA_WHITE, VGA_BLACK);
    terminal_writestring(value);
    terminal_putchar('\n');
}

static void run_command(const char *line) {
    if (line[0] == 0) return;
    if (strcmp(line, "help") == 0) {
        terminal_writestring("Commands: help, about, clear, ver, colors\n");
    } else if (strcmp(line, "about") == 0) {
        terminal_setcolor(VGA_LIGHT_BROWN, VGA_BLACK);
        terminal_writestring("Kernel:");
        terminal_setcolor(VGA_WHITE, VGA_BLACK);
        terminal_writestring(" NaoixOS - basic x86 kernel builted as a hobby.\n");
        terminal_setcolor(VGA_LIGHT_BROWN, VGA_BLACK);
        terminal_writestring("Shell :");
        terminal_setcolor(VGA_WHITE, VGA_BLACK);
        terminal_writestring(" NaoixSH - basic x86 embedded shell for NaoixOS.\n");
        terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
    } else if (strcmp(line, "clear") == 0) {
        terminal_initialize();
    } else if (strcmp(line, "ver") == 0) {
        terminal_setcolor(VGA_LIGHT_MAGENTA, VGA_BLACK);
        terminal_writestring("Kernel:");
        terminal_setcolor(VGA_WHITE, VGA_BLACK);
        terminal_writestring(" NaoixOS " KEVER "\n");
        terminal_setcolor(VGA_LIGHT_MAGENTA, VGA_BLACK);
        terminal_writestring("Shell :");
	terminal_setcolor(VGA_WHITE, VGA_BLACK);
	terminal_writestring(" NaoixSH " SHVER "\n");
	terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
    } else if (strcmp(line, "colors") == 0) {
        print_color_bar();
    } else if (strcmp(line, "easterg") == 0) {
        terminal_setcolor(VGA_LIGHT_GREY, VGA_RED);
        terminal_writestring("You found the hidden message writen by the developer jackidevz (Khalid Oumouh)\nIf you are seeing this right now, i wanna say Thank you very much for use my\nNaoixOS.\nI will give you a little gift\n Type 'jackmasterdev' or 'jmdevel' in terminal to see it!\n");
        terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
    } else if (strcmp(line, "hidden-easter-egg") == 0) {
        terminal_setcolor(VGA_LIGHT_GREY, VGA_RED);
        terminal_writestring("You found the hidden message writen by the developer jackidevz (Khalid Oumouh)\nIf you are seeing this right now, i wanna say Thank you very much for use my\nNaoixOS.\nI will give you a little gift\n Type 'jackmasterdev' or 'jmdevel' in terminal to see it!\n");
        terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
    } else if (strcmp(line, "jackmasterdev") == 0) {
        run_jackmasterdev();
    } else if (strcmp(line, "jmdevel") == 0) {
	run_jackmasterdev();
    } else if (strcmp(line, "sysinfo") == 0) {
        if (!dev_mode) {
            terminal_writestring("Unknown command: sysinfo\n");
            terminal_writestring("Type 'help' for available commands.\n");
        } else {
            terminal_setcolor(VGA_LIGHT_CYAN, VGA_BLACK);
            terminal_writestring("===== NaoixOS Developer Info =====\n");
            print_kv("Kernel      : ", "NaoixOS "  KEVER);
            print_kv("Shell       : ", "NaoixSH " SHVER);
            print_kv("Boot mode   : ", "Multiboot2 / legacy stage1 (Limbo+SeaBIOS)");
            print_kv("Arch        : ", "x86 (protected mode)");
            print_kv("Video       : ", "VGA text mode 80x25");
            print_kv("Input       : ", "PS/2 keyboard (Set 1 / Set 2 auto-detect)");
            print_kv("Developer   : ", "Jack");
            terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
        }
    } else if (strcmp(line, "make me a sandwich") == 0) {
        terminal_setcolor(VGA_LIGHT_RED, VGA_BLACK);
        terminal_writestring("Permission denied: root privileges required.\n");
        terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
        terminal_writestring("Try: sudo make me a sandwich\n");
    } else if (strcmp(line, "sudo make me a sandwich") == 0) {
        terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
        terminal_writestring("Okay.\n");
        terminal_setcolor(VGA_LIGHT_BROWN, VGA_BLACK);
        terminal_writestring("   .\"\"\"\"\"\"\"\".\n  /  ~~~~~~~~ \\\n |  == == == ==  |\n  \\  ~~~~~~~~ /\n   \"........\"\n");
        terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
        terminal_writestring("Sandwich served. Enjoy your byte-sized meal.\n");
    } else {
        terminal_writestring("Unknown command: ");
        terminal_writestring(line);
        terminal_writestring("\n");
        terminal_writestring("Type 'help' for available commands.\n");
    }
}

void naoixsh_init(void) {
    terminal_setcolor(VGA_LIGHT_CYAN, VGA_BLACK);
    terminal_writestring("NaoixOS");
    terminal_setcolor(VGA_LIGHT_BROWN, VGA_BLACK);
    terminal_writestring(" " KEVER "\n");
    terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
    terminal_writestring("Running embedded shell NaoixSH...\n");
    terminal_writestring("NaoixSH");
    terminal_setcolor(VGA_RED, VGA_BLACK);
    terminal_writestring(" " SHVER);
    terminal_setcolor(VGA_LIGHT_GREEN, VGA_BLACK);
    terminal_writestring(" - type 'help'\n");
    terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
}

void naoixsh_run(void) {
    char line[LINE_MAX];
    for (;;) {
        if (dev_mode) {
            terminal_setcolor(VGA_RED, VGA_BLACK);
            terminal_writestring("root");
            terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
            terminal_writestring("@naoixos # ");
        } else {
            terminal_setcolor(VGA_LIGHT_RED, VGA_BLACK);
            terminal_writestring("naoixos > ");
        }
        terminal_setcolor(VGA_LIGHT_GREY, VGA_BLACK);
        readline(line, LINE_MAX);
        run_command(line);
    }
}
