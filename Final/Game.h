#pragma once
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Game {
public:
    Game();
    void Start();
    void HandleInput(const std::string& input);

private:
    void LoadGameData();
    void PrintLocation() const;
    void AddToInventory(const std::string& item);
    void PrintInventory() const;
    void ShowCurrentChoices();
    void HandleChoice(int choice);
    void ChangeLocation(const std::string& newLocation);
    void ProcessChoiceAction(const json& choice);
    bool CheckChoiceRequirements(const json& choice);

    int GetRandomNumber(int min, int max);

    // Структура для улучшенного инвентаря
    struct InventoryItem {
        std::string name;
        std::string type; // "weapon", "healing", "key"
        int value; // damage, heal amount, etc.
    };

    std::vector<InventoryItem> inventory;

    // Боевая система
    void StartCombat(const std::string& enemyName);
    void HandleCombatInput(const std::string& input);
    void PlayerAttack();
    void EnemyAttack();
    void EndCombat(bool playerWon);
    void TryEscape();
    void YakovFightWithWrench(); // Специальная функция для боя с ключом
    bool QuickTimeEvent(const std::string& prompt, int timeLimit);

    // Статистики персонажа
    int playerHealth = 100;
    int playerAttack = 15;
    int playerDefense = 5;

    // Статистики врага
    int enemyHealth = 0;
    int enemyAttack = 0;
    int enemyDefense = 0;
    std::string currentEnemy;

    bool inCombat = false;

    // Данные игры
    json gameData;
    std::string currentLocation;
    std::vector<json> currentChoices;
    bool inChoiceMode = false;

    std::map<std::string, bool> flags;
};