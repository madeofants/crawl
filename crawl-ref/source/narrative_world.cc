/**
 * @file
 * @brief Narrative world generation implementation.
 **/

#include "AppHdr.h"

#include "narrative_world.h"

#include "dungeon.h"
#include "env.h"
#include "player.h"
#include "random.h"
#include "state.h"

// Global world instance
static CubePlanet* g_narrative_world = nullptr;

CubePlanet* get_narrative_world()
{
    return g_narrative_world;
}

void set_narrative_world(CubePlanet* world)
{
    if (g_narrative_world)
        delete g_narrative_world;
    g_narrative_world = world;
}

// ============================================================================
// CubePlanet Implementation
// ============================================================================

CubePlanet::CubePlanet()
    : seed(0), name("Unnamed World")
{
}

CubePlanet::CubePlanet(uint64_t world_seed)
    : seed(world_seed), name("Unnamed World")
{
}

CubeFace& CubePlanet::get_or_create_face(int face_id)
{
    if (!faces.count(face_id))
        faces[face_id] = CubeFace(80, 70);
    return faces[face_id];
}

const CubeFace& CubePlanet::get_face(int face_id) const
{
    auto it = faces.find(face_id);
    if (it == faces.end())
        die("Attempted to access non-existent face");
    return it->second;
}

CubeFace& CubePlanet::get_face_mutable(int face_id)
{
    if (!faces.count(face_id))
        die("Attempted to access non-existent face");
    return faces[face_id];
}

void CubePlanet::generate_face(int face_id, terrain_type fill_terrain)
{
    CubeFace& face = get_or_create_face(face_id);
    face.fill_terrain(fill_terrain);
}

void CubePlanet::populate_dcss_env(int active_face_id)
{
    const CubeFace& face = get_face(active_face_id);

    // Populate DCSS environment grid with face tiles
    for (int x = 0; x < face.width && x < GXM; x++)
    {
        for (int y = 0; y < face.height && y < GYM; y++)
        {
            const WorldTile& tile = face.get_tile(x, y);
            dungeon_feature_type feat = terrain_to_feature(tile.terrain);
            env.grid(x, y) = feat;
        }
    }

    // Fill any remaining grid space (if face is smaller than grid)
    for (int x = face.width; x < GXM; x++)
        for (int y = 0; y < GYM; y++)
            env.grid(x, y) = DNGN_ROCK_WALL;

    for (int y = face.height; y < GYM; y++)
        for (int x = 0; x < GXM; x++)
            env.grid(x, y) = DNGN_ROCK_WALL;
}

// ============================================================================
// Terrain Conversion
// ============================================================================

dungeon_feature_type terrain_to_feature(terrain_type t)
{
    switch (t)
    {
    case TERRAIN_OCEAN:
        return DNGN_DEEP_WATER;
    case TERRAIN_BEACH:
        return DNGN_FLOOR;  // Placeholder
    case TERRAIN_GRASS:
        return DNGN_FLOOR;
    case TERRAIN_MOUNTAIN:
        return DNGN_ROCK_WALL;
    case TERRAIN_FOREST:
        return DNGN_FLOOR;
    default:
        return DNGN_FLOOR;
    }
}

// ============================================================================
// World Generation
// ============================================================================

void narrative_world_generate()
{
    // Create the world
    CubePlanet* world = new CubePlanet(crawl_state.seed);
    world->set_name("Ocean World");

    // MVP: Generate 1 ocean face (all ocean)
    world->generate_face(FACE_NORTH, TERRAIN_OCEAN);

    // Populate DCSS environment with the generated face
    world->populate_dcss_env(FACE_NORTH);

    // Store world for later access (player exploration, etc)
    set_narrative_world(world);

    // Place player at center of map
    coord_def start_pos(GXM / 2, GYM / 2);
    you.moveto(start_pos);
}
