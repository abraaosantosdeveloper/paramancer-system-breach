#include "map_render.h"
#include <stdio.h>

// Loads the floor tile texture from the asset path
// This texture is used as a sprite for each platform block in the procedural map.
// Each tile is 64×128 pixels and represents one character position in the map grid.
//
// Returns: loaded Texture2D on success, or a texture with id=0 on failure
// Pre-condition: Asset path must be valid
// Post-condition: Caller must call UnloadTexture() when done
Texture2D map_load_floor_texture(void)
{
    Texture2D floorTexture = LoadTexture("assets/game_arts/scenario/floor.png");

    // Validate that the texture loaded successfully
    if (floorTexture.id == 0)
    {
        fprintf(stderr, "[ERROR] Failed to load floor tile texture from assets/game_arts/scenario/floor.png\n");
        return floorTexture; // Return the invalid texture so caller can check floorTexture.id == 0
    }

    return floorTexture;
}

// Renders the procedurally generated platform map using floor tile sprites
// This function is the core of the floor rendering pipeline:
// 1) Validates the input texture (must have id != 0)
// 2) Iterates through the 2D map grid (MAP_ALTURA rows × MAP_LARGURA columns)
// 3) For each platform block ('#'), calculates screen position and draws the tile
//
// Parameters:
//   map: 2D character array with '#' = platform block, '.' = empty space
//   floorTexture: loaded texture asset (64×128 pixels)
//   offsetX, offsetY: screen position offset for the map origin (top-left corner)
//
// Returns: true if rendering succeeded, false if texture is invalid
// Uses: TILE_SIZE = 64 pixels (defined in geracao.h), MAP_ALTURA and MAP_LARGURA dimensions
bool map_render_platforms(const char map[MAP_ALTURA][MAP_LARGURA],
                          Texture2D floorTexture,
                          float offsetX, float offsetY)
{
    // Validate texture is loaded
    if (floorTexture.id == 0)
    {
        fprintf(stderr, "[ERROR] Invalid floor texture (id=0). Cannot render map.\n");
        return false;
    }

    // Iterate through the map and draw floor tiles for each platform block
    // Row-major order: y (vertical) then x (horizontal)
    for (int y = 0; y < MAP_ALTURA; y++)
    {
        for (int x = 0; x < MAP_LARGURA; x++)
        {
            // Only render tiles where there's a platform block
            if (map[y][x] == '#')
            {
                // Calculate screen position for this tile
                // Each map position maps to a TILE_SIZE × TILE_SIZE pixel area
                float screenX = offsetX + (float)(x * TILE_SIZE);
                float screenY = offsetY + (float)(y * TILE_SIZE);

                // Draw the floor sprite at the computed position
                // Parameters: texture, position (top-left corner), rotation, scale, color tint
                DrawTextureEx(floorTexture,
                              (Vector2){screenX, screenY},
                              0.0f,   // rotation: 0 degrees (no rotation)
                              1.0f,   // scale: 1.0 = normal size (adjust if needed for UI scaling)
                              WHITE); // tint: WHITE = no color modification
            }
        }
    }

    return true;
}
