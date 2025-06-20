#include "Character.h"
#include <iostream>
#include <algorithm>

Character::Character(int initialHealth) : health(initialHealth) {}

int Character::GetHealth() const { return health; }

void Character::TakeDamage(int amount) {
    health = std::max(0, health - amount);
}

void Character::Heal(int amount) {
    health += amount;
}

void Character::SaveState() const {
    std::ofstream file("character.dat", std::ios::binary);
    if (file) file << health;
}

void Character::AddItem(const std::string& item, int count) {
    inventory[item] += count;
}

bool Character::RemoveItem(const std::string& item, int count) {
    if (inventory[item] >= count) {
        inventory[item] -= count;
        if (inventory[item] == 0) inventory.erase(item);
        return true;
    }
    return false;
}

void Character::ShowInventory(bool& shouldReturnToMenu) const {
    system("cls");
    std::cout << "=== ИНВЕНТАРЬ ===\n";
    std::cout << "ESC - Выход в меню\n\n";

    if (inventory.empty()) {
        std::cout << "(пусто)\n";
    }
    else {
        for (const auto& item : inventory) {
            std::cout << "- " << item.first << " x" << item.second << "\n";
        }
    }

    std::cout << "\nНажмите любую клавишу...";
    int input = _getch();
    if (input == 27) shouldReturnToMenu = true;
}

void Character::SaveInventory() const {
    std::ofstream file("inventory.dat", std::ios::binary);
    if (file) {
        for (const auto& item : inventory) {
            file << item.first << " " << item.second << "\n";
        }
    }
}

void Character::LoadInventory() {
    inventory.clear();
    std::ifstream file("inventory.dat", std::ios::binary);
    std::string item;
    int count;
    while (file >> item >> count) {
        inventory[item] = count;
    }
}