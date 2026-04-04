#ifdef _WIN32
#include <windows.h>
#ifdef BACKGROUND_RED
#undef BACKGROUND_RED
#endif
#ifdef BACKGROUND_GREEN
#undef BACKGROUND_GREEN
#endif
#ifdef BACKGROUND_BLUE
#undef BACKGROUND_BLUE
#endif
#endif

#include "text_utils.h"
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
static void ensure_terminal_ready(void)
{
    static int initialized = 0;
    if (initialized)
        return;

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode))
        {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, mode);
        }
    }

    SetConsoleOutputCP(CP_UTF8);
    initialized = 1;
}
#endif

/* charsets de borda (UTF-8) */
static const char *SIMPLE_BORDER[8] = {
    "┌", "└", "│", "┐", "│", "┘", "─", "─"};
static const char *HEAVY_BORDER[8] = {
    "┏", "┗", "┃", "┓", "┃", "┛", "━", "━"};
static const char *DOUBLE_BORDER[8] = {
    "╔", "╚", "║", "╗", "║", "╝", "═", "═"};
static const char *BROAD_BORDER[8] = {
    "▛", "▙", "▌", "▜", "▐", "▟", "▀", "▄"};
static const char *ROUND_BORDER[8] = {
    "╭", "╰", "│", "╮", "│", "╯", "─", "─"};

/* impressão colorida */

void print_colored(const char *s, const char *color)
{
#ifdef _WIN32
    ensure_terminal_ready();
#endif
    if (!color)
        color = WHITE;
    fputs(color, stdout);
    fputs(s, stdout);
    fputs(RESET, stdout);
}

void print_colored_end(const char *s, const char *color, const char *end)
{
#ifdef _WIN32
    ensure_terminal_ready();
#endif
    if (!end)
        end = "\n";
    if (!color)
        color = WHITE;
    fputs(color, stdout);
    fputs(s, stdout);
    fputs(RESET, stdout);
    fputs(end, stdout);
}

void print_red(const char *s) { print_colored_end(s, RED, "\n"); }
void print_green(const char *s) { print_colored_end(s, GREEN, "\n"); }
void print_yellow(const char *s) { print_colored_end(s, YELLOW, "\n"); }
void print_blue(const char *s) { print_colored_end(s, BLUE, "\n"); }

/* cursor */

void gotoxy(int x, int y)
{
#ifdef _WIN32
    ensure_terminal_ready();
#endif
    if (x < 1)
        x = 1;
    if (y < 1)
        y = 1;
    printf("\x1b[%d;%dH", y, x);
}

/* trim */

void trim_str(char *dst, const char *src, int size)
{
    if (size <= 0)
    {
        if (dst)
            dst[0] = '\0';
        return;
    }
    int len = (int)strlen(src);
    if (len <= size)
    {
        strncpy(dst, src, size);
        dst[size] = '\0';
        return;
    }
    if (size <= 1)
    { /* não cabe nem o texto + … */
        dst[0] = '\0';
        return;
    }
    strncpy(dst, src, size - 1);
    dst[size - 1] = '\0';
    /* substitui último caractere por reticências simples */
    dst[size - 2] = '.';
    dst[size - 3] = '.';
    dst[size - 4] = '.';
}

/* limpar tela */

void clear_screen(void)
{
#ifdef _WIN32
    ensure_terminal_ready();
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cell_count;
    COORD home_coords = {0, 0};

    if (!GetConsoleScreenBufferInfo(hOut, &csbi))
        return;

    cell_count = (DWORD)(csbi.dwSize.X * csbi.dwSize.Y);

    FillConsoleOutputCharacter(hOut, (TCHAR)' ', cell_count, home_coords, &count);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, cell_count, home_coords, &count);
    SetConsoleCursorPosition(hOut, home_coords);
#else
    system("clear");
#endif
}

/* desenho de caixas */

void draw_box(int x, int y, int w, int h, const char *color, const char *charset[8])
{
    if (w <= 1 || h <= 1)
        return;
    if (!color)
        color = WHITE;
    if (!charset)
        charset = SIMPLE_BORDER;

    for (int i = x; i < x + w; ++i)
    {
        for (int j = y; j < y + h; ++j)
        {
            gotoxy(i, j);
            fputs(color, stdout);
            if (i == x)
            {
                if (j == y)
                {
                    fputs(charset[0], stdout); /* canto sup. esq */
                }
                else if (j == y + h - 1)
                {
                    fputs(charset[1], stdout); /* canto inf. esq */
                }
                else
                {
                    fputs(charset[2], stdout); /* lateral esq */
                }
            }
            else if (i == x + w - 1)
            {
                if (j == y)
                {
                    fputs(charset[3], stdout); /* canto sup. dir */
                }
                else if (j < y + h - 1)
                {
                    fputs(charset[4], stdout); /* lateral dir */
                }
                else
                {
                    fputs(charset[5], stdout); /* canto inf. dir */
                }
            }
            else
            {
                if (j == y)
                {
                    fputs(charset[6], stdout); /* topo */
                }
                else if (j == y + h - 1)
                {
                    fputs(charset[7], stdout); /* base */
                }
                else
                {
                    fputs(" ", stdout);
                }
            }
            fputs(RESET, stdout);
        }
    }
}

void draw_simple_box(int x, int y, int w, int h, const char *color)
{
    draw_box(x, y, w, h, color, SIMPLE_BORDER);
}
void draw_heavy_box(int x, int y, int w, int h, const char *color)
{
    draw_box(x, y, w, h, color, HEAVY_BORDER);
}
void draw_double_box(int x, int y, int w, int h, const char *color)
{
    draw_box(x, y, w, h, color, DOUBLE_BORDER);
}
void draw_broad_box(int x, int y, int w, int h, const char *color)
{
    draw_box(x, y, w, h, color, BROAD_BORDER);
}
void draw_round_box(int x, int y, int w, int h, const char *color)
{
    draw_box(x, y, w, h, color, ROUND_BORDER);
}

/* grade de tabela */

void draw_table_grid(int x, int y, int cols, int rows, int cell_width, const char *color)
{
    if (!color)
        color = WHITE;
    int render_width = (cols + 1) + (cols * cell_width);
    int render_height = (rows + 1) + rows;

    draw_simple_box(x, y, render_width, render_height, color);

    /* interseções verticais nas bordas superior/inferior */
    for (int i = 1; i < cols; ++i)
    {
        int rx = x + (i * cell_width) + i;
        gotoxy(rx, y);
        print_colored("┬", color);
        gotoxy(rx, y + render_height - 1);
        print_colored("┴", color);
    }

    /* laterais esquerda/direita das linhas internas */
    for (int j = 1; j < rows; ++j)
    {
        int ry = y + j + j;
        gotoxy(x, ry);
        print_colored("├", color);
        gotoxy(x + render_width - 1, ry);
        print_colored("┤", color);
    }

    /* linhas verticais internas */
    for (int i = 1; i < cols; ++i)
    {
        int rx = x + (i * cell_width) + i;
        for (int j = y + 1; j < y + render_height - 1; ++j)
        {
            gotoxy(rx, j);
            print_colored("│", color);
        }
    }

    /* linhas horizontais internas */
    for (int j = 1; j < rows; ++j)
    {
        int ry = y + j + j;
        for (int i = x + 1; i < x + render_width - 1; ++i)
        {
            gotoxy(i, ry);
            print_colored("─", color);
        }
    }

    /* cruzetas */
    for (int i = 1; i < cols; ++i)
    {
        int rx = x + (i * cell_width) + i;
        for (int j = 1; j < rows; ++j)
        {
            int ry = y + j + j;
            gotoxy(rx, ry);
            print_colored("┼", color);
        }
    }
}

/* alinhamento */

void print_aligned(const char *text, int width, int alignment, const char *color)
{
    if (!color)
        color = WHITE;
    char buf[512];
    if ((int)strlen(text) > width && width < (int)sizeof(buf) - 4)
    {
        strncpy(buf, text, width - 1);
        buf[width - 1] = '\0';
        strcat(buf, "…");
        text = buf;
    }

    int len = (int)strlen(text);
    int left_pad = 0;
    int right_pad = 0;

    if (alignment == ALIGN_LEFT)
    {
        right_pad = width - len;
    }
    else if (alignment == ALIGN_RIGHT)
    {
        left_pad = width - len;
    }
    else
    { /* center */
        left_pad = (width - len) / 2;
        right_pad = width - len - left_pad;
    }

    char line[1024];
    int pos = 0;
    for (int i = 0; i < left_pad && pos < (int)sizeof(line) - 1; ++i)
        line[pos++] = ' ';
    for (int i = 0; i < len && pos < (int)sizeof(line) - 1; ++i)
        line[pos++] = text[i];
    for (int i = 0; i < right_pad && pos < (int)sizeof(line) - 1; ++i)
        line[pos++] = ' ';
    line[pos] = '\0';

    print_colored(line, color);
}

/* tabela sem cabeçalho */

void draw_table(int x, int y, int cell_width,
                int rows, int cols,
                const char *data[rows][cols],
                const char *grid_color,
                const char *data_color,
                const int *alignments)
{

    if (!grid_color)
        grid_color = WHITE;
    if (!data_color)
        data_color = WHITE;

    int render_width = (cols + 1) + (cols * cell_width);
    int render_height = (rows + 1) + rows;
    /* limites fixos similares ao Python (120x30) – pode ajustar */
    if (render_width > 120 - x || render_height > 30 - y)
    {
        print_error("Table size exceeds console limits...");
        return;
    }

    draw_table_grid(x, y, cols, rows, cell_width, grid_color);

    for (int j = 0; j < rows; ++j)
    {
        for (int i = 0; i < cols; ++i)
        {
            const char *cell = data[j][i] ? data[j][i] : "";
            int text_x = x + (i * cell_width) + i + 1;
            int text_y = y + j + j + 1;
            gotoxy(text_x, text_y);
            int align = ALIGN_LEFT;
            if (alignments && i < cols)
                align = alignments[i];
            print_aligned(cell, cell_width, align, data_color);
        }
    }
}

/* tabela com cabeçalho */

void draw_table_with_headers(int x, int y, int cell_width, int cols, const char *headers[], int rows, const char *data[rows][cols], const char *grid_color, const char *data_color, const char *header_color, const int *alignments)
{
    if (!grid_color)
        grid_color = WHITE;
    if (!data_color)
        data_color = WHITE;
    if (!header_color)
        header_color = BOLD_WHITE;

    int total_rows = rows + 1;
    int render_width = (cols + 1) + (cols * cell_width);
    int render_height = (total_rows + 1) + total_rows;
    if (render_width > 120 - x || render_height > 30 - y)
    {
        print_error("Table size exceeds console limits...");
        return;
    }

    draw_table_grid(x, y, cols, total_rows, cell_width, grid_color);

    /* cabeçalho */
    for (int i = 0; i < cols; ++i)
    {
        const char *cell = headers[i] ? headers[i] : "";
        int text_x = x + (i * cell_width) + i + 1;
        int text_y = y + 1;
        gotoxy(text_x, text_y);
        int align = ALIGN_LEFT;
        if (alignments && i < cols)
            align = alignments[i];
        print_aligned(cell, cell_width, align, header_color);
    }

    /* dados */
    for (int j = 0; j < rows; ++j)
    {
        for (int i = 0; i < cols; ++i)
        {
            const char *cell = data[j][i] ? data[j][i] : "";
            int text_x = x + (i * cell_width) + i + 1;
            int text_y = y + j + j + 3;
            gotoxy(text_x, text_y);
            int align = ALIGN_LEFT;
            if (alignments && i < cols)
                align = alignments[i];
            print_aligned(cell, cell_width, align, data_color);
        }
    }
}

/* barras de título */

static void _print_title_generic(const char *text, const char *box_color, const char *text_color, const char *charset[8])
{
    if (!box_color)
        box_color = WHITE;
    if (!text_color)
        text_color = WHITE;
    int console_width = 120;
    int x = console_width / 2 - (int)strlen(text) / 2;

    draw_box(1, 1, console_width, 3, box_color, charset);
    gotoxy(x, 2);
    print_colored(text, text_color);
}

void print_title_bar(const char *text, const char *box_color, const char *text_color)
{
    _print_title_generic(text, box_color, text_color, SIMPLE_BORDER);
}
void print_title_bar_double(const char *text, const char *box_color, const char *text_color)
{
    _print_title_generic(text, box_color, text_color, DOUBLE_BORDER);
}
void print_title_bar_broad(const char *text, const char *box_color, const char *text_color)
{
    _print_title_generic(text, box_color, text_color, BROAD_BORDER);
}
void print_title_bar_heavy(const char *text, const char *box_color, const char *text_color)
{
    _print_title_generic(text, box_color, text_color, HEAVY_BORDER);
}
void print_title_bar_round(const char *text, const char *box_color, const char *text_color)
{
    _print_title_generic(text, box_color, text_color, ROUND_BORDER);
}

/* barra de progresso */

void print_progress_bar(double percentage, int line, const char *box_color, const char *bar_color, const char *text_color, const char *background_text_color)
{
    if (line < 0 || line > 27)
        return;
    if (percentage < 0)
        percentage = 0;
    if (percentage > 100)
        percentage = 100;

    if (!box_color)
        box_color = WHITE;
    if (!bar_color)
        bar_color = GREEN;
    if (!text_color)
        text_color = WHITE;
    if (!background_text_color)
        background_text_color = BACKGROUND_GREEN;

    int console_width = 120;
    draw_box(1, line, console_width, 3, box_color, SIMPLE_BORDER);

    int inner_width = console_width - 2;
    int pins = (int)((percentage / 100.0) * inner_width);

    for (int i = 0; i < pins; ++i)
    {
        gotoxy(2 + i, line + 1);
        print_colored("█", bar_color);
    }

    char buf[32];
    snprintf(buf, sizeof(buf), "%.2f%%", percentage);
    int text_len = (int)strlen(buf);
    int text_start = console_width / 2 - text_len / 2;

    for (int j = 0; j < text_len; ++j)
    {
        gotoxy(text_start + j, line + 1);
        if (pins < text_start + j - 1)
        {
            print_colored((char[]){buf[j], '\0'}, text_color);
        }
        else
        {
            /* texto sobre o fundo da barra */
            fputs(background_text_color, stdout);
            print_colored((char[]){buf[j], '\0'}, text_color);
            fputs(RESET, stdout);
        }
    }
}

/* status bar */

void print_status_bar(const char *text, const char *text_color)
{
    if (!text_color)
        text_color = WHITE;
    int console_width = 120;
    /* limpa linha 28 */
    gotoxy(1, 28);
    for (int i = 0; i < console_width; ++i)
        putchar(' ');
    /* desenha box */
    draw_box(1, 27, console_width, 3, DARK_GRAY, SIMPLE_BORDER);
    gotoxy(2, 28);
    print_colored(text, text_color);
}

void print_success(const char *text)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "SUCCESS - %s", text ? text : "");
    print_status_bar(buf, GREEN);
}
void print_info(const char *text)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "INFO - %s", text ? text : "");
    print_status_bar(buf, BRIGHT_CYAN);
}
void print_warning(const char *text)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "WARNING - %s", text ? text : "");
    print_status_bar(buf, YELLOW);
}
void print_error(const char *text)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "ERROR - %s", text ? text : "");
    print_status_bar(buf, RED);
}

/* campo de edição simples */

void draw_edit_field(const char *label, int line, const char *box_color, const char *text_color)
{
    if (!box_color)
        box_color = WHITE;
    if (!text_color)
        text_color = WHITE;
    draw_box(1, line, 20, 3, box_color, SIMPLE_BORDER);
    draw_box(21, line, 100, 3, box_color, SIMPLE_BORDER);
    gotoxy(2, line + 1);
    print_colored(label ? label : "", text_color);
}
