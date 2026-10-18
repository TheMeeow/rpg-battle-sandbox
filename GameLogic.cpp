#include "GameLogic.h"
#include <iostream>
#include <random>

bool FirstHasAdvantage(MyActor& actor1, MyActor& actor2) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> coin(0, 1);
    
    return (actor1.GetAttackRange() > actor2.GetAttackRange()) 
        || (actor1.GetAttackRange() == actor2.GetAttackRange() && coin(gen) == 0);
}

std::string_view ActionVerbToString(AttackData::ActionVerb verb) {
    switch (verb)
        {
        case AttackData::ActionVerb::Punches:
            return "punches";

        case AttackData::ActionVerb::Slashes:
            return "slashes";

        case AttackData::ActionVerb::Shoots:
            return "shoots an arrow at";

        case AttackData::ActionVerb::Bites:
            return "bites";
                    
        default:
            return "attacks";
        }
}

void Battle(MyActor& actor1, MyActor& actor2, unsigned int rounds) {
    if(actor1.IsDead() || actor2.IsDead()) {
        std::cout << "\nBattle between " << actor1.GetName() << " and " 
                  << actor2.GetName() << " is canceled! One of the fighters is dead.\n";
        return;
    } else if (&actor1 == &actor2) {
        std::cout << "\nBattle is canceled! " << actor1.GetName() << " can't fight with themselves.\n";
        return;
    }
    
    std::cout << "\n------------- Battle between " << actor1.GetName() << " and " 
              << actor2.GetName() << " has started! -------------\n\n";

    bool a1Attacks = FirstHasAdvantage(actor1, actor2);

    for (unsigned int i = 0; i < rounds; i++) {
        MyActor& attacker = a1Attacks ? actor1 : actor2;
        MyActor& defender = a1Attacks ? actor2 : actor1;

        AttackData attackResult = attacker.Attack(defender);

        if (attackResult.success) {
            std::cout << attacker.GetName() << " " << ActionVerbToString(attackResult.actionVerb) 
                      << " " << defender.GetName() << " with " << attackResult.weaponName 
                      << " and deals " << attackResult.damage << " damage!\n";
            std::cout << defender.GetName() << "'s health decreased to " << defender.GetHealth() << ".\n";        
            
            if (defender.IsDead()) {
                std::cout << "*** " << defender.GetName() << " falls dead! ***\n";
                break;
            }
        } else {
            std::string actionVerb;
                    switch (attackResult.actionVerb)
                    {
                    case AttackData::ActionVerb::Shoots:
                        actionVerb = "tries to shoot, but has no arrows";
                        break;

                    default:
                        actionVerb = "tries to attack, but misses";
                        break;
                    }
            std::cout << attacker.GetName() << " " << actionVerb << ".\n";
        }

        a1Attacks = !a1Attacks;
        std::cout << "-----------------------------------------------------------------\n";
    }

    std::cout << "... Battle ended ...\n\n";
}

void AIUpdate(const std::vector<std::shared_ptr<Enemy>>& enemies) {
    for (const auto& enemy : enemies) {
        AIUpdateReport report = enemy->Update();

        switch (report.state) {
            case AIUpdateReport::State::Dead:
                std::cout << "[AI] " << report.actorName << "'s dead body is lying on the ground.\n";
                break;
                
            case AIUpdateReport::State::Idling:
                std::cout << "[AI] " << report.actorName << " is idling...\n";
                break;

            case AIUpdateReport::State::TargetLost:
                std::cout << "[AI] " << report.actorName << " realizes " 
                          << report.targetName << " is dead. The pursuit has been terminated.\n";
                break;

            case AIUpdateReport::State::Attacking:
                std::cout << "[AI] " << report.actorName << " pursues " << report.targetName << "!\n";
                if (report.attackInfo.success) {
                    std::cout << "  -> " << report.actorName << " "
                              << ActionVerbToString(report.attackInfo.actionVerb) << " "  
                              << report.targetName << " and deals " 
                              << report.attackInfo.damage << " damage!\n";
                }
                break;
        }
    }
}