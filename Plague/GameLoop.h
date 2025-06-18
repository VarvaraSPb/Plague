#pragma once
#include "Location.h"
#include "Character.h"

class GameLoop {
private:
    Character player;
    Location currentLocation;

public:
    GameLoop();
    void Run();
};