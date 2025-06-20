#pragma once
#include <map>
#include <string>
#include <fstream>
#include <conio.h>

class Character {
private:
    int health;
    std::map<std::string, int> inventory;

public:
    explicit Character(int initialHealth = 100);

    int GetHealth() const;
    void TakeDamage(int amount);
    void Heal(int amount);
    void SaveState() const;

    void AddItem(const std::string& item, int count = 1);
    bool RemoveItem(const std::string& item, int count = 1);
    void ShowInventory(bool& shouldReturnToMenu) const;
    void SaveInventory() const;
    void LoadInventory();
};