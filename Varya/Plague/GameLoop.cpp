#include "GameLoop.h"
#include <conio.h>

GameLoop::GameLoop(Character& player)
    : player(player), currentLocation(player, "Вы в канализации...") {}

void GameLoop::Run(bool& shouldReturnToMenu) {
    const std::string locations[] = {
        "Вы в канализации...",
        "Вы в своём доме!",
        "Вы в городе, будьие на чеку."
    };
    int locIndex = 0;

    while (!shouldReturnToMenu) {
        currentLocation.UpdateDescription(locations[locIndex]);
        currentLocation.Enter(shouldReturnToMenu);

        if (!shouldReturnToMenu) {
            locIndex = (locIndex + 1) % 3;
            player.SaveState();
            player.SaveInventory();
        }
    }
}