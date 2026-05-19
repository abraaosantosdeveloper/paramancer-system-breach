#ifndef PERGUNTAS_H
#define PERGUNTAS_H

// Tamanhos maximos de texto para perguntas e alternativas.
#define MAX_TEXTO_PERGUNTA 256
#define MAX_TEXTO_ALTERNATIVA 128

// Representa uma pergunta e suas alternativas.
typedef struct
{
    char enunciado[MAX_TEXTO_PERGUNTA]; // Texto principal da pergunta.
    char alt_a[MAX_TEXTO_ALTERNATIVA];  // Alternativa A.
    char alt_b[MAX_TEXTO_ALTERNATIVA];  // Alternativa B.
    char alt_c[MAX_TEXTO_ALTERNATIVA];  // Alternativa C.
    char alt_d[MAX_TEXTO_ALTERNATIVA];  // Alternativa D.
    char correta;                       // Letra da alternativa correta.
} Pergunta;

// Carrega a pergunta pelo indice (linha) no CSV.
int carregar_pergunta_por_id(int id, Pergunta *out);

#endif
