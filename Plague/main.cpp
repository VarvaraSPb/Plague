#include "Location.h"
#include <Windows.h>

int main() {
    SetConsoleOutputCP(1251); // Для русских букв

    Character player(100);
    player.AddItem("Аптечка");
    player.AddItem("Меч");

    Location forest(player, "Вы в канализации...");
    forest.Enter();

    return 0;
}