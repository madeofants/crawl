#include "narrator_control.h"
#include "newgame-def.h"
#include "ng-setup.h"
#include "species.h"
#include "jobs.h"
#include "player.h"
#include "command.h"
#include "state.h"
#include <algorithm>
#include <cctype>

/**
 * Narrator Control Implementation - MVP
 * 
 * Wraps existing DCSS functions to allow programmatic game control
 * without user interface.
 */

// Convert string to lowercase
static std::string to_lower(const std::string& s)
{
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Convert species name to ID
static species_type name_to_species(const std::string& name_str)
{
    std::string lower_name = to_lower(name_str);
    
    for (int i = SP_HUMAN; i < NUM_SPECIES; i++)
    {
        species_type sp = static_cast<species_type>(i);
        if (to_lower(species_name(sp)) == lower_name)
            return sp;
    }
    return SP_UNKNOWN;
}

// Convert job name to ID
static job_type name_to_job(const std::string& name_str)
{
    std::string lower_name = to_lower(name_str);
    
    for (int i = JOB_FIGHTER; i < NUM_JOBS; i++)
    {
        job_type job = static_cast<job_type>(i);
        if (to_lower(job_name_aux(job, 0)) == lower_name)
            return job;
    }
    return JOB_INVALID;
}

bool narrator_create_game(const std::string& name,
                          const std::string& species_name,
                          const std::string& job_name)
{
    // Convert names to IDs
    species_type species = name_to_species(species_name);
    job_type job = name_to_job(job_name);
    
    // Validate inputs
    if (species == SP_UNKNOWN || job == JOB_INVALID)
        return false;
    
    if (!is_species_valid(species) || !is_job_valid(job))
        return false;
    
    if (!is_good_combination(species, job))
        return false;
    
    // Create character definition
    newgame_def ng;
    ng.name = name;
    ng.type = GAME_TYPE_NORMAL;
    ng.species = species;
    ng.job = job;
    ng.weapon = -1;
    ng.fully_random = false;
    
    // Start the game
    // This automatically generates the world
    try
    {
        setup_game(ng);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
    catch (...)
    {
        return false;
    }
}

int narrator_run_turns(int turn_count)
{
    int turns_executed = 0;
    
    for (int i = 0; i < turn_count; i++)
    {
        // Check if game is still running
        if (you.is_dead())
            break;
        
        if (crawl_state.state != crawl_state_type::GAME)
            break;
        
        // Execute one turn: player waits, world simulates
        // Use CMD_REST to do nothing while world advances
        process_command(CMD_REST);
        
        turns_executed++;
    }
    
    return turns_executed;
}

bool narrator_is_game_running()
{
    if (you.is_dead())
        return false;
    
    if (crawl_state.state != crawl_state_type::GAME)
        return false;
    
    return true;
}
