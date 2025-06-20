#pragma once
#include "Character.h"
#include "GameLoop.h"

class GameMenu {
private:
    Character player;
    GameLoop* gameLoop;
    bool inGame;

    void NewGame();
    void LoadGame();
    void SaveGame() const;

public:
    GameMenu();
    ~GameMenu();
    void ShowMainMenu();
    void ReturnToMenu();
    bool InGame() const { return inGame; }
};
