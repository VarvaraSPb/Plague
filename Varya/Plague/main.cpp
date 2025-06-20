#include "GameMenu.h"
#include <Windows.h>

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    GameMenu game;
    game.ShowMainMenu();

    return 0;
}