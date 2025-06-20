#pragma once
#include "Character.h"

class Location {
private:
    Character& player;
    std::string description;

public:
    void UpdateDescription(const std::string& newDesc) { description = newDesc; }
    Location(Character& player, const std::string& desc);
    void Enter(bool& shouldReturnToMenu);
};