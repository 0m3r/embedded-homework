#include "Stm32/Application/Application.hpp"
#include "Stm32/Platform/Board.hpp"

int main()
{
    Board::init();
    appInit();

    while (true) {
        appUpdate();
    }
}
