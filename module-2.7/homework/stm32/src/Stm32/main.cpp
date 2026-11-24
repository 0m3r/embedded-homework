#include "Application/Application.hpp"
#include "Platform/Board.hpp"

int main()
{
    Board::init();
    appInit();

    while (true) {
        appUpdate();
    }
}
