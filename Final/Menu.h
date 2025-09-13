#pragma once
#include <string>
#include <vector>

class Menu {
public:
    enum class MenuResult {
        Play,
        Load,
        Exit,
        None
    };

    Menu();
    ~Menu() = default;

    MenuResult Show();

private:
    void Display();
    MenuResult GetChoice();

    std::vector<std::string> options;
};