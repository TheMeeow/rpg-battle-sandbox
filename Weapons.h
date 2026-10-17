#pragma once
#include <string>
#include <vector>
#include "CoreTypes.h"

/**
 * @brief Base abstract class for all weapons in the game.
 */
class Weapon {
protected:
    std::string name;
    int baseDamage;
    unsigned int range;

    /**
     * @brief Modifies the base damage of the weapon (e.g., due to wear or upgrades)
     * @param additionalDamage Value added to the current damage (can be negative)
     */
    void RecalculateDamage(int additionalDamage);

public:
    explicit Weapon(unsigned int range);
    Weapon(unsigned int range, int weapon_damage, const std::string& weapon_name);
    virtual ~Weapon();

    unsigned int GetRange() const;

    /**
     * @brief Calculates the final damage and applies weapon effects (e.g., wear and tear)
     * @param modificator Damage multiplier (buffs, critical hits, etc.)
     * @return AttackData Packet containing information about the performed attack
     */
    virtual AttackData Attack(double modificator) = 0;
    virtual AttackData Attack();
};

/**
 * @brief Melee weapon. Degrades with each use.
 */
class Sword : public Weapon {
private:
    unsigned int length;
    static constexpr unsigned int DEFAULT_RANGE = 5;
    static constexpr int DEFAULT_DAMAGE = 20;
    static constexpr int WEAR_COEFF = 10;

public:
    using Weapon::Attack;

    Sword();
    explicit Sword(const std::string& name);
    explicit Sword(unsigned int length);
    Sword(unsigned int sword_length, int sword_damage, const std::string& sword_name);

    unsigned int GetLength() const;
    AttackData Attack(double modificator) override;
};

/**
 * @brief Ranged weapon. Requires projectiles (arrows) to perform an attack.
 */
class Bow : public Weapon {
private:
    std::vector<Arrow> arrows;
    static constexpr unsigned int DEFAULT_COUNT = 5;
    static constexpr unsigned int DEFAULT_RANGE = 10;

public:
    using Weapon::Attack;

    Bow();
    Bow(unsigned int bow_range, int bow_damage, const std::string& bow_name, unsigned int arrowCount);
    Bow(int bow_damage, const std::string& bow_name, unsigned int arrowCount = DEFAULT_COUNT);
    
    /**
     * @brief Adds an arrow to the quiver.
     * @param arrow The arrow object to add.
     */
    void AddArrow(const Arrow& arrow);
    AttackData Attack(double modificator) override;
};