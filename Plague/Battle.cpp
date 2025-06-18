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
                cout << "��� ������������! ���� - " << (char)toupper(expectedKey) << "\n";
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
    cout << "=== ��� ������� ===\n";
    cout << "����� (" << player.GetHealth() << " HP) vs ���� (" << enemyHealth << " HP)\n\n";

    while (player.GetHealth() > 0 && enemyHealth > 0) {
        char requiredKey = GetRandomWASD();
        int delay = 1000 + rand() % 2000; // ��������� �������� 1-3 ���

        cout << "�������������...\n";
        this_thread::sleep_for(milliseconds(delay));

        cout << "������ ������� " << (char)toupper(requiredKey) << "!\n";

        bool success = CheckWASDInput(requiredKey, 2000); // 2 ��� �� �������

        if (success) {
            enemyHealth -= damagePerHit;
            cout << "���������! ���� -" << damagePerHit << " HP\n";
        }
        else {
            player.TakeDamage(damagePerHit);
            cout << "������! �� -" << damagePerHit << " HP\n";
        }

        cout << "\n���� HP: " << player.GetHealth() << " | ���� HP: " << enemyHealth << "\n\n";
        this_thread::sleep_for(milliseconds(1000));
    }

    if (player.GetHealth() > 0) {
        cout << "������! \n";
    }
    else {
        cout << "���������!\n";
    }
    player.SaveState();
    _getch();
}