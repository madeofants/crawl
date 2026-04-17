// test_harness.cc - Narrator Control MVP Tests
// Tests narrator_control functions for MVP verification
#include <iostream>
#include "narrator_control.h"

int main() {
    std::cout << "=== Narrator Control MVP Test Harness ===" << std::endl;
    std::cout << std::endl;
    
    // Test 1: Create a game
    std::cout << "TEST 1: Character creation (narrator_create_game)" << std::endl;
    bool created = narrator_create_game("TestChar", "human", "fighter");
    if (created && narrator_is_game_running()) {
        std::cout << "✓ Game created and running successfully" << std::endl;
    } else {
        std::cout << "✗ FAILED: Could not create game" << std::endl;
        return 1;
    }
    
    // Test 2: Run some turns
    std::cout << std::endl;
    std::cout << "TEST 2: Turn execution (narrator_run_turns)" << std::endl;
    int turns_run = narrator_run_turns(5);
    if (turns_run == 5) {
        std::cout << "✓ Executed " << turns_run << " turns successfully" << std::endl;
    } else {
        std::cout << "✗ FAILED: Expected 5 turns, got " << turns_run << std::endl;
        return 1;
    }
    
    // Test 3: Verify game still running
    std::cout << std::endl;
    std::cout << "TEST 3: Game status check (narrator_is_game_running)" << std::endl;
    if (narrator_is_game_running()) {
        std::cout << "✓ Game is still running after turns" << std::endl;
    } else {
        std::cout << "✗ FAILED: Game ended unexpectedly" << std::endl;
        return 1;
    }
    
    // Test 4: Run more turns
    std::cout << std::endl;
    std::cout << "TEST 4: Extended turn execution" << std::endl;
    turns_run = narrator_run_turns(10);
    if (turns_run >= 10) {
        std::cout << "✓ Executed " << turns_run << " more turns successfully" << std::endl;
    } else {
        std::cout << "✗ FAILED: Expected 10+ turns, got " << turns_run << std::endl;
        return 1;
    }
    
    std::cout << std::endl;
    std::cout << "=== ALL TESTS PASSED ===" << std::endl;
    std::cout << "MVP verification complete: world generation and turn simulation working" << std::endl;
    return 0;
}
