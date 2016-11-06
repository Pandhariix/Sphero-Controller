#include <stdlib.h>
#include "controller.hpp"


int main(int argc, char **argv)
{
    std::cout << "Press Enter to exit the program" << std::endl;

    Controller controller;

    controller.displayJoysticksInformations();
    controller.launchJoystickListener(0);

    while (true) {

        if (std::cin.get() == '\n') {
            std::cout << "Enter pressed, stopping the program...";
            break;
        }
    }

    controller.stopJoystickListener();
    controller.clear();

    return EXIT_SUCCESS;
}

