// Header for actor structs used in the game
#ifndef ACTORS_H
#define ACTORS_H

// Dados do personagem principal (Dex)
typedef struct {
    int health;         /* Always in integer: 1 - 100 - Each unit corresponds to half a heart */
    int buffer;         /* Each number points to a different buffer. Example: 1 = Wrong answer protection */
    int spriteState;    /* For now, we have 2 states: idle, typing number on the watch */
    int correctAnswers; /* Number of correctly given answers */
    int score;          /* Overall player score */
} Dex;

// Dados basicos das entidades inimigas (Entity)
typedef struct {
    int health;
    int spriteState;
    int entityType;
} Entity;

// Definicao de um buff aplicado ao jogador.
typedef struct {
    int id;
    char name[10];
    char description[60];
} Buffer;

#endif // ACTORS_H
