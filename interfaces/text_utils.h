#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

#include <stdio.h>

/* Cores ANSI */

// Texto normal
#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define LIGHT_GRAY "\x1b[37m"
#define DARK_GRAY "\x1b[90m"
#define BRIGHT_RED "\x1b[91m"
#define BRIGHT_GREEN "\x1b[92m"
#define BRIGHT_YELLOW "\x1b[93m"
#define BRIGHT_BLUE "\x1b[94m"
#define BRIGHT_MAGENTA "\x1b[95m"
#define BRIGHT_CYAN "\x1b[96m"
#define WHITE "\x1b[97m"

// Texto em negrito
#define BOLD_BLACK "\x1b[1;30m"
#define BOLD_RED "\x1b[1;31m"
#define BOLD_GREEN "\x1b[1;32m"
#define BOLD_YELLOW "\x1b[1;33m"
#define BOLD_BLUE "\x1b[1;34m"
#define BOLD_MAGENTA "\x1b[1;35m"
#define BOLD_CYAN "\x1b[1;36m"
#define BOLD_LIGHT_GRAY "\x1b[1;37m"
#define BOLD_DARK_GRAY "\x1b[1;90m"
#define BOLD_BRIGHT_RED "\x1b[1;91m"
#define BOLD_BRIGHT_GREEN "\x1b[1;92m"
#define BOLD_BRIGHT_YELLOW "\x1b[1;93m"
#define BOLD_BRIGHT_BLUE "\x1b[1;94m"
#define BOLD_BRIGHT_MAGENTA "\x1b[1;95m"
#define BOLD_BRIGHT_CYAN "\x1b[1;96m"
#define BOLD_WHITE "\x1b[1;97m"

// Background
#define BACKGROUND_GRAY "\x1b[40m"
#define BACKGROUND_RED "\x1b[41m"
#define BACKGROUND_GREEN "\x1b[42m"
#define BACKGROUND_YELLOW "\x1b[43m"
#define BACKGROUND_BLUE "\x1b[44m"
#define BACKGROUND_MAGENTA "\x1b[45m"
#define BACKGROUND_CYAN "\x1b[46m"
#define BACKGROUND_LIGHT_GRAY "\x1b[47m"
#define BACKGROUND_DARK_GRAY "\x1b[100m"
#define BACKGROUND_BRIGHT_RED "\x1b[101m"
#define BACKGROUND_BRIGHT_GREEN "\x1b[102m"
#define BACKGROUND_BRIGHT_YELLOW "\x1b[103m"
#define BACKGROUND_BRIGHT_BLUE "\x1b[104m"
#define BACKGROUND_BRIGHT_MAGENTA "\x1b[105m"
#define BACKGROUND_BRIGHT_CYAN "\x1b[106m"
#define BACKGROUND_WHITE "\x1b[107m"

#define RESET "\x1b[0m"

/* Alinhamento */
#define ALIGN_LEFT 0
#define ALIGN_CENTER 1
#define ALIGN_RIGHT 2

/* Prototipos */

// impressão colorida
void print_colored(const char *s, const char *color);
void print_colored_end(const char *s, const char *color, const char *end);

// atalho de cores
void print_red(const char *s);
void print_green(const char *s);
void print_yellow(const char *s);
void print_blue(const char *s);

// cursor
void gotoxy(int x, int y);

// utilidades de string
void trim_str(char *dst, const char *src, int size);

// limpar tela
void clear_screen(void);

// desenho de caixas
void draw_box(int x, int y, int w, int h, const char *color, const char *charset[8]);

void draw_simple_box(int x, int y, int w, int h, const char *color);
void draw_heavy_box(int x, int y, int w, int h, const char *color);
void draw_double_box(int x, int y, int w, int h, const char *color);
void draw_broad_box(int x, int y, int w, int h, const char *color);
void draw_round_box(int x, int y, int w, int h, const char *color);

// tabelas
void draw_table_grid(int x, int y, int cols, int rows, int cell_width, const char *color);
void print_aligned(const char *text, int width, int alignment, const char *color);
void draw_table(int x, int y, int cell_width,
                int rows, int cols,
                const char *data[rows][cols],
                const char *grid_color,
                const char *data_color,
                const int *alignments); // pode ser NULL

void draw_table_with_headers(int x, int y, int cell_width,
                             int cols,
                             const char *headers[],
                             int rows,
                             const char *data[rows][cols],
                             const char *grid_color,
                             const char *data_color,
                             const char *header_color,
                             const int *alignments);

// barra de título
void print_title_bar(const char *text,
                     const char *box_color,
                     const char *text_color);
void print_title_bar_double(const char *text,
                            const char *box_color,
                            const char *text_color);
void print_title_bar_broad(const char *text,
                           const char *box_color,
                           const char *text_color);
void print_title_bar_heavy(const char *text,
                           const char *box_color,
                           const char *text_color);
void print_title_bar_round(const char *text,
                           const char *box_color,
                           const char *text_color);

// barra de progresso
void print_progress_bar(double percentage,
                        int line,
                        const char *box_color,
                        const char *bar_color,
                        const char *text_color,
                        const char *background_text_color);

// status bar
void print_status_bar(const char *text, const char *text_color);
void print_success(const char *text);
void print_info(const char *text);
void print_warning(const char *text);
void print_error(const char *text);

// campos de edição (simples)
void draw_edit_field(const char *label, int line,
                     const char *box_color,
                     const char *text_color);

#endif /* TEXT_UTILS_H */
