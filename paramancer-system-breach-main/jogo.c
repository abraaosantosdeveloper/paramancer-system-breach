// Simplified game flow – background and music always present
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "tipos.h"
#include <string.h>
#include <ctype.h>
#define SUPPORT_FILEFORMAT_JPEG
#include "raylib.h"
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
static bool firstMistake = true;
static bool firstCorrect = true;
static bool firstQuestionMistake = true;
static bool firstQuestionCorrect = true;

// Global resources (provided by main.c)
extern Music bgm;

// Global character instances (defined here)
Entity entity = {.health = 100, .spriteState = 0, .entityType = 0};
Dex dex = {.health = 100, .buffer = 0, .spriteState = 0, .correctAnswers = 0, .score = 0};

// Procedurally generated platform map (used during gameplay)
char gameMap[MAP_ALTURA][MAP_LARGURA] = {0};

// Floor tile texture for procedural map rendering
Texture2D floorTexture = {0};
Texture2D backgroundTexture = {0};

// Background layers
static Texture2D skyTexture = {0};
static Texture2D layer1Texture = {0};
static Texture2D layer2Texture = {0};
static bool bgOffsetsReady = false;
static float bgOffsets[3] = {0};
static bool entityVisible = false;
static bool entitySlideActive = false;
static float entitySlideX = 0.0f;
static bool dexBlinkActive = false;
static bool dexBlinkVisible = true;
static float dexBlinkTimer = 0.0f;
static int dexBlinkTogglesRemaining = 0;
static bool entityBlinkActive = false;
static bool entityBlinkVisible = true;
static float entityBlinkTimer = 0.0f;
static int entityBlinkTogglesRemaining = 0;

// Actor textures (character portraits)
static Texture2D texDex = {0};
static Texture2D texZero = {0};
static Texture2D texEntity = {0};
static Texture2D texDexIdle = {0};
static Texture2D texEntidadeSprite = {0};
static Texture2D heartFullRed = {0};
static Texture2D heartHalfRed = {0};
static Texture2D heartFullBlue = {0};
static Texture2D heartHalfBlue = {0};
static Texture2D heartEmpty = {0};

// Load all actor textures – called once before the first scene
static void load_actor_textures(void)
{
    // Load Dex sprite and make black background transparent
    texDex = LoadTexture("assets/game_arts/actors/dexface.png");
    // Load Zero sprite (no background removal needed)
    texZero = LoadTexture("assets/game_arts/actors/zeroface.png");

    // Load Entidade sprite and make black background transparent
    texEntity = LoadTexture("assets/game_arts/actors/entidade.png");

    texDexIdle = LoadTexture("assets/game_arts/actors/dex_idle.png");
    texEntidadeSprite = LoadTexture("assets/game_arts/actors/the_entity.png");
    heartFullRed = LoadTexture("assets/game_arts/actors/full_heart.png");
    heartHalfRed = LoadTexture("assets/game_arts/actors/half_heart.png");
    heartFullBlue = LoadTexture("assets/game_arts/actors/full_heart_blue.png");
    heartHalfBlue = LoadTexture("assets/game_arts/actors/half_heart_blue.png");
    heartEmpty = LoadTexture("assets/game_arts/actors/empty_heart.png");

    // Load background layers
    skyTexture = LoadTexture("assets/game_arts/scenario/City background sky.png");
    layer1Texture = LoadTexture("assets/game_arts/scenario/City background layer1.png");
    layer2Texture = LoadTexture("assets/game_arts/scenario/City background layer2.png");

    if (skyTexture.id == 0 || layer1Texture.id == 0 || layer2Texture.id == 0)
    {
        fprintf(stderr, "[WARNING] Could not load one or more background layers.\n");
    }
    entityVisible = false;
    entitySlideActive = false;
    entitySlideX = 0.0f;
    dexBlinkActive = false;
    dexBlinkVisible = true;
    dexBlinkTimer = 0.0f;
    dexBlinkTogglesRemaining = 0;
    entityBlinkActive = false;
    entityBlinkVisible = true;
    entityBlinkTimer = 0.0f;
    entityBlinkTogglesRemaining = 0;
}

// Unload actor textures – called on graceful shutdown
static void unload_actor_textures(void)
{
    if (texDex.id)
        UnloadTexture(texDex);
    if (texZero.id)
        UnloadTexture(texZero);
    if (texEntity.id)
        UnloadTexture(texEntity);
    if (texDexIdle.id)
        UnloadTexture(texDexIdle);
    if (texEntidadeSprite.id)
        UnloadTexture(texEntidadeSprite);
    if (heartFullRed.id)
        UnloadTexture(heartFullRed);
    if (heartHalfRed.id)
        UnloadTexture(heartHalfRed);
    if (heartFullBlue.id)
        UnloadTexture(heartFullBlue);
    if (heartHalfBlue.id)
        UnloadTexture(heartHalfBlue);
    if (heartEmpty.id)
        UnloadTexture(heartEmpty);

    if (skyTexture.id)
        UnloadTexture(skyTexture);
    if (layer1Texture.id)
        UnloadTexture(layer1Texture);
    if (layer2Texture.id)
        UnloadTexture(layer2Texture);
}

// Helper: return the correct texture for the speaker based on the dialog line
static Texture2D get_speaker_texture(const char *msg)
{
    if (strstr(msg, "- Dex") != NULL)
        return texDex;
    if (strstr(msg, "- Zero") != NULL)
        return texZero;
    if (strstr(msg, "- The Entity") != NULL ||
        strstr(msg, "- The entity") != NULL)
        return texEntity;
    return (Texture2D){0}; // no portrait
}

static void draw_hearts_row(int startX, int startY, bool isDex, int health, int maxHealth)
{
    const int heartCount = 10;
    int clamped = health;
    if (clamped < 0)
        clamped = 0;
    if (clamped > maxHealth)
        clamped = maxHealth;

    int missingHalfUnits = (maxHealth - clamped) / 5;
    if (missingHalfUnits < 0)
        missingHalfUnits = 0;
    if (missingHalfUnits > heartCount * 2)
        missingHalfUnits = heartCount * 2;

    int emptyCount = missingHalfUnits / 2;
    int hasHalf = (missingHalfUnits % 2);

    Texture2D fullTex = isDex ? heartFullRed : heartFullBlue;
    Texture2D halfTex = isDex ? heartHalfRed : heartHalfBlue;
    Texture2D emptyTex = heartEmpty;

    const float HEART_SCALE = 0.1f;
    int heartW = (int)(fullTex.width * HEART_SCALE);
    int heartH = (int)(fullTex.height * HEART_SCALE);
    int spacing = 4;

    for (int i = 0; i < heartCount; ++i)
    {
        int drawX = startX + i * (heartW + spacing);
        int drawY = startY;
        Texture2D toDraw = fullTex;

        if (isDex)
        {
            int emptyStart = heartCount - emptyCount;
            if (i >= emptyStart)
            {
                toDraw = emptyTex;
            }
            else if (hasHalf && i == emptyStart - 1)
            {
                toDraw = halfTex;
            }
        }
        else
        {
            if (i < emptyCount)
            {
                toDraw = emptyTex;
            }
            else if (hasHalf && i == emptyCount)
            {
                toDraw = halfTex;
            }
        }

        DrawTextureEx(toDraw, (Vector2){(float)drawX, (float)drawY}, 0.0f, HEART_SCALE, WHITE);
    }
}

static void draw_health_hud(void)
{
    const int maxHealth = 100;
    const float HEART_SCALE = 0.1f;
    int heartW = (int)(heartFullRed.width * HEART_SCALE);
    int spacing = 4;
    int totalW = heartW * 10 + spacing * 9;

    draw_hearts_row(20, 16, true, dex.health, maxHealth);
    draw_hearts_row(GetScreenWidth() - totalW - 20, 16, false, entity.health, maxHealth);
}

// Text strings used in the game
static const char *sistema[] = {
    "Bem-vindo ao System Breach! Prepare-se para uma jornada de perguntas e respostas sobre segurança cibernética.",
    "Você está pronto para testar seus conhecimentos e se tornar um mestre da segurança digital?",
    "Vamos começar! Boa sorte!"};

static const char *dialogo[] = {
    "Tem algo de errado... - Dex",
    "Como assim? - Zero",
    "Não percebe? Tudo parece... instável? Até parece que existem falhas na Matrix! - Dex",
    "Agora que você falou... também tinha percebido algo estranho. Mas achei que era coisa da minha cabeça... - Zero",
    "(Entidade aparece)",
    "O que diabos é isso? - Dex",
    "Pode me chamar de Entidade! - The Entity",
    "O que é você? O que você quer? - Dex",
    "Sou uma IA extremamente poderosa. Eu vim aqui para tomar os parâmetros do universo, para me fortalecer e dominar o mundo! - The entity",
    "Q... Quê? - Dex",
    "Dex! Você consegue enxergar? Tente tomar os parâmetros de volta para devolver a estabilidade ao universo! - Zero",
    "Está bem, mas como vamos fazer isso? - Dex",
    "Eu acho que encontrei uma forma de parar a Entidade. Preciso que você tente descobrir o parâmetro do sistema. - Zero",
    "É um número inteiro, pelo que vejo. O problema é saber qual número, especificamente. - Zero",
    "Qual o seu palpite? - Zero",
    "O parâmetro é... - Dex",
    "Excelente! Você acertou um dos parâmetros. Mas isso ainda não é tudo!",
    "Vamos ao próximo... - Zero",
    "Não está correto... mas encontrei um jeito de impedir que isso nos afete de alguma maneira. - Zero",
    "Existe uma série de criptografias, mas para quebrá-las, precisamos descobrir palavras-chave com base em algumas pistas. - Zero",
    "Aqui vai a primeira pista... - Zero",
    "Dex, se concentra. Não deixe a entidade tirar sua atenção. Tenta novamente! - Zero ",
    "Excelente! Mas isso ainda não é tudo. Precisamos tentar adivinhar o parâmetro novamente... - Zero ",
    "O que está havendo?! C... Como você pode ter me vencido?! - The Entity",
    "Parece que você não obteve êxito no seu plano... - Dex",
    "Conseguimos! Eu vou destruí-la de uma vez por todas, de dentro para fora. Já restaurei os parâmetros e agora vou desativar os servidores que alimentam esse monstro! - Zero",
    "Nãaaaaaoooo! Malditos! - The Entity",
    "Consegui desativar! - Zero",
    "Isso! Conseguimos! - Dex ",
    "Parece que vocês não são tão espertos quanto pensam. Não conseguiram me parar, e nunca alcançarão este feito! - The entity ",
    "Droga! - Dex",
    "Não podemos fazer mais nada, a entidade conseguiu fechar as brechas de segurança que usei para invadir os servidores... - Zero ",
    "Eu não acredito! - Dex ",
    "Ninguém irá me impedir de dominar o universo. - The entity ",
    "Adeus! - The entity",
};

// Forward declarations for helper functions
static void draw_gameplay_background(void);

// Helper: draw centered text with background and keep music playing
static void draw_centered(const char *text)
{
    // Keep music playing while rendering the centered message
    UpdateMusicStream(bgm);
    // Draw full background (scenario) before text
    draw_gameplay_background();

    // Choose base font size – adjust for specific sistema messages
    int fontSize = 20;
    if (strcmp(text, sistema[0]) == 0)
    {
        fontSize = 18; // slightly smaller for the first message
    }
    else if (strcmp(text, sistema[1]) == 0)
    {
        fontSize = 23; // two points smaller for the second message
    }

    if (strcmp(text, dialogo[25]) == 0)
    {
        fontSize = 25;
    }

    // Detect if the text comes from dialogo array (dynamic length)
    bool isDialog = false;
    int dialogCount = sizeof(dialogo) / sizeof(dialogo[0]);
    for (int i = 0; i < dialogCount; ++i)
    {
        if (strcmp(text, dialogo[i]) == 0)
        {
            isDialog = true;
            break;
        }
    }
    // Reduce font size for dialog messages displayed via exibid_dial

    // Adjust font size if text would overflow the screen
    const int padding = 8;
    const int spriteSpace = 64;                                       // reserve space on the right for character sprite
    int maxWidth = GetScreenWidth() - 2 * padding - spriteSpace - 20; // 20px margin
    int txtWidth = MeasureText(text, fontSize);
    if (txtWidth > maxWidth && txtWidth > 0)
    {
        // Scale font down proportionally to fit within maxWidth
        fontSize = (int)(fontSize * (float)maxWidth / (float)txtWidth);
        txtWidth = MeasureText(text, fontSize);
    }

    // Determine if a speaker texture is available
    Texture2D speakerTex = get_speaker_texture(text);
    bool hasSpeaker = (speakerTex.id != 0);

    if (!entityVisible && strcmp(text, dialogo[4]) == 0)
    {
        entityVisible = true;
        entitySlideActive = true;
        entitySlideX = (float)GetScreenWidth() + 40.0f;
    }

    // Base rectangle position assuming sprite space present
    int baseRectX = (GetScreenWidth() - txtWidth - 2 * padding - spriteSpace) / 2;
    int baseRectY = isDialog ? (int)(GetScreenHeight() * 0.2f) - fontSize / 2 : GetScreenHeight() / 2 - fontSize / 2;

    // Content dimensions
    int textBlockHeight = fontSize + 2 * padding;
    int contentHeight = (textBlockHeight > spriteSpace) ? textBlockHeight : spriteSpace;

    const int extra = 5; // extra margin

    // Effective dimensions based on speaker presence
    int effectiveSpriteSpace = hasSpeaker ? spriteSpace : 0;
    int effectiveHeight = hasSpeaker ? contentHeight : textBlockHeight;
    int effectiveWidth = txtWidth + 2 * padding + effectiveSpriteSpace;

    int rectWidth = effectiveWidth + 2 * extra;
    int rectHeight = effectiveHeight + 2 * extra;
    int rectX = baseRectX - extra;
    int rectY = baseRectY - padding - extra;

    Rectangle border = {(float)rectX, (float)rectY, (float)rectWidth, (float)rectHeight};
    DrawRectangleRec(border, BLACK);
    DrawRectangleLinesEx(border, 4, WHITE);

    // Text positioning
    int textX;
    if (hasSpeaker)
        textX = rectX + extra + padding; // left side
    else
        textX = rectX + extra + (rectWidth - 2 * extra - txtWidth) / 2; // centered horizontally
    int textY = rectY + extra + (effectiveHeight - fontSize) / 2;       // vertically centered
    DrawText(text, textX, textY, fontSize, WHITE);

    // Draw speaker texture if present
    if (hasSpeaker)
    {
        int targetH = effectiveHeight - 2 * padding;
        float scale = (float)targetH / (float)speakerTex.height;
        int texX = rectX + extra + padding + txtWidth + padding;
        int texY = rectY + extra + (effectiveHeight - targetH) / 2;
        DrawTextureEx(speakerTex, (Vector2){(float)texX, (float)texY}, 0.0f, scale, WHITE);
    }
}

// Generic warning message box helper
static void warning(const char *msg)
{
    while (true)
    {
        if (WindowShouldClose())
            return;
        UpdateMusicStream(bgm);
        BeginDrawing();
        // Background with procedurally generated platforms
        draw_gameplay_background();
        draw_health_hud();
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
        DrawRectangleLinesEx(wbox, 4, WHITE);
        DrawText(msg, boxX + pad, boxY + pad, msgFontSize, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_ENTER))
            break;
    }
}

// Display dialog in a box similar to introducao
static void exibid_dial(const char *msg)
{
    while (true)
    {
        if (WindowShouldClose())
            return;
        UpdateMusicStream(bgm);
        BeginDrawing();
        draw_gameplay_background();
        draw_centered(msg);
        EndDrawing();
        if (IsKeyPressed(KEY_ENTER))
            break;
    }
}

// Victory box displayed when Dex wins
static void victoryBox(void)
{
    exibid_dial(dialogo[23]);
    exibid_dial(dialogo[24]);
    exibid_dial(dialogo[25]);
    exibid_dial(dialogo[26]);
    exibid_dial(dialogo[27]);
    exibid_dial(dialogo[28]);
    while (true)
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
        DrawRectangleLinesEx(wbox, 4, WHITE);
        DrawText(msg, boxX + pad, boxY + pad, msgFontSize, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_ENTER))
            break;
    }
}

static void play_entity_defeat_sequence(void)
{
    entityBlinkActive = true;
    entityBlinkVisible = true;
    entityBlinkTimer = 0.0f;
    entityBlinkTogglesRemaining = 6;

    while (entityBlinkActive)
    {
        if (WindowShouldClose())
            return;
        UpdateMusicStream(bgm);
        BeginDrawing();
        draw_gameplay_background();
        draw_health_hud();
        EndDrawing();
    }

    entityVisible = false;
}

// Defeat box displayed when Dex loses
static void defeatBox(void)
{
    exibid_dial(dialogo[29]);
    exibid_dial(dialogo[30]);
    exibid_dial(dialogo[31]);
    exibid_dial(dialogo[32]);
    exibid_dial(dialogo[33]);
    exibid_dial(dialogo[34]);
    while (true)
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
        DrawRectangleLinesEx(wbox, 4, WHITE);
        DrawText(msg, boxX + pad, boxY + pad, msgFontSize, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_ENTER))
            break;
    }
}

// Renders the gameplay background with procedurally generated platforms
// Responsibilities: Clear screen with sky color and render floor tiles
// Parameters: None (uses global gameMap and floorTexture)
// Flow: 1) Draw sunset sky blue background 2) Render floor tiles from procedural map
static void draw_gameplay_background(void)
{
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();
    float dt = GetFrameTime();

    // Fallback: sky blue background
    DrawRectangle(0, 0, screenW, screenH, (Color){135, 206, 235, 255});

    // Draw background layers with minimal scaling and subtle parallax
    // Order: sky → layer1 (distant, fog tint) → layer2 (close, full color)
    Texture2D layers[3] = {skyTexture, layer1Texture, layer2Texture};
    // Tints: sky = normal, layer1 = bluish fog (semi-transparent), layer2 = full opacity
    Color layerTints[3] = {
        WHITE,                       // sky
        (Color){180, 210, 230, 160}, // layer1: distant, névoa azulada
        WHITE                        // layer2: proxima, cor normal
    };
    if (!bgOffsetsReady)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (layers[i].id == 0)
            {
                bgOffsets[i] = 0.0f;
                continue;
            }

            float scale = 1.0f;
            if (layers[i].width < screenW || layers[i].height < screenH)
            {
                float scaleW = (float)screenW / (float)layers[i].width;
                float scaleH = (float)screenH / (float)layers[i].height;
                scale = (scaleW > scaleH) ? scaleW : scaleH;
            }

            float destW = layers[i].width * scale;
            float maxShift = (destW > screenW) ? (destW - (float)screenW) : 0.0f;
            bgOffsets[i] = (maxShift > 0.0f) ? (float)GetRandomValue(0, (int)maxShift) : 0.0f;
        }
        bgOffsetsReady = true;
    }

    for (int i = 0; i < 3; ++i)
    {
        if (layers[i].id == 0)
            continue;

        float scale = 1.0f;
        if (layers[i].width < screenW || layers[i].height < screenH)
        {
            float scaleW = (float)screenW / (float)layers[i].width;
            float scaleH = (float)screenH / (float)layers[i].height;
            scale = (scaleW > scaleH) ? scaleW : scaleH;
        }

        float destW = layers[i].width * scale;
        float destH = layers[i].height * scale;
        float offsetX = (destW > screenW) ? -bgOffsets[i] : 0.0f;

        DrawTexturePro(layers[i],
                       (Rectangle){0.0f, 0.0f, (float)layers[i].width, (float)layers[i].height},
                       (Rectangle){offsetX, 0.0f, destW, destH},
                       (Vector2){0.0f, 0.0f}, 0.0f, layerTints[i]);
    }
    // Render the floor tiles anchored to the bottom
    // floor.png is 64×128 pixels
    // Map is 20 tiles × 1 row (1280px width × 128px height)
    // Position at Y = 592 to place floor at bottom of 720px screen (720 - 128 = 592)
    const float floorY = 592.0f;
    if (floorTexture.id != 0)
    {
        map_render_platforms(gameMap, floorTexture, 0.0f, floorY);
    }
    // Render final scene sprites: Dex idle on left, Entidade on right
    if (texDexIdle.id)
    {
        const float scaleDex = 0.09f;
        const float scaleEnt = 0.22f;
        float wDex = texDexIdle.width * scaleDex;
        float dexY = (float)floorY - texDexIdle.height * scaleDex + 1.0f;
        float dexX = (float)GetScreenWidth() * 0.25f - wDex * 0.5f;
        if (dexBlinkActive)
        {
            dexBlinkTimer += dt;
            if (dexBlinkTimer >= 0.12f)
            {
                dexBlinkTimer -= 0.12f;
                dexBlinkVisible = !dexBlinkVisible;
                dexBlinkTogglesRemaining--;
                if (dexBlinkTogglesRemaining <= 0)
                {
                    dexBlinkActive = false;
                    dexBlinkVisible = true;
                }
            }
        }

        if (!dexBlinkActive || dexBlinkVisible)
        {
            DrawTextureEx(texDexIdle, (Vector2){dexX, dexY}, 0.0f, scaleDex, WHITE);
        }

        if (entityVisible && texEntidadeSprite.id)
        {
            float wEnt = texEntidadeSprite.width * scaleEnt;
            float entY = (float)floorY - texEntidadeSprite.height * scaleEnt + 1.0f;
            float entTargetX = (float)GetScreenWidth() * 0.75f - wEnt * 0.5f;
            float entX = entTargetX;
            if (entitySlideActive)
            {
                entitySlideX -= 420.0f * dt;
                if (entitySlideX <= entTargetX)
                {
                    entitySlideX = entTargetX;
                    entitySlideActive = false;
                }
                entX = entitySlideX;
            }
            if (entityBlinkActive)
            {
                entityBlinkTimer += dt;
                if (entityBlinkTimer >= 0.12f)
                {
                    entityBlinkTimer -= 0.12f;
                    entityBlinkVisible = !entityBlinkVisible;
                    entityBlinkTogglesRemaining--;
                    if (entityBlinkTogglesRemaining <= 0)
                    {
                        entityBlinkActive = false;
                        entityBlinkVisible = true;
                    }
                }
            }

            if (!entityBlinkActive || entityBlinkVisible)
            {
                DrawTextureEx(texEntidadeSprite, (Vector2){entX, entY}, 0.0f, scaleEnt, WHITE);
            }
        }
    }
}

// Show a sequence of messages, advancing automatically after 3 seconds each
static void introducao(void)
{
    // Show system messages first – advance on ENTER key
    for (int i = 0; i < 3; ++i)
    {
        while (true)
        {
            if (WindowShouldClose())
                return;
            UpdateMusicStream(bgm);
            BeginDrawing();
            draw_gameplay_background();
            draw_centered(sistema[i]);
            EndDrawing();
            // Advance when ENTER is pressed
            if (IsKeyPressed(KEY_ENTER))
                break;
        }
    }

    // Then show all dialogo messages – dynamic count, advance on ENTER key
    for (int i = 0; i < 15; ++i)
    {
        while (true)
        {
            if (WindowShouldClose())
                return;
            UpdateMusicStream(bgm);
            BeginDrawing();
            draw_gameplay_background();
            draw_centered(dialogo[i]);
            EndDrawing();
            if (IsKeyPressed(KEY_ENTER))
                break;
        }
    }
}

// Simple gameplay: guess a number between 1 and 100
static void perguntas(void);

static Session current_session; // holds data for the current gameplay session

static void gameplay(void)
{
    // Initialize a session record for this round
    memset(&current_session, 0, sizeof(current_session));
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(current_session.timestamp, sizeof(current_session.timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    int target = sortearNumero(); // 1..100
    current_session.target = target;
    lastTarget = target;   // store for hint after question
    char guessStr[4] = ""; // up to 3 digits

    // Map and texture are pre-initialized at startup in main()
    dexBlinkActive = true;
    dexBlinkVisible = true;
    dexBlinkTimer = 0.0f;
    dexBlinkTogglesRemaining = 6;

    while (true)
    {
        if (WindowShouldClose())
            return;
        // Interrupt if any character health reaches zero
        if (dex.health <= 0 || entity.health <= 0)
        {
            if (entity.health <= 0 && dex.health > 0)
            {
                play_entity_defeat_sequence();
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
        draw_health_hud();

        // Centered black rectangle with white text and black border
        const int rectWidth = 500;
        const int rectHeight = 150;
        int rectX = (GetScreenWidth() - rectWidth) / 2;
        int rectY = (GetScreenHeight() - rectHeight) / 2;
        Rectangle rect = {(float)rectX, (float)rectY, (float)rectWidth, (float)rectHeight};
        DrawRectangleRec(rect, BLACK);
        DrawRectangleLinesEx(rect, 4, WHITE);
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
            // Record the guess
            if (current_session.attempts_count < MAX_GUESSES)
            {
                current_session.guesses[current_session.attempts_count] = guess;
            }
            current_session.attempts_count++;
            // Track high/low bias
            if (guess > target)
            {
                current_session.high_count++;
            }
            else if (guess < target)
            {
                current_session.low_count++;
            }
            lastGuess = guess;
            if (guess == target)
            {
                // Correct guess: damage entity
                if (firstCorrect)
                {
                    exibid_dial(dialogo[16]); // "Excelente! Você acertou um dos parâmetros. Mas isso ainda não é tudo!"
                    exibid_dial(dialogo[17]); // "Vamos ao próximo… - Zero"
                    firstCorrect = false;
                }
                entity.health -= 60;
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
                if (firstMistake)
                {
                    // Show special dialog sequence on first mistake
                    exibid_dial(dialogo[18]);
                    exibid_dial(dialogo[19]);
                    exibid_dial(dialogo[20]);
                    firstMistake = false;
                }
                // Show warning message before question immediately
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
        draw_health_hud();
        // Central black box
        const int boxW = 600, boxH = 250;
        int boxX = (GetScreenWidth() - boxW) / 2;
        int boxY = (GetScreenHeight() - boxH) / 2;
        Rectangle box = {(float)boxX, (float)boxY, (float)boxW, (float)boxH};
        DrawRectangleRec(box, BLACK);
        DrawRectangleLinesEx(box, 4, WHITE);
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
        dex.health -= 35;
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

        if (firstQuestionMistake)
        {
            exibid_dial(dialogo[21]);
            firstQuestionMistake = false;
        }

        while (true)
        {
            if (WindowShouldClose())
                return;
            UpdateMusicStream(bgm);
            BeginDrawing();
            // Background for warning screen: Use gameplay background (sky color + floor tiles)
            draw_gameplay_background();
            draw_health_hud();
            const char *msg1 = "dex recebeu 35 de dano!";
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
            DrawRectangleLinesEx(wbox, 4, WHITE);
            DrawText(msg1, boxX + pad, boxY + pad, msgFontSize, WHITE);
            DrawText(msg2, boxX + pad, boxY + pad + msgFontSize + pad, msgFontSize, WHITE);
            EndDrawing();
            if (IsKeyPressed(KEY_ENTER))
                break;
        }
    }
    else
    {
        // Correct answer – show success box with hint about previous numeric guess

        if (firstQuestionCorrect)
        {
            exibid_dial(dialogo[22]);
            firstQuestionCorrect = false;
        }
        while (true)
        {
            if (WindowShouldClose())
                return;
            UpdateMusicStream(bgm);
            BeginDrawing();
            // Background for success screen: Use gameplay background (sky color + floor tiles)
            draw_gameplay_background();
            draw_health_hud();
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
            DrawRectangleLinesEx(wbox, 4, WHITE);
            DrawText(msg1, boxX + pad, boxY + pad, msgFontSize, WHITE);
            DrawText(hint, boxX + pad, boxY + pad + msgFontSize + pad, msgFontSize, WHITE);
            EndDrawing();
            if (IsKeyPressed(KEY_ENTER))
                break;
        }
    }
}

// Public API – runs the full flow (intro then gameplay)
Session executar_partida()
{
    // Load character portraits
    load_actor_textures();

    // Ensure music is playing from the start
    PlayMusicStream(bgm);
    // Main loop – just keep drawing background while intro or gameplay runs
    introducao();
    gameplay();

    // Unload character portraits before exiting
    unload_actor_textures();
    // Return the populated session data
    return current_session;
}