// Simplified game flow – background and music always present
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <raylib.h>
#ifdef _WIN32
#include <conio.h>
#endif

#include "jogo.h"
#include "actors.h"
#include "historico.h"
#include "perguntas.h"
#include "analise.h"
#include "sorteio.h"
#include "geracao.h"
#include "map_render.h"

// Global variables to keep last numeric guess and target for hint after question
static int lastGuess = 0;
static int lastTarget = 0;

// Global resources (provided by main.c)
extern Music bgm;

// Global character instances (defined here)
Entity entity = {.health = 100, .spriteState = 0, .entityType = 0};
Dex dex = {.health = 100, .buffer = 0, .spriteState = 0, .correctAnswers = 0, .score = 0};

// Procedurally generated platform map (used during gameplay)
char gameMap[MAP_ALTURA][MAP_LARGURA] = {0};

// Floor tile texture for procedural map rendering
Texture2D floorTexture = {0};

// Text strings used in the game
static const char *sistema[] = {
    "Bem-vindo ao System Breach! Prepare-se para uma jornada de perguntas e respostas sobre segurança cibernética.",
    "Você está pronto para testar seus conhecimentos e se tornar um mestre da segurança digital?",
    "Vamos começar! Boa sorte!"};

static const char *dialogo[] = {
    "eu sou a entidade e bla bla bla -entidade",
    "vamo jogar um jogo de acertar numero humano patetico e zas e zas -entidade",
    "eu ajudo mano -zero"};

// Forward declarations for helper functions
static void draw_gameplay_background(void);

// Helper: draw centered text with background and keep music playing
static void draw_centered(const char *text, Color col)
{
    // Keep music playing while rendering the centered message
    UpdateMusicStream(bgm);
    // Draw solid background (sky blue)
    ClearBackground((Color){200, 140, 80, 255});

    // Choose font size – make sistema[0] a bit smaller, sistema[1] a bit smaller by 2
    int fontSize = 20;
    if (strcmp(text, sistema[0]) == 0)
    {
        fontSize = 14; // slightly smaller for the first message
    }
    else if (strcmp(text, sistema[1]) == 0)
    {
        fontSize = 18; // two points smaller for the second message
    }
    // Detect if the text comes from dialogo array
    bool isDialog = false;
    for (int i = 0; i < 3; ++i)
    {
        if (strcmp(text, dialogo[i]) == 0)
        {
            isDialog = true;
            break;
        }
    }
    // Measure text width
    int txtWidth = MeasureText(text, fontSize);
    // Horizontal centering
    int x = (GetScreenWidth() - txtWidth) / 2;
    // Vertical positioning: top‑center for dialog, otherwise centre
    int y = isDialog ? (int)(GetScreenHeight() * 0.2f) - fontSize / 2 : GetScreenHeight() / 2 - fontSize / 2;

    // Define padding for the rectangle
    const int padding = 8;
    Rectangle border = {
        (float)(x - padding),
        (float)(y - padding),
        (float)(txtWidth + 2 * padding),
        (float)(fontSize + 2 * padding)};

    // Draw a black filled rectangle as background for the text
    DrawRectangleRec(border, BLACK);
    // Then draw a black border around it
    DrawRectangleLinesEx(border, 2, WHITE);

    // Draw the text – white for all messages (overrides any passed color)
    DrawText(text, x, y, fontSize, WHITE);
}

// Generic warning message box helper
static void warning(const char *msg)
{
    double start = GetTime();
    while (GetTime() - start < 4.0)
    {
        if (WindowShouldClose())
            return;
        UpdateMusicStream(bgm);
        BeginDrawing();
        // Background with procedurally generated platforms
        draw_gameplay_background();
        // Health HUD (same as other warnings)
        char dexHealthStr[32];
        sprintf(dexHealthStr, "Vida do Dex: %d", dex.health);
        DrawText(dexHealthStr, 20, 20, 20, WHITE);
        char entityHealthStr[32];
        sprintf(entityHealthStr, "Vida da Entidade: %d", entity.health);
        int eTextWidth = MeasureText(entityHealthStr, 20);
        DrawText(entityHealthStr, GetScreenWidth() - eTextWidth - 20, 20, 20, WHITE);
        // Warning box
        int msgFontSize = 20;
        int txtW = MeasureText(msg, msgFontSize);
        const int pad = 8;
        int boxW = txtW + 2 * pad;
        int boxH = msgFontSize + 2 * pad;
        int boxX = (GetScreenWidth() - boxW) / 2;
        int boxY = (GetScreenHeight() - boxH) / 2;
        Rectangle wbox = {(float)boxX, (float)boxY, (float)boxW, (float)boxH};
        DrawRectangleRec(wbox, BLACK);
        DrawRectangleLinesEx(wbox, 2, WHITE);
        DrawText(msg, boxX + pad, boxY + pad, msgFontSize, WHITE);
        EndDrawing();
    }
}

// Victory box displayed when Dex wins
static void victoryBox(void)
{
    double start = GetTime();
    while (GetTime() - start < 3.0)
    {
        if (WindowShouldClose())
            return;
        UpdateMusicStream(bgm);
        BeginDrawing();
        // Background with procedurally generated platforms
        draw_gameplay_background();
        // Victory message
        const char *msg = "parabens! dex venceu!";
        int msgFontSize = 20;
        int txtW = MeasureText(msg, msgFontSize);
        const int pad = 8;
        int boxW = txtW + 2 * pad;
        int boxH = msgFontSize + 2 * pad;
        int boxX = (GetScreenWidth() - boxW) / 2;
        int boxY = (GetScreenHeight() - boxH) / 2;
        Rectangle wbox = {(float)boxX, (float)boxY, (float)boxW, (float)boxH};
        DrawRectangleRec(wbox, BLACK);
        DrawRectangleLinesEx(wbox, 2, WHITE);
        DrawText(msg, boxX + pad, boxY + pad, msgFontSize, WHITE);
        EndDrawing();
    }
}

// Defeat box displayed when Dex loses
static void defeatBox(void)
{
    double start = GetTime();
    while (GetTime() - start < 3.0)
    {
        if (WindowShouldClose())
            return;
        UpdateMusicStream(bgm);
        BeginDrawing();
        // Background with procedurally generated platforms
        draw_gameplay_background();
        // Defeat message
        const char *msg = "que pena, a entidade venceu";
        int msgFontSize = 20;
        int txtW = MeasureText(msg, msgFontSize);
        const int pad = 8;
        int boxW = txtW + 2 * pad;
        int boxH = msgFontSize + 2 * pad;
        int boxX = (GetScreenWidth() - boxW) / 2;
        int boxY = (GetScreenHeight() - boxH) / 2;
        Rectangle wbox = {(float)boxX, (float)boxY, (float)boxW, (float)boxH};
        DrawRectangleRec(wbox, BLACK);
        DrawRectangleLinesEx(wbox, 2, WHITE);
        DrawText(msg, boxX + pad, boxY + pad, msgFontSize, WHITE);
        EndDrawing();
    }
}

// Renders the gameplay background with procedurally generated platforms
// Responsibilities: Clear screen with sky color and render floor tiles
// Parameters: None (uses global gameMap and floorTexture)
// Flow: 1) Draw sunset sky blue background 2) Render floor tiles from procedural map
static void draw_gameplay_background(void)
{
    // Draw sky blue background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){135, 206, 235, 255});

    // Render the floor tiles anchored to the bottom
    // floor.png is 64×128 pixels
    // Map is 20 tiles × 1 row (1280px width × 128px height)
    // Position at Y = 592 to place floor at bottom of 720px screen (720 - 128 = 592)
    if (floorTexture.id != 0)
    {
        map_render_platforms(gameMap, floorTexture, 0.0f, 592.0f);
    }
}

// Show a sequence of messages, advancing automatically after 3 seconds each
static void introducao(void)
{
    // Sequence: first three sistema messages, then first three dialogo messages
    const char *seq[] = {sistema[0], sistema[1], sistema[2], dialogo[0], dialogo[1], dialogo[2]};
    const int total = 6;
    const float displayTime = 4.0f; // seconds per message

    for (int i = 0; i < total; ++i)
    {
        double start = GetTime(); // Raylib timer in seconds
        while (true)
        {
            if (WindowShouldClose())
                return; // graceful exit

            // Keep music playing while messages are displayed
            UpdateMusicStream(bgm);

            BeginDrawing();
            ClearBackground(GRAY);
            draw_centered(seq[i], DARKGRAY);
            EndDrawing();

            // Exit loop after the desired display time has elapsed
            if ((GetTime() - start) >= displayTime)
                break;
        }
    }
}

// Simple gameplay: guess a number between 1 and 100
static void perguntas(void);

static void gameplay(void)
{
    // Initialize a session record for this round
    Session sess = {0};
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(sess.timestamp, sizeof(sess.timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    int target = sortearNumero(); // 1..100
    sess.target = target;
    lastTarget = target;   // store for hint after question
    char guessStr[4] = ""; // up to 3 digits

    // Map and texture are pre-initialized at startup in main()

    while (true)
    {
        if (WindowShouldClose())
            return;
        // Interrupt if any character health reaches zero
        if (dex.health <= 0 || entity.health <= 0)
        {
            if (entity.health <= 0 && dex.health > 0)
            {
                // Dex wins – show victory box for 3 seconds
                victoryBox();
            }
            else if (dex.health <= 0 && entity.health > 0)
            {
                // Dex loses – show defeat box for 3 seconds
                defeatBox();
            }
            // Return to menu (exit gameplay loop)
            return;
        }
        // Keep music playing
        UpdateMusicStream(bgm);
        BeginDrawing();
        // Draw gameplay background with procedurally generated platforms
        draw_gameplay_background();
        // Health displays (always visible)
        char dexHealthStr[32];
        sprintf(dexHealthStr, "Vida do Dex: %d", dex.health);
        DrawText(dexHealthStr, 20, 20, 20, WHITE);
        char entityHealthStr[32];
        sprintf(entityHealthStr, "Vida da Entidade: %d", entity.health);
        int eTextWidth = MeasureText(entityHealthStr, 20);
        DrawText(entityHealthStr, GetScreenWidth() - eTextWidth - 20, 20, 20, WHITE);

        // Centered black rectangle with white text and black border
        const int rectWidth = 500;
        const int rectHeight = 150;
        int rectX = (GetScreenWidth() - rectWidth) / 2;
        int rectY = (GetScreenHeight() - rectHeight) / 2;
        Rectangle rect = {(float)rectX, (float)rectY, (float)rectWidth, (float)rectHeight};
        DrawRectangleRec(rect, BLACK);
        DrawRectangleLinesEx(rect, 2, BLACK);
        const char *prompt = "Digite um número (1-100) e pressione ENTER:";
        int promptFontSize = 20;
        DrawText(prompt, rectX + 20, rectY + 20, promptFontSize, WHITE);
        int guessFontSize = 30;
        DrawText(guessStr, rectX + 20, rectY + 20 + promptFontSize + 20, guessFontSize, WHITE);
        EndDrawing();

        int key = GetKeyPressed();
        if (key >= 48 && key <= 57)
        { // 0-9
            if (strlen(guessStr) < 3)
            {
                size_t len = strlen(guessStr);
                guessStr[len] = (char)key;
                guessStr[len + 1] = '\0';
            }
        }
        else if (key == KEY_BACKSPACE && strlen(guessStr) > 0)
        {
            guessStr[strlen(guessStr) - 1] = '\0';
        }
        else if (IsKeyPressed(KEY_ENTER) && strlen(guessStr) > 0)
        {
            int guess = atoi(guessStr);
            lastGuess = guess;
            if (guess == target)
            {
                // Correct guess: damage entity
                entity.health -= 100;
                warning("a entidade tomou dano!!");
                // Reset for a new round
                target = sortearNumero();
                // Update global target for next hint
                lastTarget = target;
                guessStr[0] = '\0';
                continue;
            }
            else
            {
                // Incorrect guess: show background and health for 4 seconds, then ask question
                double msgStart = GetTime();
                while (GetTime() - msgStart < 4.0)
                {
                    if (WindowShouldClose())
                        return;
                    UpdateMusicStream(bgm);
                    BeginDrawing();
                    // Draw gameplay background with procedurally generated platforms
                    draw_gameplay_background();
                    // Redraw health HUD
                    DrawText(dexHealthStr, 20, 20, 20, WHITE);
                    DrawText(entityHealthStr, GetScreenWidth() - eTextWidth - 20, 20, 20, WHITE);
                    EndDrawing();
                }
                // Show warning message before question
                warning("você terá uma chance de evitar o dano, acerte essa pergunta!");
                // Call perguntas after warning
                perguntas();
                // Reset guess input for next attempt
                guessStr[0] = '\0';
            }
        }
    }

    // Cleanup: unload the floor texture
    if (floorTexture.id != 0)
    {
        UnloadTexture(floorTexture);
        floorTexture = (Texture2D){0};
    }
}

// Implement perguntas function
static void perguntas(void)
{
    // Determine total number of questions in CSV
    FILE *file = fopen("perguntas.csv", "r");
    if (!file)
        return;
    int total = 0;
    char line[512];
    while (fgets(line, sizeof(line), file))
    {
        if (strlen(line) > 1)
            total++;
    }
    fclose(file);
    if (total == 0)
        return;
    // Choose a random question (1‑based index)
    int id = rand() % total + 1;
    Pergunta p;
    if (!carregar_pergunta_por_id(id, &p))
        return;

    char playerChoice = '\0';
    bool answered = false;
    while (!answered)
    {
        if (WindowShouldClose())
            return;
        UpdateMusicStream(bgm);
        BeginDrawing();
        // Background for question screen: Use gameplay background (sky color + floor tiles)
        draw_gameplay_background();
        // Health HUD
        char dexHealthStr[32];
        sprintf(dexHealthStr, "Vida do Dex: %d", dex.health);
        DrawText(dexHealthStr, 20, 20, 20, WHITE);
        char entityHealthStr[32];
        sprintf(entityHealthStr, "Vida da Entidade: %d", entity.health);
        int eTextWidth = MeasureText(entityHealthStr, 20);
        DrawText(entityHealthStr, GetScreenWidth() - eTextWidth - 20, 20, 20, WHITE);
        // Central black box
        const int boxW = 600, boxH = 200;
        int boxX = (GetScreenWidth() - boxW) / 2;
        int boxY = (GetScreenHeight() - boxH) / 2;
        Rectangle box = {(float)boxX, (float)boxY, (float)boxW, (float)boxH};
        DrawRectangleRec(box, BLACK);
        DrawRectangleLinesEx(box, 2, BLACK);
        // Render question and options
        int fontSize = 20;
        int lineY = boxY + 20;
        DrawText(p.enunciado, boxX + 20, lineY, fontSize, WHITE);
        lineY += fontSize + 10;
        char optBuf[256];
        sprintf(optBuf, "a) %s", p.alt_a);
        DrawText(optBuf, boxX + 20, lineY, fontSize, WHITE);
        lineY += fontSize + 5;
        sprintf(optBuf, "b) %s", p.alt_b);
        DrawText(optBuf, boxX + 20, lineY, fontSize, WHITE);
        lineY += fontSize + 5;
        sprintf(optBuf, "c) %s", p.alt_c);
        DrawText(optBuf, boxX + 20, lineY, fontSize, WHITE);
        lineY += fontSize + 5;
        sprintf(optBuf, "d) %s", p.alt_d);
        DrawText(optBuf, boxX + 20, lineY, fontSize, WHITE);
        lineY += fontSize + 10;
        // Show selected input if any
        if (playerChoice)
        {
            char selBuf[32];
            sprintf(selBuf, "Sua escolha: %c", playerChoice);
            DrawText(selBuf, boxX + 20, lineY, fontSize, WHITE);
        }
        EndDrawing();

        int k = GetKeyPressed();
        if (k >= 65 && k <= 68)
        { // A-D
            playerChoice = (char)tolower(k);
        }
        else if (k >= 97 && k <= 100)
        { // a-d
            playerChoice = (char)k;
        }
        if (IsKeyPressed(KEY_ENTER) && playerChoice)
        {
            answered = true;
        }
    }

    // Evaluate answer
    // Use case‑insensitive comparison by converting both to lower case
    char correctLower = (char)tolower(p.correta);
    if (playerChoice != correctLower)
    {
        // Incorrect answer – apply damage to Dex and show warning box
        dex.health -= 20;
        const char *correctText = "";
        switch (toupper(correctLower))
        {
        case 'A':
            correctText = p.alt_a;
            break;
        case 'B':
            correctText = p.alt_b;
            break;
        case 'C':
            correctText = p.alt_c;
            break;
        case 'D':
            correctText = p.alt_d;
            break;
        default:
            correctText = "";
            break;
        }
        double warnStart = GetTime();
        while (GetTime() - warnStart < 4.0)
        {
            if (WindowShouldClose())
                return;
            UpdateMusicStream(bgm);
            BeginDrawing();
            // Background for warning screen: Use gameplay background (sky color + floor tiles)
            draw_gameplay_background();
            char dexHealthStr[32];
            sprintf(dexHealthStr, "Vida do Dex: %d", dex.health);
            DrawText(dexHealthStr, 20, 20, 20, WHITE);
            char entityHealthStr[32];
            sprintf(entityHealthStr, "Vida da Entidade: %d", entity.health);
            int eTextWidth = MeasureText(entityHealthStr, 20);
            DrawText(entityHealthStr, GetScreenWidth() - eTextWidth - 20, 20, 20, WHITE);
            const char *msg1 = "dex recebeu 20 de dano!";
            char msg2[128];
            sprintf(msg2, "a resposta da pergunta era (%c) %s", toupper(correctLower), correctText);
            int msgFontSize = 20;
            int width1 = MeasureText(msg1, msgFontSize);
            int width2 = MeasureText(msg2, msgFontSize);
            int maxW = width1 > width2 ? width1 : width2;
            const int pad = 8;
            int boxW = maxW + 2 * pad;
            int boxH = msgFontSize * 2 + 3 * pad;
            int boxX = (GetScreenWidth() - boxW) / 2;
            int boxY = (GetScreenHeight() - boxH) / 2;
            Rectangle wbox = {(float)boxX, (float)boxY, (float)boxW, (float)boxH};
            DrawRectangleRec(wbox, BLACK);
            DrawRectangleLinesEx(wbox, 2, WHITE);
            DrawText(msg1, boxX + pad, boxY + pad, msgFontSize, WHITE);
            DrawText(msg2, boxX + pad, boxY + pad + msgFontSize + pad, msgFontSize, WHITE);
            EndDrawing();
        }
    }
    else
    {
        // Correct answer – show success box with hint about previous numeric guess
        double winStart = GetTime();
        while (GetTime() - winStart < 4.0)
        {
            if (WindowShouldClose())
                return;
            UpdateMusicStream(bgm);
            BeginDrawing();
            // Background for success screen: Use gameplay background (sky color + floor tiles)
            draw_gameplay_background();
            char dexHealthStr[32];
            sprintf(dexHealthStr, "Vida do Dex: %d", dex.health);
            DrawText(dexHealthStr, 20, 20, 20, WHITE);
            char entityHealthStr[32];
            sprintf(entityHealthStr, "Vida da Entidade: %d", entity.health);
            int eTextWidth = MeasureText(entityHealthStr, 20);
            DrawText(entityHealthStr, GetScreenWidth() - eTextWidth - 20, 20, 20, WHITE);
            const char *msg1 = "Você acertou a pergunta!";
            char hint[128];
            if (lastGuess < lastTarget)
            {
                strcpy(hint, "Dica: o número que você chutou era muito baixo.");
            }
            else if (lastGuess > lastTarget)
            {
                strcpy(hint, "Dica: o número que você chutou era muito alto.");
            }
            else
            {
                strcpy(hint, "Dica: o número que você chutou era exatamente o alvo.");
            }
            int msgFontSize = 20;
            int width1 = MeasureText(msg1, msgFontSize);
            int width2 = MeasureText(hint, msgFontSize);
            int maxW = width1 > width2 ? width1 : width2;
            const int pad = 8;
            int boxW = maxW + 2 * pad;
            int boxH = msgFontSize * 2 + 3 * pad;
            int boxX = (GetScreenWidth() - boxW) / 2;
            int boxY = (GetScreenHeight() - boxH) / 2;
            Rectangle wbox = {(float)boxX, (float)boxY, (float)boxW, (float)boxH};
            DrawRectangleRec(wbox, BLACK);
            DrawRectangleLinesEx(wbox, 2, WHITE);
            DrawText(msg1, boxX + pad, boxY + pad, msgFontSize, WHITE);
            DrawText(hint, boxX + pad, boxY + pad + msgFontSize + pad, msgFontSize, WHITE);
            EndDrawing();
        }
    }
}

// Public API – runs the full flow (intro then gameplay)
Session executar_partida()
{
    // Ensure music is playing from the start
    PlayMusicStream(bgm);
    // Main loop – just keep drawing background while intro or gameplay runs
    introducao();
    gameplay();
    return (Session){0}; // Placeholder return value; fill with actual session data as needed
}
