#pragma once
#include <string>
#include <memory>
#include "Weapons.h"

/**
 * @brief Base class for all interactive entities in the level (players, enemies).
 */
class MyActor {
private:
    std::string name;
    int health = 100;
    bool isAlive = true;

public:
    explicit MyActor(const std::string& name);
    MyActor();
    virtual ~MyActor();

    std::string GetName() const;
    int GetHealth() const;
    bool IsDead() const;

    /**
     * @brief Returns the entity's attack range (used for calculating initiative).
     */
    virtual unsigned int GetAttackRange() const;   

    /**
     * @brief Initiates an attack on the chosen target
     * @param other Reference to the target entity
     * @return AttackData Result of the performed attack
     */
    virtual AttackData Attack(MyActor& other) = 0;
    
    /**
     * @brief Safely subtracts health points.
     * @param damage Incoming damage.
     * @return The actual amount of health subtracted (preventing negative health)
     */
    int TakeDamage(int damage);

    virtual void Die();
};

/**
 * @brief Playable character class. Contains an inventory (weapon)
 */
class Player : public MyActor {
private:
    std::unique_ptr<Weapon> myWeapon;

public:
    explicit Player(const std::string& name);
    Player(const std::string& name, std::unique_ptr<Weapon> newWeapon);
    ~Player() override = default;

    unsigned int GetAttackRange() const override;
    AttackData Attack(MyActor& target) override;
    void Die() override;
};

/**
 * @brief Class representing an AI-controlled opponent.
 */
class Enemy : public MyActor {
private:
    std::weak_ptr<Player> target; ///< Tracked target
    int myDamage;
    static constexpr int DEFAULT_ENEMY_DAMAGE = 5;

public:
    Enemy(const std::string& name, int damage, std::weak_ptr<Player> target);
    Enemy(const std::string& name, std::weak_ptr<Player> target);
    explicit Enemy(std::weak_ptr<Player> target);
    ~Enemy() override = default;

    AttackData Attack(MyActor& targetActor) override;
    void Die() override;

    /**
     * @brief Simulates AI logic for a single tick.
     * @return AIUpdateReport Report detailing the AI's decision.
     */
    AIUpdateReport Update();
};