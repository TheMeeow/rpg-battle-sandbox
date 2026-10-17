#include "Actors.h"
#include <iostream>
#include <algorithm>

// --- MyActor Implementation ---

MyActor::MyActor(const std::string& name) : name(name) {}
MyActor::MyActor() : MyActor("Default") {}

MyActor::~MyActor() {
    std::cout << "[SYSTEM LOG]: Memory freed for Actor '" << name << "'\n";
}

std::string MyActor::GetName() const { return name; }
int MyActor::GetHealth() const { return health; }
bool MyActor::IsDead() const { return !isAlive; }
unsigned int MyActor::GetAttackRange() const { return 1; }

int MyActor::TakeDamage(int damage) {
    if (damage <= 0 || !isAlive) return 0;
    
    int actualDamage = std::min(health, damage);
    health -= actualDamage;
    
    if (health <= 0) Die();
    return actualDamage;
}

void MyActor::Die() {
    health = 0;
    isAlive = false;
}

// --- Player Implementation ---

Player::Player(const std::string& name) : MyActor(name), myWeapon(std::make_unique<Sword>()) {}

Player::Player(const std::string& name, std::unique_ptr<Weapon> newWeapon) 
    : MyActor(name), myWeapon(std::move(newWeapon)) {}

unsigned int Player::GetAttackRange() const { 
    return myWeapon ? myWeapon->GetRange() : 1; 
}

AttackData Player::Attack(MyActor& target) {
    if (IsDead() || target.IsDead()) return {false, 0, "", ""};

    AttackData weaponAttack = myWeapon ? myWeapon->Attack() : AttackData{true, 1, "punches", "bare hands"};
    
    int dealtDamage = 0;
    if (weaponAttack.success) {
        dealtDamage = target.TakeDamage(weaponAttack.damage);
    }

    return {weaponAttack.success, dealtDamage, weaponAttack.actionVerb, weaponAttack.weaponName};
}

void Player::Die() {
    MyActor::Die();
    myWeapon.reset(); // Player drops the weapon upon death
}

// --- Enemy Implementation ---

Enemy::Enemy(const std::string& name, int damage, std::weak_ptr<Player> target) 
    : MyActor(name), myDamage(damage), target(std::move(target)) {}

Enemy::Enemy(const std::string& name, std::weak_ptr<Player> target) 
    : MyActor(name), myDamage(DEFAULT_ENEMY_DAMAGE), target(std::move(target)) {}

Enemy::Enemy(std::weak_ptr<Player> target) 
    : MyActor(), myDamage(DEFAULT_ENEMY_DAMAGE), target(std::move(target)) {}

AttackData Enemy::Attack(MyActor& targetActor) {
    if (IsDead() || targetActor.IsDead()) return {false, 0, "", ""};
    
    int dealtDamage = targetActor.TakeDamage(myDamage);
    return {true, dealtDamage, "bites", "fangs"};
}

void Enemy::Die() {
    MyActor::Die();
    target.reset(); // Enemy leaves it's target upon death
}

AIUpdateReport Enemy::Update() {
    if (IsDead()) {
        return {AIUpdateReport::State::Dead, GetName(), "", {}};
    }
    
    if (auto t_ptr = target.lock()) {
        if (t_ptr->IsDead()) {
            target.reset();
            return {AIUpdateReport::State::TargetLost, GetName(), t_ptr->GetName(), {}};
        } else {
            return {AIUpdateReport::State::Attacking, GetName(), t_ptr->GetName(), this->Attack(*t_ptr)};
        }
    }

    return {AIUpdateReport::State::Idling, GetName(), "", {}};
}