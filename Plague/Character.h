#pragma once
#include <map>
#include <string>
#include <conio.h>

class Character {
private:
    int health;
    std::map<std::string, int> inventory;

public:
    Character(int initialHealth);
    int GetHealth() const;
    void TakeDamage(int amount);
    void Heal(int amount);
    void SaveState() const;

    void AddItem(const std::string& item, int count = 1);
    void ShowInventory() const;
    std::map<std::string, int>& GetInventory(); 
    const std::map<std::string, int>& GetInventory() const;  
};