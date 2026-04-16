#ifndef NARRATOR_CONTROL_H
#define NARRATOR_CONTROL_H

#include <string>

/**
 * Narrator Control API - MVP
 * 
 * Minimal interface to:
 * 1. Create a character programmatically
 * 2. Execute game actions
 * 3. Check game status
 * 
 * All functions are C++ wrappers around existing DCSS functions.
 * Goal: "Start a game and have a world generated" - DCSS already does this!
 */

/**
 * Create and start a new game with a character
 * 
 * @param name Character name (e.g., "Adventurer")
 * @param species Species name (e.g., "human", "elf")
 * @param job Job name (e.g., "fighter", "mage")
 * @return true if game started successfully, false on error
 * 
 * This bypasses the DCSS UI menu and directly initializes the game.
 * setup_game() automatically generates the world.
 */
bool narrator_create_game(const std::string& name,
                          const std::string& species,
                          const std::string& job);

/**
 * Execute N turns of the game loop
 * 
 * Each turn:
 * - Player waits (no action)
 * - Monsters move and act  
 * - Projectiles move
 * - Effects tick down
 * - World state updates
 * 
 * @param turn_count Number of turns to execute
 * @return Number of turns actually executed (may be less if game ended)
 */
int narrator_run_turns(int turn_count);

/**
 * Check if the game is still running
 * @return true if game is active, false if player died or quit
 */
bool narrator_is_game_running();

#endif
