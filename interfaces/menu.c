#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <synchapi.h>
#include <sysinfoapi.h>
#include "text_utils.h"
#include "menu.h"
#include "../src/read_leaderboard.h"

enum MenuOption
{
    MENU_PLAY = 0,
    MENU_INSTRUCTIONS,
    MENU_LEADERBOARD,
    MENU_EXIT,
    MENU_COUNT
};

static const char *MENU_ITEMS[MENU_COUNT] = {
    "Jogar",
    "Instrucoes",
    "Tabela de Recordes",
    "Sair"};

static const int CONSOLE_WIDTH = 120;
static const int MENU_ITEM_WIDTH = 24;
static const long MESSAGE_DURATION_MS = 3000;
static const int MENU_Y = 8;

static const int KEY_UP = -1;
static const int KEY_DOWN = 1;
static const int KEY_ENTER = 0;
static const int KEY_NONE = 99;

static long long current_millis(void)
{
    return (long long)GetTickCount64();
}

static int read_key_nonblocking(void)
{
    if (!_kbhit())
        return KEY_NONE;

    int ch = _getch();
    if (ch == 224 || ch == 0)
    {
        int ext = _getch();
        if (ext == 72)
            return KEY_UP;
        if (ext == 80)
            return KEY_DOWN;
    }
    if (ch == '\r')
        return KEY_ENTER;
    return KEY_NONE;
}

static void draw_menu_frame(void)
{
    int menu_x = (CONSOLE_WIDTH - MENU_ITEM_WIDTH) / 2;
    const char *hint = "Use as setas para navegar e Enter para selecionar.";
    int hint_x = (CONSOLE_WIDTH - (int)strlen(hint)) / 2;

    clear_screen();
    print_title_bar("LuckyGuess - Menu Principal", BOLD_BLUE, BOLD_WHITE);

    for (int i = 0; i < MENU_COUNT; ++i)
    {
        gotoxy(menu_x, MENU_Y + i * 2);
        printf("  %-20s  ", MENU_ITEMS[i]);
    }

    gotoxy(hint_x, MENU_Y + MENU_COUNT * 2 + 2);
    print_colored(hint, DARK_GRAY);
    fflush(stdout);
}

static void draw_menu_options(int selected, int show_message_for)
{
    int menu_x = (CONSOLE_WIDTH - MENU_ITEM_WIDTH) / 2;

    for (int i = 0; i < MENU_COUNT; ++i)
    {
        gotoxy(menu_x, MENU_Y + i * 2);
        if (i == selected)
        {
            printf(BACKGROUND_WHITE BOLD_BLUE "  %-20s  " RESET, MENU_ITEMS[i]);
        }
        else
        {
            printf("  %-20s  ", MENU_ITEMS[i]);
        }

        if (i == show_message_for && i != MENU_EXIT)
        {
            printf("  " YELLOW "Em breve..." RESET);
        }
        else
        {
            printf("              ");
        }
    }
    fflush(stdout);
}

static void show_goodbye(void)
{
    const char *goodbye_text = "Até a próxima! =D";
    int text_x = (CONSOLE_WIDTH - (int)strlen(goodbye_text)) / 2;

    clear_screen();
    print_title_bar("LuckyGuess - Menu Principal", BOLD_BLUE, BOLD_WHITE);
    gotoxy(text_x, 12);
    print_colored(goodbye_text, BOLD_YELLOW);
    Sleep(3000);
}

int run_main_menu(void)
{
    int selected = 0;
    int show_message_for = -1;
    long long message_until = 0;
    int last_drawn_selected = -1;
    int last_drawn_message = -2;
    int running = 1;

    draw_menu_frame();

    while (running)
    {
        int dirty = 0;

        if (show_message_for != -1 && current_millis() >= message_until)
        {
            show_message_for = -1;
            dirty = 1;
        }

        int key = read_key_nonblocking();
        if (key == KEY_UP)
        {
            selected = (selected - 1 + MENU_COUNT) % MENU_COUNT;
            dirty = 1;
        }
        else if (key == KEY_DOWN)
        {
            selected = (selected + 1) % MENU_COUNT;
            dirty = 1;
        }
        else if (key == KEY_ENTER)
        {
            if (selected == MENU_EXIT)
            {
                show_goodbye();
                running = 0;
            }
            else if (selected == MENU_LEADERBOARD)
            {
                display_leaderboard();
                draw_menu_frame();
                dirty = 1;
            }
            else
            {
                show_message_for = selected;
                message_until = current_millis() + MESSAGE_DURATION_MS;
                dirty = 1;
            }
        }

        if (dirty || selected != last_drawn_selected || show_message_for != last_drawn_message)
        {
            draw_menu_options(selected, show_message_for);
            last_drawn_selected = selected;
            last_drawn_message = show_message_for;
        }

        Sleep(16);
    }

    clear_screen();
    return 0;
}
