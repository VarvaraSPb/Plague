#pragma once
#include <map>
#include <string>
#include <iostream>

class Inventory {
private:
    std::map<std::string, int> items;

public:
    void AddItem(const std::string& item, int count = 1);
    void Show() const;
    void SaveToFile() const;
    void LoadFromFile();
};