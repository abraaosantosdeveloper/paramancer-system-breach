#ifndef GERACAO_H
#define GERACAO_H

// Tile dimensions
#define TILE_SIZE 64

// Floor dimensions – simple tiled floor at the bottom
// floor.png is 64×128 pixels
// 20 tiles × 64px = 1280px width (covers 1280px window exactly)
// 1 row × 128px height (complete image displayed, anchored at Y=592)
#define MAP_LARGURA 20 // Width: 20 tiles (1280px total)
#define MAP_ALTURA 1   // Height: 1 row (full 128px image)

// Generates a simple solid floor line (all '#') that tiles across the screen
// Maps are stored in row-major order: mapa[y][x]
// Pre-condition: mapa must be a valid 2D array of size [MAP_ALTURA][MAP_LARGURA]
void gerar_plataformas(char mapa[MAP_ALTURA][MAP_LARGURA]);

#endif // GERACAO_H
