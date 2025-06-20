#pragma once
#include "Character.h"

class BattleSystem {
private:
    Character& player;
    int enemyHealth;
    int damagePerHit;

    char GetRandomWASD() const;
    bool CheckWASDInput(char expectedKey, int timeoutMs, bool& shouldReturnToMenu);

public:
    BattleSystem(Character& player, int enemyHp, int dmg);
    void StartBattle();
};