#include <stdio.h>
#include <time.h>
#include <stdlib.h>

static void inicializarSeed(void)
{
    static int seed_inicializada = 0;

    if (!seed_inicializada)
    {
        srand((unsigned int)time(NULL));
        seed_inicializada = 1;
    }
}

// Sorteia um número para a função inicial do programa
int sortearNumero()
{
    // Define a seed uma única vez para evitar repetição entre chamadas próximas.
    inicializarSeed();
    int numero_sorteado = (rand() % 100) + 1;
    return numero_sorteado;
}

// Sorteia um número para id de pergunta
int sortearPergunta()
{
    inicializarSeed();

    // Carrega arquivo
    FILE *arquivo = fopen("perguntas.csv", "r");
    if (arquivo == NULL)
        return -1;

    int total_linhas = 0;
    char linha[2048];

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        if (linha[0] != '\n' && linha[0] != '\r' && linha[0] != '\0')
        {
            total_linhas++;
        }
    }

    fclose(arquivo);

    if (total_linhas <= 0)
    {
        return -1;
    }

    int pergunta_sorteada = (rand() % total_linhas) + 1;
    return pergunta_sorteada;
}
