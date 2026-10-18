#include "Weapons.h"
#include <iostream>

// --- Weapon Implementation ---

Weapon::Weapon(unsigned int range) : baseDamage(10), name("Default Weapon"), range(range) {}

Weapon::Weapon(unsigned int range, int weapon_damage, const std::string& weapon_name) 
    : baseDamage(weapon_damage), name(weapon_name), range(range) {}

Weapon::~Weapon() {
    std::cout << "[SYSTEM LOG]: Memory freed for Weapon '" << name << "'\n";
}

void Weapon::RecalculateDamage(int additionalDamage) {
    if (baseDamage + additionalDamage > 0) {
        baseDamage += additionalDamage;
    } else {
        baseDamage = 1; // Weapon damage cannot drop below 1
    }
}

unsigned int Weapon::GetRange() const { return range; }

AttackData Weapon::Attack() { return Attack(1.0); }

// --- Sword Implementation ---

Sword::Sword() : Weapon(DEFAULT_RANGE), length(DEFAULT_RANGE) {}
Sword::Sword(const std::string& name) : Weapon(DEFAULT_RANGE, DEFAULT_DAMAGE, name), length(DEFAULT_RANGE) {}
Sword::Sword(unsigned int length) : Weapon(length), length(length) {}
Sword::Sword(unsigned int sword_length, int sword_damage, const std::string& sword_name) 
    : Weapon(sword_length, sword_damage, sword_name), length(sword_length) {}

unsigned int Sword::GetLength() const { return length; }

AttackData Sword::Attack(double modificator) {
    auto modif_damage = modificator * baseDamage;
    RecalculateDamage(-baseDamage * WEAR_COEFF / 100); // Not ideal, redo
    return {true, static_cast<int>(modif_damage), AttackData::ActionVerb::Slashes, name};
}

// --- Bow Implementation ---

Bow::Bow() : Weapon(DEFAULT_RANGE), arrows(DEFAULT_COUNT, Arrow(baseDamage)) {}

Bow::Bow(unsigned int bow_range, int bow_damage, const std::string& bow_name, unsigned int arrowCount) 
    : Weapon(bow_range, bow_damage, bow_name), 
        arrows(arrowCount == 0 
            ? std::vector<Arrow> {} 
            : std::vector<Arrow>(arrowCount, Arrow(baseDamage))) {}

Bow::Bow(int bow_damage, const std::string& bow_name, unsigned int arrowCount) 
    : Bow(DEFAULT_RANGE, bow_damage, bow_name, arrowCount) {}

void Bow::AddArrow(const Arrow& arrow) {
    arrows.push_back(arrow);
}

AttackData Bow::Attack(double modificator) {
    if (!arrows.empty()) {
        auto modif_damage = modificator * arrows.back().GetDamage();
        arrows.pop_back();
        return {true, static_cast<int>(modif_damage), AttackData::ActionVerb::Shoots, name};
    } else {
        return {false, 0, AttackData::ActionVerb::Shoots, name};
    }
}