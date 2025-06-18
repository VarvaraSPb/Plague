#include "Inventory.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>

void Inventory::AddItem(const std::string& item, int count) {
    items[item] += count;
}

void Inventory::Show() const {
    system("cls");
    std::cout << "=== ��������� ===\n";

    if (items.empty()) {
        std::cout << "(�����)\n";
    }
    else {
        for (const auto& entry : items) {
            std::cout << entry.first << " x" << entry.second << "\n";
        }
    }
    std::cout << "\n������� ����� �������...";
    _getch();
}

void Inventory::SaveToFile() const {
    std::ofstream file("inventory.dat");
    if (file.is_open()) {
        for (const auto& entry : items) {
            file << entry.first << " " << entry.second << "\n";
        }
    }
}

void Inventory::LoadFromFile() {
    std::ifstream file("inventory.dat");
    if (file.is_open()) {
        std::string item;
        int count;
        while (file >> item >> count) {
            items[item] = count;
        }
    }
}