/**
 * @file
 * @brief Narrative world generation and management.
 *
 * Implements a cube-planet data structure with per-face terrain generation.
 * MVP: Single ocean face. Scales to 6 faces by repeating the generation function.
 **/

#pragma once

#include <vector>
#include <map>
#include <cstdint>

using std::vector;
using std::map;

// Terrain types for the narrative world
enum terrain_type
{
    TERRAIN_OCEAN,      // Water (all tiles for MVP)
    TERRAIN_BEACH,      // Sand/beach (future)
    TERRAIN_GRASS,      // Land (future)
    TERRAIN_MOUNTAIN,   // Mountains (future)
    TERRAIN_FOREST,     // Trees (future)
    NUM_TERRAIN_TYPES
};

// Cube face identifiers (relative directions)
enum CubeFaceID
{
    FACE_NORTH = 0,
    FACE_SOUTH = 1,
    FACE_EAST = 2,
    FACE_WEST = 3,
    FACE_TOP = 4,
    FACE_BOTTOM = 5,
    NUM_FACES = 6
};

/**
 * A single tile in the world.
 * MVP: Only terrain type. Future: weather, entities, elevation, biome, etc.
 */
struct WorldTile
{
    terrain_type terrain;

    WorldTile() : terrain(TERRAIN_OCEAN) {}
    explicit WorldTile(terrain_type t) : terrain(t) {}
};

/**
 * A face of the cube planet.
 * Stores a 2D grid of tiles.
 * Can be extended with neighbor face connections, rotation info, etc.
 */
struct CubeFace
{
    int width;
    int height;
    vector<vector<WorldTile>> tiles;

    // Neighbors: which face is adjacent in each direction (for later)
    int neighbor_north;
    int neighbor_south;
    int neighbor_east;
    int neighbor_west;

    CubeFace()
        : width(80), height(70),
          neighbor_north(-1), neighbor_south(-1),
          neighbor_east(-1), neighbor_west(-1)
    {
        tiles.resize(width, vector<WorldTile>(height));
    }

    CubeFace(int w, int h)
        : width(w), height(h),
          neighbor_north(-1), neighbor_south(-1),
          neighbor_east(-1), neighbor_west(-1)
    {
        tiles.resize(width, vector<WorldTile>(height));
    }

    /**
     * Get tile at position. Bounds-checked.
     */
    WorldTile& get_tile(int x, int y)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return tiles[0][0]; // Safe fallback
        return tiles[x][y];
    }

    const WorldTile& get_tile(int x, int y) const
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return tiles[0][0];
        return tiles[x][y];
    }

    /**
     * Fill entire face with a single terrain type.
     */
    void fill_terrain(terrain_type terrain)
    {
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
                tiles[x][y].terrain = terrain;
    }
};

/**
 * The cube planet.
 * Stores all 6 faces. MVP only generates 1 (FACE_NORTH).
 * Scales to 6 by calling face generation function in a loop.
 */
class CubePlanet
{
private:
    map<int, CubeFace> faces;  // FaceID -> face data
    uint64_t seed;
    string name;

public:
    CubePlanet();
    explicit CubePlanet(uint64_t world_seed);

    /**
     * Get or create a face by ID.
     */
    CubeFace& get_or_create_face(int face_id);

    /**
     * Check if face exists.
     */
    bool has_face(int face_id) const
    {
        return faces.count(face_id) > 0;
    }

    /**
     * Get existing face (const).
     */
    const CubeFace& get_face(int face_id) const;

    /**
     * Get mutable face reference.
     */
    CubeFace& get_face_mutable(int face_id);

    /**
     * Generate a single face with given terrain.
     * Called once per face - repeatable for all 6 faces.
     */
    void generate_face(int face_id, terrain_type fill_terrain);

    /**
     * Populate DCSS environment grid with specified face.
     * Converts world tile data to DCSS dungeon features.
     */
    void populate_dcss_env(int active_face_id);

    uint64_t get_seed() const { return seed; }
    void set_seed(uint64_t s) { seed = s; }

    string get_name() const { return name; }
    void set_name(const string& n) { name = n; }
};

/**
 * Main entry point: Generate the narrative world at game startup.
 * Called from initial_dungeon_setup() when game_type is NARRATIVE.
 *
 * MVP: Generates 1 ocean face and populates DCSS environment.
 */
void narrative_world_generate();

/**
 * Convert terrain type to DCSS dungeon feature.
 */
dungeon_feature_type terrain_to_feature(terrain_type t);

/**
 * Get the global narrative world (if initialized).
 */
CubePlanet* get_narrative_world();
void set_narrative_world(CubePlanet* world);
