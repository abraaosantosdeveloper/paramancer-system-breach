#ifndef TIPOS_H
#define TIPOS_H

// Numero maximo de palpites armazenados por sessao.
#define MAX_GUESSES 100

// Dados de uma sessao de jogo gravada no historico.
typedef struct Session
{
    char timestamp[32];       // Data/hora no formato CSV.
    int target;               // Numero alvo sorteado.
    int attempts_count;       // Total de tentativas.
    int guesses[MAX_GUESSES]; // Palpites realizados.
    int low_count;            // Palpites abaixo do alvo.
    int high_count;           // Palpites acima do alvo.
} Session;

// Estatisticas agregadas do historico.
typedef struct Stats
{
    double average_attempts; // Media de tentativas por sessao.
    int best_attempts;       // Melhor numero de tentativas.
    int worst_attempts;      // Pior numero de tentativas.
    double stddev_attempts;  // Desvio padrao de tentativas.
    double avg_low_bias;     // Media de palpites baixos.
    double avg_high_bias;    // Media de palpites altos.
} Stats;

#endif