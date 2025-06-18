#include "Character.h"
#include <iostream>
#include <fstream>

Character::Character(int initialHealth) : health(initialHealth) {}
int Character::GetHealth() const { return health; }

void Character::TakeDamage(int amount) {
    health = (health - amount > 0) ? health - amount : 0;
}

void Character::Heal(int amount) {
    health += amount;
}

void Character::SaveState() const {
    std::ofstream file("character.dat");
    if (file.is_open()) {
        file << health;
    }
}

void Character::AddItem(const std::string& item, int count) {
    inventory[item] += count;
}

std::map<std::string, int>& Character::GetInventory() {
    return inventory;
}

const std::map<std::string, int>& Character::GetInventory() const {
    return inventory;
}

void Character::ShowInventory() const {
    system("cls");
    std::cout << "=== ÈÍÂÅÍÒÀÐÜ ===\n";
    for (const auto& item : inventory) {
        std::cout << item.first << " x" << item.second << "\n";
    }
    std::cout << "\nÍàæìèòå ëþáóþ êëàâèøó...";
    _getch();
}