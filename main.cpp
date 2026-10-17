#include <memory>
#include <vector>
#include "Actors.h"
#include "GameLogic.h"

int main() {
    auto player1 = std::make_shared<Player>("Link", std::make_unique<Sword>(3, 30, "Master Sword"));
    auto player2 = std::make_shared<Player>("Harry", std::make_unique<Sword>("Godric Gryffindor's Sword"));
    auto enemy1 = std::make_shared<Enemy>("Goblin", player2);

    std::vector<std::shared_ptr<Enemy>> enemies = {enemy1};
    
    AIUpdate(enemies);
    Battle(*player1, *enemy1, 8);

    auto player3 = std::make_shared<Player>("Fenlia", std::make_unique<Bow>());
    auto enemy2 = std::make_shared<Enemy>("Troll", player1);
    enemies.push_back(enemy2);
    
    AIUpdate(enemies);
    Battle(*player1, *player3, 8);

    AIUpdate(enemies);
    Battle(*player3, *enemy1, 5);
    
    AIUpdate(enemies);
    
    return 0;
}