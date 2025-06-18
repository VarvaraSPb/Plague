#include "Location.h"
#include "Battle.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>

Location::Location(Character& player, const std::string& desc)
    : player(player), description(desc) {}

void Location::Enter() {
    bool inLocation = true;

    while (inLocation) {
        system("cls");
        std::cout << description << "\n\n";
        std::cout << "1 - Вступить в бой (WASD-механика)\n";
        std::cout << "2 - Уйти\n";
        std::cout << "I - Инвентарь\n";

        if (_kbhit()) {
            char input = tolower(_getch());

            if (input == 'i') {
                player.ShowInventory();
                continue;
            }

            if (input == '1') {
                BattleSystem battle(player, 50, 15);
                battle.StartBattle();

                // После боя проверяем здоровье
                if (player.GetHealth() <= 0) {
                    std::cout << "Вы слишком слабы чтобы продолжать...\n";
                    _getch();
                    inLocation = false;
                }
                continue; // Возврат в меню локации
            }

            if (input == '2') {
                std::cout << "Вы покидаете это место.\n";
                _getch();
                inLocation = false;
            }
        }
    }
}