#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LARGURA 64 // Comprimento horizontal do nível
#define ALTURA 24  // Altura da tela

void gerar_plataformas(char mapa[ALTURA][LARGURA])
{
    // Inicializa o mapa com espaços vazios
    for (int y = 0; y < ALTURA; y++)
    {
        for (int x = 0; x < LARGURA; x++)
        {
            mapa[y][x] = '.';
        }
    }

    // Cria o chão na parte inferior do mapa
    for (int x = 0; x < LARGURA; x++)
    {
        mapa[ALTURA - 1][x] = '#';
    }

    // Geração procedural das plataformas
    int altura_atual = ALTURA - 4; // Padrão inicial da primeira plataforma
    int espaco_horizontal = 0;

    for (int x = 0; x < LARGURA; x++)
    {
        // Reduz o espaço horizontal a cada bloco percorrido
        if (espaco_horizontal > 0)
        {
            espaco_horizontal--;
        }
        else
        {
            // Define o tamanho da plataforma atual (entre 2 e 5 blocos)
            int tamanho_plataforma = rand() % 4 + 2;

            // Posiciona os blocos da plataforma no mapa
            for (int p = 0; p < tamanho_plataforma && x < LARGURA; p++)
            {
                if (altura_atual > 2)
                {
                    mapa[altura_atual][x] = '#';
                }
                x++;
            }
            x--; // Ajusta o loop principal

            // Define a distância horizontal para a próxima plataforma (entre 1 e 3)
            espaco_horizontal = rand() % 3 + 1;

            // Define a nova altura (garante que não ultrapasse as bordas)
            int variacao_altura = (rand() % 3) - 1; // -1, 0 ou +1 (desce, reto, sobe)
            altura_atual += variacao_altura;

            if (altura_atual >= ALTURA - 2)
                altura_atual = ALTURA - 4;
            if (altura_atual < 4)
                altura_atual = 4;
        }
    }
}

void imprimir_mapa(char mapa[ALTURA][LARGURA])
{
    for (int y = 0; y < ALTURA; y++)
    {
        for (int x = 0; x < LARGURA; x++)
        {
            printf("%c", mapa[y][x]);
        }
        printf("\n");
    }
}

int main()
{
    char mapa[ALTURA][LARGURA];

    // Inicializa a semente aleatória baseada no tempo
    srand(time(NULL));

    gerar_plataformas(mapa);
    imprimir_mapa(mapa);
    getchar();

    return 0;
}