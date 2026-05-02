#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include "historico.h"

// Simple state machine for the main menu flow.
enum gameState
{
    idle,         // Menu idle state.
    playing,      // Blink animation for locked items.
    viewingStats, // Stats screen is active.
    gameOver      // Exit requested.
};

// Layout data for the stats table UI.
typedef struct
{
    Rectangle container; // Outer bounds of the table.
    Rectangle headerRow; // Header bar bounds.
    Rectangle rowsClip;  // Clip region for rows.
    Vector2 titleSize;   // Measured title text size.
    Vector2 footerSize;  // Measured footer text size.
    float rowHeight;     // Height of each row.
    float innerPadding;  // Padding inside the table.
    int titleFontSize;   // Title font size.
    int headerFontSize;  // Header font size.
    int rowFontSize;     // Row font size.
    int footerFontSize;  // Footer font size.
} StatsLayout;

// UI palette for text fill and outline.
static const Color TEXT_COLOR = {0xFF, 0xFF, 0xFF, 0xFF};
static const Color OUTLINE_COLOR = {0x0A, 0x0F, 0x1F, 0xFF};
static const Color EM_BREVE_COLOR = {0xFF, 0xFF, 0x00, 0xFF}; // Amarelo para "em breve"

// Builds a compact set of glyphs to keep the font atlas small.
static int build_font_codepoints(int *codepoints, int capacity)
{
    int count = 0;
    // Keep the atlas small: ASCII plus the Portuguese glyphs we actually render.
    for (int c = 32; c <= 126 && count < capacity; c++)
    {
        codepoints[count++] = c;
    }

    // Extra glyphs needed for Portuguese.
    const int extras[] = {
        0x00BA,
        0x00C1,
        0x00E1,
        0x00C9,
        0x00E9,
        0x00CD,
        0x00ED,
        0x00D3,
        0x00F3,
        0x00DA,
        0x00FA,
        0x00C7,
        0x00E7,
        0x00C3,
        0x00E3,
        0x00D5,
        0x00F5,
        0x00CA,
        0x00EA,
        0x00D4,
        0x00F4};

    // Append extra glyphs while capacity allows.
    int extrasCount = (int)(sizeof(extras) / sizeof(extras[0]));
    for (int i = 0; i < extrasCount && count < capacity; i++)
    {
        codepoints[count++] = extras[i];
    }

    return count;
}

// Loads a font and falls back to the default if it fails.
static Font load_ui_font_from_path(const char *path, int baseSize, bool *outIsDefault)
{
    // Build the codepoint list and try to load the font.
    int codepoints[128];
    int count = build_font_codepoints(codepoints, (int)(sizeof(codepoints) / sizeof(codepoints[0])));
    Font font = LoadFontEx(path, baseSize, codepoints, count);
    // Fall back to the default font if loading failed.
    if (font.baseSize <= 0)
    {
        if (outIsDefault)
        {
            *outIsDefault = true;
        }
        return GetFontDefault();
    }

    if (outIsDefault)
    {
        *outIsDefault = false;
    }
    return font;
}

// Loads the menu textures used in the background and mascot.
static int load_menu_textures(Texture2D *backgroundTexture, Texture2D *logoTexture)
{
    // Load background, logo, and mascot textures.
    *backgroundTexture = LoadTexture("assets/game_arts/scenario/scenario.png");
    *logoTexture = LoadTexture("assets/game_arts/Logo_no-bg.png");

    // Guard against partial loads and release any successful handles.
    if (backgroundTexture->id == 0 || logoTexture->id == 0)
    {
        // Se algo falhar, descarrega o que foi aberto para evitar vazamentos.
        if (backgroundTexture->id != 0)
        {
            UnloadTexture(*backgroundTexture);
        }
        if (logoTexture->id != 0)
        {
            UnloadTexture(*logoTexture);
        }
        return 0;
    }

    return 1;
}

// Releases the menu textures loaded at startup.
static void unload_menu_textures(Texture2D backgroundTexture, Texture2D logoTexture)
{
    // Free all menu textures.
    UnloadTexture(backgroundTexture);
    UnloadTexture(logoTexture);
}

// Converts CSV timestamps to the on-screen format.
static void format_timestamp(const char *input, char *output, size_t output_size)
{
    // Parse the timestamp components.
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;

    // Normalize the CSV timestamp to the on-screen format.
    if (input && sscanf(input, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) == 6)
    {
        snprintf(output, output_size, "%02d/%02d/%04d - %02d:%02d", day, month, year, hour, minute);
        return;
    }

    // If output is empty, bail out.
    if (output_size == 0)
    {
        return;
    }

    // Preserve the input when parsing fails.
    if (input)
    {
        strncpy(output, input, output_size - 1);
        output[output_size - 1] = '\0';
        return;
    }

    output[0] = '\0';
}

// Computes positions and sizes for the stats table container.
static StatsLayout build_stats_layout(Font titleFont, Font bodyFont, int screenWidth, int screenHeight)
{
    StatsLayout layout = {0};
    const int padding = 24;
    const float rowSpacing = 6.0f;
    const char *title = "Estatísticas";
    const char *footer = "Esc para voltar";

    // Font sizes and padding for the stats table.
    layout.titleFontSize = 22;
    layout.headerFontSize = 16;
    layout.rowFontSize = 16;
    layout.footerFontSize = 16;
    layout.innerPadding = 12.0f;

    // Measure text sizes for layout calculations.
    layout.titleSize = MeasureTextEx(titleFont, title, (float)layout.titleFontSize, 1.0f);
    layout.footerSize = MeasureTextEx(bodyFont, footer, (float)layout.footerFontSize, 1.0f);

    // Compute the table bounds with minimum height.
    float tableY = (float)padding + layout.titleSize.y + 12.0f;
    float tableHeight = (float)screenHeight - (float)(padding * 2) - layout.titleSize.y - layout.footerSize.y - 32.0f;
    if (tableHeight < 120.0f)
    {
        tableHeight = 120.0f;
    }

    // Position the main container.
    layout.container = (Rectangle){(float)padding, tableY, (float)screenWidth - (float)(padding * 2), tableHeight};

    // Compute the header row rectangle.
    float headerHeight = (float)layout.rowFontSize + 8.0f;
    layout.headerRow = (Rectangle){
        layout.container.x + 2.0f,
        layout.container.y + 2.0f,
        layout.container.width - 4.0f,
        headerHeight + layout.innerPadding};

    // Compute row height and the clip region for rows.
    layout.rowHeight = (float)layout.rowFontSize + rowSpacing;
    layout.rowsClip = (Rectangle){
        layout.container.x + layout.innerPadding,
        layout.container.y + layout.innerPadding + headerHeight + 6.0f,
        layout.container.width - layout.innerPadding * 2.0f,
        layout.container.y + layout.container.height - layout.innerPadding - (layout.container.y + layout.innerPadding + headerHeight + 6.0f)};

    // Ensure at least one row fits.
    if (layout.rowsClip.height < layout.rowHeight)
    {
        layout.rowsClip.height = layout.rowHeight;
    }

    return layout;
}

// Draws text with a simple outline by rendering offsets first.
static void draw_text_outlined(Font font,
                               const char *text,
                               Vector2 position,
                               float fontSize,
                               float spacing,
                               Color fill,
                               Color outline,
                               int thickness)
{
    // Draw outline first by offsetting the text.
    for (int dy = -thickness; dy <= thickness; dy++)
    {
        for (int dx = -thickness; dx <= thickness; dx++)
        {
            if (dx == 0 && dy == 0)
            {
                continue;
            }
            DrawTextEx(font,
                       text,
                       (Vector2){position.x + (float)dx, position.y + (float)dy},
                       fontSize,
                       spacing,
                       outline);
        }
    }

    // Draw the fill text on top.
    DrawTextEx(font, text, position, fontSize, spacing, fill);
}

// Draws centered text inside a rectangle with outline and clipping.
static void draw_text_clipped_centered(Font font,
                                       const char *text,
                                       Rectangle bounds,
                                       float fontSize,
                                       float spacing,
                                       Color fill,
                                       Color outline,
                                       int thickness)
{
    // Center the text in the given bounds.
    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);
    float textX = bounds.x + (bounds.width - textSize.x) * 0.5f;
    float textY = bounds.y + (bounds.height - textSize.y) * 0.5f;

    // Clamp to the bounds if the text is larger than the cell.
    if (textX < bounds.x)
    {
        textX = bounds.x;
    }
    if (textY < bounds.y)
    {
        textY = bounds.y;
    }

    // Clip to avoid drawing outside the cell.
    BeginScissorMode((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height);
    draw_text_outlined(font, text, (Vector2){textX, textY}, fontSize, spacing, fill, outline, thickness);
    EndScissorMode();
}

// Draws centered text inside a rectangle WITHOUT outline (para linhas da tabela).
static void draw_text_clipped_centered_no_outline(Font font,
                                                  const char *text,
                                                  Rectangle bounds,
                                                  float fontSize,
                                                  float spacing,
                                                  Color fill)
{
    // Center the text in the given bounds.
    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);
    float textX = bounds.x + (bounds.width - textSize.x) * 0.5f;
    float textY = bounds.y + (bounds.height - textSize.y) * 0.5f;

    // Clamp to the bounds if the text is larger than the cell.
    if (textX < bounds.x)
    {
        textX = bounds.x;
    }
    if (textY < bounds.y)
    {
        textY = bounds.y;
    }

    // Clip to avoid drawing outside the cell.
    BeginScissorMode((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height);
    DrawTextEx(font, text, (Vector2){textX, textY}, fontSize, spacing, fill);
    EndScissorMode();
}

// Draws the stats table and content with clipping.
static void draw_stats_screen(Font titleFont,
                              Font bodyFont,
                              const Session *sessions,
                              int sessionCount,
                              int scrollIndex,
                              int rowsPerPage,
                              int screenWidth,
                              int screenHeight)
{
    // Labels and column layout.
    const char *title = "Estatísticas";
    const char *footer = "Esc para voltar";
    const char *headers[] = {"timestamp", "alvo", "nº tentativas", "Palpites altos", "palpites baixos"};
    const float ratios[] = {0.34f, 0.10f, 0.17f, 0.19f, 0.20f};
    const int columnCount = (int)(sizeof(headers) / sizeof(headers[0]));

    // Compute layout and draw title.
    StatsLayout layout = build_stats_layout(titleFont, bodyFont, screenWidth, screenHeight);
    float titleX = (screenWidth - layout.titleSize.x) / 2.0f;
    draw_text_outlined(titleFont,
                       title,
                       (Vector2){titleX, 24.0f},
                       (float)layout.titleFontSize,
                       1.0f,
                       TEXT_COLOR,
                       OUTLINE_COLOR,
                       2);

    // Draw table container and header background.
    DrawRectangleRec(layout.container, Fade(BLACK, 0.6f));
    DrawRectangleLinesEx(layout.container, 2, Fade(WHITE, 0.8f));
    DrawRectangleRec(layout.headerRow, Fade(BLACK, 0.45f));

    // Draw column headers and vertical separators.
    float columnX = layout.container.x + layout.innerPadding;
    float availableWidth = layout.container.width - layout.innerPadding * 2.0f;
    float headerTextY = layout.container.y + layout.innerPadding;

    for (int i = 0; i < columnCount; i++)
    {
        float columnWidth = availableWidth * ratios[i];
        Rectangle headerCell = {columnX, headerTextY, columnWidth, (float)layout.headerFontSize + 6.0f};
        draw_text_clipped_centered(titleFont,
                                   headers[i],
                                   headerCell,
                                   (float)layout.headerFontSize,
                                   1.0f,
                                   WHITE,
                                   OUTLINE_COLOR,
                                   2);

        // Draw a column separator except for the last column.
        if (i < columnCount - 1)
        {
            int lineX = (int)(columnX + columnWidth);
            DrawLine(lineX, (int)layout.container.y + 2, lineX, (int)(layout.container.y + layout.container.height - 2), Fade(WHITE, 0.25f));
        }

        columnX += columnWidth;
    }

    // Show empty state or draw the visible rows.
    if (sessionCount <= 0)
    {
        const char *emptyText = "Nenhum historico encontrado.";
        Vector2 emptySize = MeasureTextEx(bodyFont, emptyText, (float)layout.rowFontSize, 1.0f);
        float emptyX = (screenWidth - emptySize.x) / 2.0f;
        float emptyY = layout.rowsClip.y + layout.rowHeight;
        draw_text_outlined(bodyFont,
                           emptyText,
                           (Vector2){emptyX, emptyY},
                           (float)layout.rowFontSize,
                           1.0f,
                           TEXT_COLOR,
                           OUTLINE_COLOR,
                           2);
    }
    else
    {
        // Clamp the row range to the session list.
        int start = scrollIndex;
        int end = start + rowsPerPage;
        if (end > sessionCount)
        {
            end = sessionCount;
        }

        // Clip da tabela para evitar texto vazando fora do container.
        BeginScissorMode((int)layout.rowsClip.x, (int)layout.rowsClip.y, (int)layout.rowsClip.width, (int)layout.rowsClip.height);
        for (int i = start; i < end; i++)
        {
            // Build row strings.
            char timeText[32];
            char line[256];
            format_timestamp(sessions[i].timestamp, timeText, sizeof(timeText));
            snprintf(line, sizeof(line), "%s", timeText);

            // Position the row and build each column value.
            float y = layout.rowsClip.y + (float)(i - start) * layout.rowHeight;
            float cellX = layout.container.x + layout.innerPadding;
            float available = layout.container.width - layout.innerPadding * 2.0f;

            const char *cells[] = {line, "", "", "", ""};
            char targetText[16];
            char attemptsText[16];
            char highText[16];
            char lowText[16];

            // Fill numeric columns.
            snprintf(targetText, sizeof(targetText), "%d", sessions[i].target);
            snprintf(attemptsText, sizeof(attemptsText), "%d", sessions[i].attempts_count);
            snprintf(highText, sizeof(highText), "%d", sessions[i].high_count);
            snprintf(lowText, sizeof(lowText), "%d", sessions[i].low_count);

            cells[1] = targetText;
            cells[2] = attemptsText;
            cells[3] = highText;
            cells[4] = lowText;

            // Draw all columns for the row.
            for (int c = 0; c < columnCount; c++)
            {
                float columnWidth = available * ratios[c];
                Rectangle cellRect = {cellX, y, columnWidth, layout.rowHeight};
                draw_text_clipped_centered_no_outline(bodyFont,
                                                      cells[c],
                                                      cellRect,
                                                      (float)layout.rowFontSize,
                                                      1.0f,
                                                      WHITE);
                cellX += columnWidth;
            }
        }
        EndScissorMode();
    }

    // Draw the footer hint.
    float footerX = (screenWidth - layout.footerSize.x) / 2.0f;
    float footerY = (float)screenHeight - 24.0f - layout.footerSize.y;
    draw_text_outlined(bodyFont,
                       footer,
                       (Vector2){footerX, footerY},
                       (float)layout.footerFontSize,
                       1.0f,
                       TEXT_COLOR,
                       OUTLINE_COLOR,
                       2);
}

// Runs the main menu loop, handling input, states, and rendering.
static void run_menu(Texture2D backgroundTexture,
                     Texture2D logoTexture,
                     Font uiFontBold,
                     Font uiFontLight,
                     int screenWidth,
                     int screenHeight, Music bgm)
{
    // Scale the logo to fit the menu layout.
    float logoScale = 1.0f;
    const float logoMaxWidth = 320.0f;
    if (logoTexture.width > (int)logoMaxWidth)
    {
        logoScale = logoMaxWidth / (float)logoTexture.width;
    }

    // Menu items and locked entries.
    const char *items[] = {"Jogar", "Estatísticas", "Sair"};
    const int itemCount = (int)(sizeof(items) / sizeof(items[0]));
    const int emBreveIndices[] = {0};
    const int emBreveCount = 1;

    // Layout constants for typography and mascot.
    const int fontSize = 32;
    const int emBreveFontSize = 20;
    const int itemSpacing = 56;
    const int emBreveSpacing = 16;
    const int highlightPaddingX = 10;
    const int highlightPaddingY = 6;

    // Compute vertical layout of logo and menu.
    int menuHeight = (itemCount - 1) * itemSpacing + fontSize;

    float logoWidth = logoTexture.width * logoScale;
    float logoHeight = logoTexture.height * logoScale;
    const float logoSpacing = 24.0f;

    // Center logo + menu block on screen.
    float logoY = (screenHeight - (logoHeight + logoSpacing + (float)menuHeight)) / 2.0f;
    if (logoY < 24.0f)
    {
        logoY = 24.0f;
    }
    float menuStartY = logoY + logoHeight + logoSpacing;

    // Selection and hover state.
    int selectedIndex = 0;
    int hoveredIndex = -1;

    // State machine variables.
    enum gameState state = idle;
    int blinkingIndex = -1;
    bool blinkVisible = true;
    int blinkTogglesRemaining = 0;
    float blinkTimer = 0.0f;
    const float blinkInterval = 0.2f;

    // Stats screen data cache.
    Session sessions[256];
    int sessionCount = 0;
    int statsScroll = 0;
    bool statsLoaded = false;

    // Main loop runs until user closes or selects exit.
    while (state != gameOver && !WindowShouldClose())
    {

        // Update audio stream
        UpdateMusicStream(bgm);
        // Read pointer position and reset hover state.
        Vector2 mouse = GetMousePosition();
        hoveredIndex = -1;

        int startY = (int)menuStartY;

        if (state != viewingStats)
        {
            // Menu input: hover, keyboard selection, and activation.
            for (int i = 0; i < itemCount; i++)
            {
                int itemY = startY + i * itemSpacing;
                Vector2 textSize = MeasureTextEx(uiFontBold, items[i], (float)fontSize, 1.0f);
                int itemX = (int)((screenWidth - textSize.x) / 2.0f);
                Rectangle hit = {(float)itemX, (float)itemY, textSize.x, (float)fontSize};
                // Update hover when mouse is over the item.
                if (CheckCollisionPointRec(mouse, hit))
                {
                    hoveredIndex = i;
                }
            }

            // Sync selection with hover when available.
            if (hoveredIndex >= 0)
            {
                selectedIndex = hoveredIndex;
            }

            // Keyboard navigation.
            if (IsKeyPressed(KEY_DOWN))
            {
                selectedIndex = (selectedIndex + 1) % itemCount;
            }
            if (IsKeyPressed(KEY_UP))
            {
                selectedIndex = (selectedIndex - 1 + itemCount) % itemCount;
            }

            // Activation via keyboard or mouse.
            bool activateSelected = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);
            if (hoveredIndex >= 0 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                activateSelected = true;
            }

            if (activateSelected)
            {
                // Switch the state based on the selected action.
                if (selectedIndex == 2)
                {
                    state = gameOver;
                }
                else if (selectedIndex == 1)
                {
                    state = viewingStats;
                    statsLoaded = false;
                }
                else
                {
                    blinkingIndex = selectedIndex;
                    blinkVisible = true;
                    blinkTogglesRemaining = 6;
                    blinkTimer = 0.0f;
                    state = playing;
                }
            }
        }
        else
        {
            // Exit stats view on common back keys.
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            {
                state = idle;
            }
        }

        if (state == viewingStats)
        {
            // Stats view: lazy-load data, handle scroll, and render.
            if (!statsLoaded)
            {
                // Carrega o historico ao entrar na tela para manter o menu leve.
                sessionCount = carregar_historico(sessions, (int)(sizeof(sessions) / sizeof(sessions[0])));
                statsScroll = 0;
                statsLoaded = true;
            }

            // Calculate rows per page based on the clip height.
            StatsLayout layout = build_stats_layout(uiFontBold, uiFontLight, screenWidth, screenHeight);
            int rowsPerPage = (int)(layout.rowsClip.height / layout.rowHeight);
            if (rowsPerPage < 1)
            {
                rowsPerPage = 1;
            }

            // Scroll input for the stats list.
            if (IsKeyPressed(KEY_DOWN))
            {
                statsScroll++;
            }
            if (IsKeyPressed(KEY_UP))
            {
                statsScroll--;
            }
            if (IsKeyPressed(KEY_PAGE_DOWN))
            {
                statsScroll += rowsPerPage;
            }
            if (IsKeyPressed(KEY_PAGE_UP))
            {
                statsScroll -= rowsPerPage;
            }

            // Clamp scroll to available rows.
            int maxScroll = sessionCount - rowsPerPage;
            if (maxScroll < 0)
            {
                maxScroll = 0;
            }
            if (statsScroll < 0)
            {
                statsScroll = 0;
            }
            if (statsScroll > maxScroll)
            {
                statsScroll = maxScroll;
            }

            // Render background and stats screen.
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw the backdrop and the stats table.
            Rectangle bgSrc = {0.0f, 0.0f, (float)backgroundTexture.width, (float)backgroundTexture.height};
            Rectangle bgDst = {0.0f, 0.0f, (float)screenWidth, (float)screenHeight};
            DrawTexturePro(backgroundTexture, bgSrc, bgDst, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

            draw_stats_screen(uiFontBold, uiFontLight, sessions, sessionCount, statsScroll, rowsPerPage, screenWidth, screenHeight);
            EndDrawing();
            continue;
        }

        if (state == playing)
        {
            // Blink animation for the "em breve" tag.
            blinkTimer += GetFrameTime();
            if (blinkTimer >= blinkInterval)
            {
                blinkTimer -= blinkInterval;
                blinkVisible = !blinkVisible;
                blinkTogglesRemaining--;
                if (blinkTogglesRemaining <= 0)
                {
                    state = idle;
                    blinkVisible = true;
                    blinkingIndex = -1;
                }
            }
        }

        // Render menu background and contents.
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw menu background and mascot.
        Rectangle bgSrc = {0.0f, 0.0f, (float)backgroundTexture.width, (float)backgroundTexture.height};
        Rectangle bgDst = {0.0f, 0.0f, (float)screenWidth, (float)screenHeight};
        DrawTexturePro(backgroundTexture, bgSrc, bgDst, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

        // Draw the game logo.
        float logoX = (screenWidth - logoWidth) / 2.0f;
        DrawTextureEx(logoTexture, (Vector2){logoX, logoY}, 0.0f, logoScale, WHITE);

        // Draw menu items and optional "em breve" labels.
        for (int i = 0; i < itemCount; i++)
        {
            int itemY = startY + i * itemSpacing;
            Vector2 textSize = MeasureTextEx(uiFontBold, items[i], (float)fontSize, 1.0f);
            int itemX = (int)((screenWidth - textSize.x) / 2.0f);
            // Draw highlight behind the selected item.
            if (i == selectedIndex)
            {
                Rectangle highlight = {
                    (float)(itemX - highlightPaddingX),
                    (float)(itemY - highlightPaddingY),
                    textSize.x + (float)(highlightPaddingX * 2),
                    (float)(fontSize + highlightPaddingY * 2)};
                DrawRectangleRec(highlight, WHITE);
            }
            draw_text_outlined(uiFontBold,
                               items[i],
                               (Vector2){(float)itemX, (float)itemY},
                               (float)fontSize,
                               1.0f,
                               TEXT_COLOR,
                               OUTLINE_COLOR,
                               3);

            // Determine whether the "em breve" label applies.
            bool canShowEmBreve = false;
            for (int j = 0; j < emBreveCount; j++)
            {
                if (emBreveIndices[j] == i)
                {
                    canShowEmBreve = true;
                    break;
                }
            }

            if (canShowEmBreve)
            {
                // Reveal "em breve" on hover/selection and blink while locked.
                bool showEmBreve = (i == selectedIndex) || (i == hoveredIndex);
                if (state == playing && blinkingIndex == i)
                {
                    showEmBreve = blinkVisible;
                }
                if (showEmBreve)
                {
                    float emBreveX = (float)itemX + textSize.x + (float)emBreveSpacing;
                    float emBreveY = (float)itemY + (float)(fontSize - emBreveFontSize) / 2.0f;
                    draw_text_outlined(uiFontLight,
                                       "em breve",
                                       (Vector2){emBreveX, emBreveY},
                                       (float)emBreveFontSize,
                                       1.0f,
                                       EM_BREVE_COLOR,
                                       OUTLINE_COLOR,
                                       2);
                }
            }
        }

        EndDrawing();
    }
}

int main(void)
{
    // Window setup.
    const int screenWidth = 900;
    const int screenHeight = 800;

    // Initialize the window and frame rate.
    InitWindow(screenWidth, screenHeight, "Paramancer");
    InitAudioDevice();
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    Music bgm = LoadMusicStream("assets/audio/soundtrack.mp3");
    PlayMusicStream(bgm);

    // Load textures required by the menu.
    Texture2D backgroundTexture;
    Texture2D logoTexture;
    // Abort early if any texture fails to load.
    if (!load_menu_textures(&backgroundTexture, &logoTexture))
    {
        CloseWindow();
        return 1;
    }

    // Load bold/light fonts for the menu and stats table.
    bool boldIsDefault = false;
    bool lightIsDefault = false;
    Font uiFontBold = load_ui_font_from_path("assets/fonts/Pixeloid/PixeloidSans-Bold.ttf", 32, &boldIsDefault);
    Font uiFontLight = load_ui_font_from_path("assets/fonts/Pixeloid/PixeloidSans.ttf", 30, &lightIsDefault);

    // Run the menu loop until exit.
    run_menu(backgroundTexture, logoTexture, uiFontBold, uiFontLight, screenWidth, screenHeight, bgm);

    // Release assets.
    if (!boldIsDefault)
    {
        UnloadFont(uiFontBold);
    }
    if (!lightIsDefault)
    {
        UnloadFont(uiFontLight);
    }

    // Cleanup texture resources.
    UnloadMusicStream(bgm);
    unload_menu_textures(backgroundTexture, logoTexture);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}