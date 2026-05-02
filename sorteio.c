#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorteio.h"

// Inicializa a semente do RNG apenas uma vez.
static void inicializar_seed(void)
{
    static int seed_inicializada = 0;
    // Protege contra inicializacao dupla.
    if (!seed_inicializada)
    {
        // Inicializa o RNG uma unica vez por execucao.
        srand((unsigned int)time(NULL));
        seed_inicializada = 1;
    }
}

// Sorteia um numero entre 1 e 100.
int sortearNumero(void)
{
    // Garante que a seed do RNG foi configurada.
    inicializar_seed();
    return (rand() % 100) + 1;
}

// Sorteia um indice de pergunta valido no CSV.
int sortearPergunta(void)
{
    // Garante que a seed do RNG foi configurada.
    inicializar_seed();

    static int total_linhas = -1;
    // Conta as linhas do CSV apenas na primeira chamada.
    if (total_linhas < 0)
    {
        // Conta as linhas validas apenas uma vez e reutiliza.
        FILE *arquivo = fopen("perguntas.csv", "r");
        if (arquivo == NULL)
            return -1;

        total_linhas = 0;
        char linha[2048];

        // Conta somente linhas com conteudo.
        while (fgets(linha, sizeof(linha), arquivo) != NULL)
        {
            if (linha[0] != '\n' && linha[0] != '\r' && linha[0] != '\0')
                total_linhas++;
        }

        fclose(arquivo);
    }

    // Retorna erro se o CSV estiver vazio.
    if (total_linhas <= 0)
        return -1;

    // Sorteia um indice valido dentro do total de linhas.
    return (rand() % total_linhas) + 1;
}
