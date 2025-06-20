#include "GameMenu.h"
#include <iostream>
#include <fstream>

GameMenu::GameMenu() : player(100), gameLoop(nullptr), inGame(false) {}

GameMenu::~GameMenu() {
    delete gameLoop;
}

void GameMenu::NewGame() {
    player = Character(100);
    player.AddItem("Аптечка", 3);
    inGame = true;
}

void GameMenu::LoadGame() {
    std::ifstream save("save.dat", std::ios::binary);
    if (!save) {
        std::cout << "Сохранение не найдено!";
        _getch();
        return;
    }

    int health;
    save >> health;  // Сначала читаем здоровье
    save.ignore();   // Пропускаем оставшийся символ новой строки

    player = Character(health);
    player.LoadInventory();
    inGame = true;
}

void GameMenu::SaveGame() const {
    std::ofstream save("save.dat", std::ios::binary);
    if (save) {
        save << player.GetHealth() << "\n";
        player.SaveInventory();
    }
}

void GameMenu::ShowMainMenu() {
    while (true) {
        system("cls");
        std::cout << "=== ГЛАВНОЕ МЕНЮ ===\n";
        std::cout << "1. Новая игра\n";
        std::cout << "2. Загрузить игру\n";
        std::cout << "3. Выход\n";
        std::cout << "Выберите: ";

        int choice = _getch();

        switch (choice) {
        case '1':
            NewGame();
            if (inGame) {
                gameLoop = new GameLoop(player);
                bool shouldReturn = false;
                gameLoop->Run(shouldReturn);
                if (shouldReturn) ReturnToMenu();
                delete gameLoop;
                gameLoop = nullptr;
            }
            break;
        case '2':
            LoadGame();
            if (inGame) {
                gameLoop = new GameLoop(player);
                bool shouldReturn = false;
                gameLoop->Run(shouldReturn);
                if (shouldReturn) ReturnToMenu();
                delete gameLoop;
                gameLoop = nullptr;
            }
            break;
        case '3':
            exit(0);
        default:
            std::cout << "Неверный ввод!";
            _getch();
        }
    }
}

void GameMenu::ReturnToMenu() {
    SaveGame();
    inGame = false;
}