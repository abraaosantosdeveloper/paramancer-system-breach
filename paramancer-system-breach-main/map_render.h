#ifndef MAP_RENDER_H
#define MAP_RENDER_H

#include <raylib.h>
#include "geracao.h"

// Renders the procedurally generated platform map using floor tile sprites
// Parameters:
//   map: 2D character array with '#' = platform, '.' = empty
//   floorTexture: loaded texture to use for rendering tiles
//   offsetX, offsetY: screen position offset for the map origin (top-left corner)
// Returns: true if rendering succeeded, false if texture is invalid
bool map_render_platforms(const char map[MAP_ALTURA][MAP_LARGURA],
                          Texture2D floorTexture,
                          float offsetX, float offsetY);

// Loads the floor tile texture from the asset path
// Returns: loaded Texture2D on success, or a texture with id=0 on failure
// Caller is responsible for unloading the texture with UnloadTexture()
Texture2D map_load_floor_texture(void);

#endif // MAP_RENDER_H
