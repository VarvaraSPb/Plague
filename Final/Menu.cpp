#include "Menu.h"
#include <iostream>

Menu::Menu() {
    options = {
        "1. New Game",
        "2. Load Game",
        "3. Exit"
    };
}

Menu::MenuResult Menu::Show() {
    Display();
    return GetChoice();
}

void Menu::Display() {
    std::cout << "=== PLAGUE ===\n";
    for (const auto& option : options) {
        std::cout << option << "\n";
    }
    std::cout << "- - - - - - - - -\n";
}

Menu::MenuResult Menu::GetChoice() {
    int choice;
    std::cin >> choice;

    switch (choice) {
    case 1: return MenuResult::Play;
    case 2: return MenuResult::Load;
    case 3: return MenuResult::Exit;
    default: return MenuResult::None;
    }
}