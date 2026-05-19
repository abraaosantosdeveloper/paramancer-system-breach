#include "analise.h"
#include <math.h>

// Funcoes de analise para estatisticas das partidas.

// Soma recursiva: Caso base n == 0 [cite: 51, 179]
int soma_tentativas_recursiva(Session sessoes[], int n)
{
    // Soma recursiva para evitar laços em uma implementação didática.
    // Caso base da recursao.
    if (n <= 0)
        return 0;
    // Soma o elemento atual e segue para o restante do vetor.
    return sessoes[n - 1].attempts_count + soma_tentativas_recursiva(sessoes, n - 1);
}

// Soma dos quadrados para o Desvio Padrão [cite: 161, 182]
double soma_quadrados_recursiva(Session sessoes[], int n, double media)
{
    // Soma dos desvios ao quadrado para o desvio padrão.
    // Caso base da recursao.
    if (n <= 0)
        return 0.0;
    // Calcula o desvio da sessao atual.
    double diff = sessoes[n - 1].attempts_count - media;
    // Soma o quadrado do desvio com o restante.
    return (diff * diff) + soma_quadrados_recursiva(sessoes, n - 1, media);
}

Stats processar_historico(Session sessoes[], int n)
{
    // Inicializa a struct de estatisticas.
    Stats st = {0};
    // Valida a quantidade de sessoes.
    if (n <= 0)
        return st;

    // Calcula media e desvio padrão de tentativas das sessões.
    // Media de tentativas.
    st.average_attempts = (double)soma_tentativas_recursiva(sessoes, n) / n;
    // Soma dos quadrados e desvio padrão.
    double soma_q = soma_quadrados_recursiva(sessoes, n, st.average_attempts);
    st.stddev_attempts = sqrt(soma_q / n);

    // Calcula melhor (menor) e pior (maior) número de tentativas.
    st.best_attempts = sessoes[0].attempts_count;
    st.worst_attempts = sessoes[0].attempts_count;
    int sum_low = 0;
    int sum_high = 0;
    for (int i = 0; i < n; i++) {
        if (sessoes[i].attempts_count < st.best_attempts) st.best_attempts = sessoes[i].attempts_count;
        if (sessoes[i].attempts_count > st.worst_attempts) st.worst_attempts = sessoes[i].attempts_count;
        sum_low += sessoes[i].low_count;
        sum_high += sessoes[i].high_count;
    }
    // Médias de viés baixo e alto.
    st.avg_low_bias = (double)sum_low / n;
    st.avg_high_bias = (double)sum_high / n;

    // Retorna as estatísticas calculadas.
    return st;
}