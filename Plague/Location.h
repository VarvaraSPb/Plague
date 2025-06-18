#pragma once
#include "Character.h"

class Location {
private:
    Character& player;
    std::string description;

public:
    Location(Character& player, const std::string& desc);
    void Enter();
};