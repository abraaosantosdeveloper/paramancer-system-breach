#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "../interfaces/text_utils.h"
#include "read_leaderboard.h"

#define MAX_NOME 50
#define LEADERBOARD_FILE "jogadores.csv"

typedef struct
{
    int id;
    char nome[MAX_NOME];
    int pontos;
} Jogador;

void display_leaderboard(void)
{
    Jogador jogadores[10];
    int count = 0;

    FILE *file = fopen(LEADERBOARD_FILE, "r");
    if (!file)
    {
        clear_screen();
        print_title_bar("LuckyGuess - Tabela de Recordes", BOLD_BLUE, BOLD_WHITE);
        gotoxy(15, 10);
        print_colored("Erro ao abrir arquivo de recordes.", RED);
        gotoxy(15, 12);
        print_colored("Pressione qualquer tecla para voltar...", DARK_GRAY);
        fflush(stdout);
        _getch();
        return;
    }

    while (count < 10 && fscanf(file, "%d,%49[^,],%d", &jogadores[count].id, jogadores[count].nome, &jogadores[count].pontos) == 3)
    {
        count++;
    }
    fclose(file);

    clear_screen();
    print_title_bar("LuckyGuess - Tabela de Recordes", BOLD_BLUE, BOLD_WHITE);

    const char *headers[] = {"Pos", "Nome", "Pts"};
    int cell_width_1 = 5;
    int cell_width_2 = 28;
    int cell_width_3 = 8;
    int total_width = cell_width_1 + cell_width_2 + cell_width_3 + 6;
    int table_x = (120 - total_width) / 2;

    int pos_y = 6;

    gotoxy(table_x, pos_y);
    print_colored("Pos", BOLD_WHITE);
    gotoxy(table_x + 6, pos_y);
    print_colored("Nome", BOLD_WHITE);
    gotoxy(table_x + 38, pos_y);
    print_colored("Pts", BOLD_WHITE);

    gotoxy(table_x, pos_y + 1);
    print_colored("---  -----------------------------  -------", DARK_GRAY);

    for (int i = 0; i < count; ++i)
    {
        int row_y = pos_y + 2 + i;

        /* coluna de posição */
        gotoxy(table_x, row_y);
        printf("%2d.", i + 1);

        /* coluna de nome */
        gotoxy(table_x + 6, row_y);
        printf("%s", jogadores[i].nome);

        /* coluna de pontos – alinhada pela coluna, não pelo tamanho do nome */
        gotoxy(table_x + 37, row_y);
        printf("%6d", jogadores[i].pontos);
    }

    gotoxy(table_x, pos_y + 2 + count + 2);
    print_colored("Pressione qualquer tecla para voltar...", DARK_GRAY);
    fflush(stdout);
    _getch();
}
