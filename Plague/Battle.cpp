#include "Battle.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace std::chrono;

BattleSystem::BattleSystem(Character& player, int enemyHp, int dmg)
    : player(player), enemyHealth(enemyHp), damagePerHit(dmg) {
    srand(static_cast<unsigned>(time(nullptr)));
}

char BattleSystem::GetRandomWASD() const {
    const char keys[] = { 'w', 'a', 's', 'd' };
    return keys[rand() % 4];
}

bool BattleSystem::CheckWASDInput(char expectedKey, int timeoutMs) const {
    auto start = steady_clock::now();
    while (duration_cast<milliseconds>(steady_clock::now() - start).count() < timeoutMs) {
        if (_kbhit()) {
            char input = tolower(_getch());
            if (input == 'i') {
                player.ShowInventory();
                cout << "Бой продолжается! Ждем - " << (char)toupper(expectedKey) << "\n";
                continue;
            }
            if (input == expectedKey) {
                return true;
            }
        }
        this_thread::sleep_for(milliseconds(10));
    }
    return false;
}

void BattleSystem::StartBattle() {
    system("cls");
    cout << "=== БОЙ НАЧАЛСЯ ===\n";
    cout << "Герой (" << player.GetHealth() << " HP) vs Враг (" << enemyHealth << " HP)\n\n";

    while (player.GetHealth() > 0 && enemyHealth > 0) {
        char requiredKey = GetRandomWASD();
        int delay = 1000 + rand() % 2000; // Случайная задержка 1-3 сек

        cout << "Приготовьтесь...\n";
        this_thread::sleep_for(milliseconds(delay));

        cout << "Быстро нажмите " << (char)toupper(requiredKey) << "!\n";

        bool success = CheckWASDInput(requiredKey, 2000); // 2 сек на реакцию

        if (success) {
            enemyHealth -= damagePerHit;
            cout << "Попадание! Враг -" << damagePerHit << " HP\n";
        }
        else {
            player.TakeDamage(damagePerHit);
            cout << "Промах! Вы -" << damagePerHit << " HP\n";
        }

        cout << "\nВаше HP: " << player.GetHealth() << " | Враг HP: " << enemyHealth << "\n\n";
        this_thread::sleep_for(milliseconds(1000));
    }

    if (player.GetHealth() > 0) {
        cout << "ПОБЕДА! \n";
    }
    else {
        cout << "Поражение!\n";
    }
    player.SaveState();
    _getch();
}