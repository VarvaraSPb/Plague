#include "GameLoop.h"

GameLoop::GameLoop() :
    player(100),
    currentLocation(player, "Вы в канализации...") {}

void GameLoop::Run() {
    while (true) {
        currentLocation.Enter();
        // Здесь переход между локациями
    }
}