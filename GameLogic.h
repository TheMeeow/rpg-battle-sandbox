#pragma once
#include <vector>
#include <memory>
#include "Actors.h"

/**
 * @brief Determines who attacks first based on attack range.
 */
bool FirstHasAdvantage(MyActor& actor1, MyActor& actor2);

/**
 * @brief Conducts a forced battle (like arena duel) between two entities.
 * @param rounds Number of attack exchanges.
 */
void Battle(MyActor& actor1, MyActor& actor2, unsigned int rounds);

/**
 * @brief Polls the list of enemies and outputs their actions to the console (AI UI layer).
 */
void AIUpdate(const std::vector<std::shared_ptr<Enemy>>& enemies);