#pragma once
#include "Character.h"
#include "Location.h"

class GameLoop {
private:
    Character& player;
    Location currentLocation;

public:
    GameLoop(Character& player);
    void Run(bool& shouldReturnToMenu);
};