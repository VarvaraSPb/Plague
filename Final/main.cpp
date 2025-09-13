#include "Menu.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>


int main() {

    using json = nlohmann::json;

    Menu mainMenu;
    Game game;

    while (true) {
        Menu::MenuResult result = mainMenu.Show();

        switch (result) {
        case Menu::MenuResult::Play:
            game.Start();
            break;
        case Menu::MenuResult::Load:
            std::cout << "Loading not implemented yet.\n";
            break;
        case Menu::MenuResult::Exit:
            std::cout << "Goodbye!\n";
            return 0;
        case Menu::MenuResult::None:
            std::cout << "Invalid choice. Try again.\n";
            break;
        }
    }
}