#include <stdlib.h>
#include <time.h>
#include "geracao.h"

// Generates a simple map with a solid floor at the bottom
void gerar_plataformas(char mapa[MAP_ALTURA][MAP_LARGURA])
{
    // Fill all rows with floor tiles
    for (int y = 0; y < MAP_ALTURA; y++)
    {
        for (int x = 0; x < MAP_LARGURA; x++)
        {
            mapa[y][x] = '#';
        }
    }
}
