#pragma once
#include <string>

/**
 * @brief Data Transfer Object describing the result of a weapon attack.
 */
struct AttackData {
    bool success;            ///< Flag indicating if the attack was successful (e.g., enough ammo)
    int damage;              ///< Calculated damage amount
    std::string actionVerb;  ///< Verb describing the action (e.g., "attacks", "shoots")
    std::string weaponName;  ///< Name of the used weapon
};

/**
 * @brief Data structure describing AI state and actions during a single game tick.
 */
struct AIUpdateReport {
    enum class State { Idling, TargetLost, Attacking, Dead };
    
    State state;             ///< Current state of the AI
    std::string actorName;   ///< Name of the entity owning the AI
    std::string targetName;  ///< Name of the pursued target (if any)
    AttackData attackInfo;   ///< Attack data (populated only in the Attacking state)
};

/**
 * @brief Ammunition class for ranged weapons. 
 * P.S. Game logic can be supplemented with the ability to find arrows.
 */
class Arrow {
private: 
    int damage;
public:
    explicit Arrow(int damage) : damage(damage) {}
    int GetDamage() const { return damage; }
};