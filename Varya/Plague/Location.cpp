#include "Location.h"
#include "Battle.h"  
#include <conio.h>
#include <Windows.h>
#include <iostream>

Location::Location(Character& player, const std::string& desc)
    : player(player), description(desc) {}

void Location::Enter(bool& shouldReturnToMenu) {
    bool inLocation = true;

    while (inLocation && !shouldReturnToMenu) {
        system("cls");
        std::cout << description << "\n\n";
        std::cout << "1 - Проверка боя\n";
        std::cout << "2 - Открыть инвентарь\n";
        std::cout << "ESC - Выход в меню\n";

        int input = _getch();

        switch (input) {
        case '1': {
            BattleSystem battle(player, 100, 10);
            battle.StartBattle();
            if (player.GetHealth() <= 0) {
                std::cout << "Вы умерли! Возврат в меню...\n";
                _getch();
                shouldReturnToMenu = true;
            }
            break;
        }
        case '2': {
            bool inventoryShouldReturn = false;
            player.ShowInventory(inventoryShouldReturn);
            shouldReturnToMenu = inventoryShouldReturn;
            break;
        }
        case 27:  
            shouldReturnToMenu = true;
            break;
        }
    }
}