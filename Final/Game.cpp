#include "Game.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <random>
#include <string>
#include <future>
#include <chrono>
#include <thread>
#include <atomic>
#include <nlohmann/json.hpp>

// Глобальные переменные для системы таймера
std::atomic<bool> timer_expired(false);
std::atomic<bool> input_received(false);

static bool startsWith(const std::string& str, const std::string& prefix) {
    return str.rfind(prefix, 0) == 0;
}

Game::Game() {
    LoadGameData();
    if (gameData.contains("start_location")) {
        currentLocation = gameData["start_location"];
    }
    else {
        currentLocation = "sewer_entrance";
    }
}

void Game::LoadGameData() {
    std::ifstream file("Story.json");
    if (file.is_open()) {
        try {
            gameData = json::parse(file);
            //std::cout << "Story.json successfully loaded!\n";
        }
        catch (const std::exception& e) {
            std::cerr << "JSON parsing error: " << e.what() << "\n";
            gameData = json::object();
        }
    }
    else {
        std::cerr << "Error: Story.json file not found!\n";
        gameData = json::object();
    }
}

void Game::Start() {
    // Показываем первую локацию
    if (gameData["locations"].contains(currentLocation)) {
        auto& location = gameData["locations"][currentLocation];
        std::cout << "\n" << location["description"] << "\n";
    }

    // Показываем choices для первой локации
    if (gameData["locations"].contains(currentLocation) &&
        gameData["locations"][currentLocation].contains("choices") &&
        !gameData["locations"][currentLocation]["choices"].empty()) {

        ShowCurrentChoices();
    }

    std::string input;
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, input);

        if (input == "exit") break;

        HandleInput(input);
    }
}

void Game::HandleInput(const std::string& input) {
    // Всегда обрабатываем инвентарь
    if (input == "i" || input == "inventory") {
        PrintInventory();
        return;
    }

    // 1. Первым делом проверяем, находимся ли мы в режиме боя
    if (inCombat) {
        HandleCombatInput(input);
        return;
    }

    // 2. Затем проверяем, находимся ли мы в режиме выбора
    if (inChoiceMode) {
        try {
            int choice = std::stoi(input);
            HandleChoice(choice);
            return;
        }
        catch (...) {
            std::cout << "Please enter a number from 1 to " << currentChoices.size() << "\n";
            ShowCurrentChoices(); // Показываем choices снова
        }
        return;
    }

    if (input == "inventory") {
        PrintInventory();
    }
    else if (startsWith(input, "take ")) {
        std::string item = input.substr(5);

        if (gameData["locations"].contains(currentLocation) &&
            gameData["locations"][currentLocation].contains("items")) {

            auto& locItems = gameData["locations"][currentLocation]["items"];
            auto it = std::find(locItems.begin(), locItems.end(), item);

            if (it != locItems.end()) {
                AddToInventory(item);
                locItems.erase(it);
                std::cout << "You picked up " << item << "!\n";

                // Отслеживаем важные предметы
                if (item == "wrench") flags["has_wrench"] = true;
                if (item == "bandages") flags["has_bandages"] = true;
                if (item == "small knife") flags["has_knife"] = true;
            }
            else {
                std::cout << "There is no '" << item << "' here!\n";
            }
        }
        else {
            std::cout << "You can't take anything here.\n";
        }
    }
    else if (input == "look") {
        // Команда осмотреться
        PrintLocation();
    }
    else if (input == "help") {
        // Команда помощи
        std::cout << "Available commands:\n";
        std::cout << "  take [item] - pick up an item\n";
        std::cout << "  inventory - show your inventory\n";
        std::cout << "  look - look around again\n";
        std::cout << "  exit - return to main menu\n";
    }
    else {
        std::cout << "Unknown command. Try: take [item], inventory, look, help, exit\n";
    }
}

void Game::ShowCurrentChoices() {
    if (!gameData.contains("locations") ||
        !gameData["locations"].contains(currentLocation) ||
        !gameData["locations"][currentLocation].contains("choices")) {
        std::cout << "No choices available here.\n";
        return;
    }

    currentChoices.clear();
    auto availableChoices = gameData["locations"][currentLocation]["choices"];

    //std::cout << "Available choices: " << availableChoices.size() << "\n";

    // Фильтруем choices по флагам
    for (const auto& choice : availableChoices) {
        if (CheckChoiceRequirements(choice)) {
            currentChoices.push_back(choice);
        }
    }

    if (currentChoices.empty()) {
        std::cout << "No available choices at this time.\n";
        return;
    }

    inChoiceMode = true;

    std::cout << "\nMake your choice:\n";
    for (size_t i = 0; i < currentChoices.size(); ++i) {
        std::cout << i + 1 << ". " << currentChoices[i]["text"] << "\n";
    }
    std::cout << "> ";
}

bool Game::CheckChoiceRequirements(const json& choice) {
    if (choice.contains("requires_flag")) {
        std::string flagName = choice["requires_flag"];

        // Заменяем starts_with на find
        if (flagName.find("!") == 0) { // Если начинается с !
            std::string actualFlag = flagName.substr(1);
            return flags.find(actualFlag) == flags.end() || !flags[actualFlag];
        }

        return flags.find(flagName) != flags.end() && flags[flagName];
    }
    return true;
}

void Game::HandleChoice(int choiceIndex) {
    if (choiceIndex < 1 || choiceIndex > currentChoices.size()) {
        std::cout << "Invalid choice!\n";
        ShowCurrentChoices(); // Показываем choices снова
        return;
    }

    json selectedChoice = currentChoices[choiceIndex - 1];
    ProcessChoiceAction(selectedChoice);

    // Сохраняем следующую локацию перед сбросом режима
    std::string nextLocation = currentLocation;
    if (selectedChoice.contains("next_location")) {
        nextLocation = selectedChoice["next_location"];
    }

    inChoiceMode = false;
    currentChoices.clear();

    // Меняем локацию и показываем следующий выбор
    if (selectedChoice.contains("next_location")) {
        ChangeLocation(nextLocation);
    }

    // Всегда показываем choices после выбора, если они есть
    if (gameData["locations"].contains(currentLocation) &&
        gameData["locations"][currentLocation].contains("choices") &&
        !gameData["locations"][currentLocation]["choices"].empty()) {

        ShowCurrentChoices();
    }
}

void Game::ProcessChoiceAction(const json& choice) {
    if (choice.contains("action")) {
        std::cout << "\n" << choice["action"] << "\n";
    }

    if (choice.contains("set_flag")) {
        std::string flagName = choice["set_flag"];
        flags[flagName] = true;
        std::cout << "[Flag set: " << flagName << "]\n";
    }

    // Специальные флаги для отслеживания состояния
    if (choice.contains("text")) {
        std::string choiceText = choice["text"];
        if (choiceText.find("bandage") != std::string::npos ||
            choiceText.find("heal") != std::string::npos) {
            flags["anton_healed"] = true;
        }
    }
}

void Game::ChangeLocation(const std::string& newLocation) {
    currentLocation = newLocation;

    // Сначала показываем описание локации
    if (gameData["locations"].contains(currentLocation)) {
        auto& location = gameData["locations"][currentLocation];
        std::cout << "\n" << location["description"] << "\n";

        if (location.contains("items") && !location["items"].empty()) {
            std::cout << "\nItems here: ";
            for (const auto& item : location["items"]) {
                std::cout << item << " ";
            }
            std::cout << "\n";
        }
    }
}

void Game::PrintLocation() const {
    if (!gameData.contains("locations") || !gameData["locations"].contains(currentLocation)) {
        std::cout << "\nLocation '" << currentLocation << "' not found!\n";
        return;
    }

    auto& location = gameData["locations"][currentLocation];

    if (location.contains("items") && !location["items"].empty()) {
        std::cout << "\nItems here: ";
        for (const auto& item : location["items"]) {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }
}

void Game::AddToInventory(const std::string& item) {
    // Создаем InventoryItem из строки
    InventoryItem newItem;
    newItem.name = item;

    // Определяем тип предмета
    if (item == "wrench" || item == "small knife") {
        newItem.type = "weapon";
        newItem.value = (item == "wrench") ? 8 : 12;
    }
    else if (item == "bandages") {
        newItem.type = "healing";
        newItem.value = 20;
    }
    else {
        newItem.type = "misc";
        newItem.value = 0;
    }

    inventory.push_back(newItem);
}

void Game::PrintInventory() const {
    std::cout << "\n=== INVENTORY ===\n";

    if (inventory.empty()) {
        std::cout << "Empty.\n";
    }
    else {
        for (const auto& item : inventory) {
            std::cout << "  - " << item.name;
            if (item.type == "weapon") std::cout << "(Weapon)";
            if (item.type == "healing") std::cout << "(Heals: " << item.value << " HP)";
            if (item.type == "key") std::cout << "(Key)";
            std::cout << "\n";
        }
    }

    // Показываем активные флаги (безопасный доступ)
    std::cout << "\nEquipment:\n";
    if (flags.count("has_wrench") > 0 && flags.at("has_wrench"))
        std::cout << "  - Wrench\n";
    if (flags.count("has_knife") > 0 && flags.at("has_knife"))
        std::cout << "  - Knife\n";
    if (flags.count("has_bandages") > 0 && flags.at("has_bandages"))
        std::cout << "  - Bandages\n";

    std::cout << "Health: " << playerHealth << "/100\n";

    // Убираем ShowCurrentChoices() из const метода
    // Вместо этого просто inform пользователя
    if (inCombat) {
        std::cout << "\n[Combat in progress - type '1', '2', or '3']\n";
    }
    else if (inChoiceMode) {
        std::cout << "\n[Choices available - enter corresponding number]\n";
    }
}

int Game::GetRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void Game::HandleCombatInput(const std::string& input) {
    // Если это специальный бой с Яковом, не обрабатываем обычные команды
    if (currentEnemy == "yakov" && flags["has_wrench"]) {
        std::cout << "Press ENTER quickly for quick-time events!\n";
        return;
    }

    // Обычная логика боя для других случаев
    if (input == "1") {
        PlayerAttack();
    }
    else if (input == "2") {
        TryEscape();
    }
    else {
        std::cout << "1. Fight\n2. Try to escape\n> ";
    }
}

void Game::StartCombat(const std::string& enemyName) {
    std::cout << "DEBUG: StartCombat called with: " << enemyName << std::endl;
    std::cout << "DEBUG: has_wrench = " << (flags["has_wrench"] ? "true" : "false") << std::endl;

    inCombat = true;
    currentEnemy = enemyName;

    // Специальная обработка для Якова с ключом
    if (enemyName == "yakov" && flags["has_wrench"]) {
        std::cout << "DEBUG: Starting wrench fight sequence!" << std::endl;
        YakovFightWithWrench();
        return;
    }

    // Обычный бой для других случаев
    std::cout << "\n" << enemyName << " attacks!\n";

    if (inChoiceMode) {
        std::cout << "1. Fight\n2. Try to escape\n> ";
    }
}

void Game::PlayerAttack() {
    int damage = playerAttack - enemyDefense;
    if (damage < 1) damage = 1;

    enemyHealth -= damage;
    std::cout << "You attack " << currentEnemy << " for " << damage << " damage!\n";

    if (enemyHealth <= 0) {
        std::cout << "You defeated " << currentEnemy << "!\n";
        EndCombat(true);
    }
    else {
        std::cout << currentEnemy << " has " << enemyHealth << " HP remaining.\n";
        EnemyAttack(); // Враг атакует после вас
    }
}

void Game::EnemyAttack() {
    int damage = enemyAttack - playerDefense;
    if (damage < 1) damage = 1;

    playerHealth -= damage;
    std::cout << currentEnemy << " attacks you for " << damage << " damage!\n";
    std::cout << "You have " << playerHealth << " HP remaining.\n";

    if (playerHealth <= 0) {
        std::cout << "You were defeated by " << currentEnemy << "...\n";
        EndCombat(false);
    }
    else if (inCombat) {
        std::cout << "Your turn. Commands: attack, use [item], inventory, run\n> ";
    }
}

void Game::YakovFightWithWrench() {
    std::cout << "\nYakov pulls out a knife! 'Sorry, kids. Only the strong survive.'\n";
    std::cout << "You remember you have a wrench...\n";

    if (!QuickTimeEvent("Type 'hit' to dodge Yakov's attack!", 3)) {
        std::cout << "\nToo slow! Yakov stabs you...\n";
        std::cout << "GAME OVER\n";
        inCombat = false;
        return;
    }

    std::cout << "\nYou dodged! Now attack with wrench!\n";

    if (!QuickTimeEvent("Type 'hit' to strike with wrench!", 4)) {
        std::cout << "\nYou fumbled with the wrench... Yakov strikes!\n";
        std::cout << "GAME OVER\n";
        inCombat = false;
        return;
    }

    std::cout << "\nYou defeated Yakov with the wrench!\n";
    EndCombat(true);
    ChangeLocation("after_yakov_victory");
}

void Game::TryEscape() {
    std::cout << "You attempt to run away...\n";
    int escapeChance = 40;

    if (flags.find("has_knife") != flags.end() && flags["has_knife"]) {
        escapeChance = 70;
    }

    if (GetRandomNumber(1, 100) <= escapeChance) {
        std::cout << "Successfully escaped!\n";
        EndCombat(true);
    }
    else {
        std::cout << "Escape failed! ";
        int damage = 20;
        playerHealth -= damage;
        std::cout << "You take " << damage << " damage! ";
        std::cout << "HP: " << playerHealth << "/100\n";

        if (playerHealth <= 0) {
            std::cout << "YOU DIED!\n";
            EndCombat(false);
        }
    }
}

void Game::EndCombat(bool playerWon) {
    inCombat = false;
    if (playerWon) {
        std::cout << "Combat victory!\n";
    }
    else {
        std::cout << "Combat failed...\n";
    }
}

bool Game::QuickTimeEvent(const std::string& prompt, int timeLimit) {
    std::cout << "\n" << prompt << "\n";
    std::cout << "You have " << timeLimit << " seconds! Type 'hit' quickly!\n";
    std::cout << "> ";
    std::cout.flush();

    // Простой таймер без потоков
    auto start = std::chrono::steady_clock::now();

    std::string input;
    std::getline(std::cin, input);

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    bool success = (duration <= timeLimit) && (input == "hit");

    if (!success) {
        std::cout << "Too slow! Took " << duration << " seconds.\n";
    }
    else {
        std::cout << "Good reaction! Took " << duration << " seconds.\n";
    }

    return success;
}