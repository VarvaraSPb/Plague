#include "Location.h"
#include <Windows.h>

int main() {
    SetConsoleOutputCP(1251); // ��� ������� ����

    Character player(100);
    player.AddItem("�������");
    player.AddItem("���");

    Location forest(player, "�� � �����������...");
    forest.Enter();

    return 0;
}