#include "controller.hpp"


Input Controller::input;
bool  Controller::listenerActive = NULL;


/**
 * @brief Controller::Controller
 *
 * Constructor
 */
Controller::Controller() {

    this->listenerActive = false;

    try {
        this->initializeSDL();

    } catch(std::string const &error) {
        std::cerr << error << std::endl;
    }
}


/**
 * @brief Controller::~Controller
 *
 * Destructor
 */
Controller::~Controller() {
    this->clear();
}



/**
 * @brief Controller::launchJoystickListener
 * @param joystickNum - The joystick to be initialized
 *
 * Launches the joystick listener
 */
void Controller::launchJoystickListener(int joystickNum) {

    if (!Controller::listenerActive) {
        try {
            SDL_JoystickEventState(SDL_ENABLE);
            this->initializeInput(joystickNum);

            Controller::listenerActive = true;
            this->listenerThread       = SDL_CreateThread(Controller::joystickListenerCallback, "joystickListenerCallback", NULL);

        } catch(std::string const &error) {
            std::cerr << error << std::endl;
        }
    }
}


/**
 * @brief Controller::stopJoystickListener
 *
 * Stops the listener
 */
void Controller::stopJoystickListener() {

    if (Controller::listenerActive) {
        Controller::listenerActive = false;
        SDL_WaitThread(this->listenerThread, NULL);
        this->destroyInput();
    }
}



/**
 * @brief Controller::clear
 *
 * Stops the listener and the SDL
 */
void Controller::clear() {
    this->stopJoystickListener();
    this->destroySDL();
}



/**
 * @brief Controller::initializeSDL
 *
 * Launches the SDL
 */
void Controller::initializeSDL() {

    if(SDL_Init(SDL_INIT_JOYSTICK) < 0)
        throw std::string("The joystick module couldn't be launched");
}



/**
 * @brief Controller::destroySDL
 *
 * Destroys the SDL
 */
void Controller::destroySDL() {

    SDL_Quit();
}


/**
 * @brief Controller::displayJoysticksInformations
 *
 * Displays informations on the josystick
 */
void Controller::displayJoysticksInformations() {

    SDL_Joystick *joystick;

    std::cout << "Number of controllers detected : " << SDL_NumJoysticks() << std::endl << std::endl;

    for(int i = 0; i < SDL_NumJoysticks(); i++) {
        joystick = SDL_JoystickOpen(i);

        std::cout << "#------------#" << std::endl;
        std::cout << "|Controller "   << i << "|" << std::endl;
        std::cout << "#------------#" << std::endl;

        std::cout << "Number of buttons : " << SDL_JoystickNumButtons(joystick) << std::endl;
        std::cout << "Number of axis    : " << SDL_JoystickNumAxes(joystick)    << std::endl;
        std::cout << "Nombre of hats    : " << SDL_JoystickNumHats(joystick)    << std::endl;
        std::cout << "Nombre of balls   : " << SDL_JoystickNumBalls(joystick)   << std::endl;

        SDL_JoystickClose(joystick);
    }
}


/**
 * @brief Controller::initializeInput
 * @param joystickNum - The joystick to be initialized
 *
 * Initializes the input for the desired joystick
 */
void Controller::initializeInput(int joystickNum) {

    if(joystickNum < SDL_NumJoysticks()) {

        Controller::input.joystick = SDL_JoystickOpen(joystickNum);
        Controller::input.numero = joystickNum;


        Controller::input.buttons    = (char*) malloc(SDL_JoystickNumButtons(Controller::input.joystick) * sizeof(char));
        Controller::input.axes       = (int*) malloc(SDL_JoystickNumAxes(Controller::input.joystick) * sizeof(int));
        Controller::input.hats       = (int*) malloc(SDL_JoystickNumHats(Controller::input.joystick) * sizeof(int));
        Controller::input.trackballs = (InputTrackball*) malloc(SDL_JoystickNumBalls(Controller::input.joystick) * sizeof(InputTrackball));

        for(int i=0;i<SDL_JoystickNumButtons(Controller::input.joystick);i++)
            Controller::input.buttons[i] = 0;

        for(int i=0;i<SDL_JoystickNumAxes(Controller::input.joystick);i++)
            Controller::input.axes[i] = 0;

        for(int i=0;i<SDL_JoystickNumHats(Controller::input.joystick);i++)
            Controller::input.hats[i] = SDL_HAT_CENTERED;

        for(int i=0;i<SDL_JoystickNumBalls(Controller::input.joystick);i++) {
            Controller::input.trackballs[i].xrel = 0;
            Controller::input.trackballs[i].yrel = 0;
        }
    }

    else {

        Controller::input.joystick   = NULL;
        Controller::input.buttons    = NULL;
        Controller::input.axes       = NULL;
        Controller::input.hats       = NULL;
        Controller::input.trackballs = NULL;

        throw std::string("The number of the joystick cannot be resolved, all variables set to NULL");
    }
}


/**
 * @brief Controller::destroyInput
 *
 * Destroys the input
 */
void Controller::destroyInput() {

    if(Controller::input.joystick != NULL) {

        Controller::input.numero = 0;

        free(Controller::input.buttons);
        free(Controller::input.axes);
        free(Controller::input.hats);
        free(Controller::input.trackballs);

        SDL_JoystickClose(Controller::input.joystick);
    }
}



void Controller::updateEvent() {

    static SDL_Event evenements;

    for(int i = 0; i < SDL_JoystickNumBalls(Controller::input.joystick); i++) {

        Controller::input.trackballs[i].xrel = 0;
        Controller::input.trackballs[i].yrel = 0;
    }

    while(SDL_PollEvent(&evenements)) {

        if(Controller::input.joystick != NULL
        &&(evenements.jbutton.which == input.numero
        || evenements.jaxis.which   == input.numero
        || evenements.jhat.which    == input.numero
        || evenements.jball.which   == input.numero)) {

            switch(evenements.type) {

                case SDL_JOYBUTTONDOWN:
                    Controller::input.buttons[evenements.jbutton.button] = 1;
                    break;

                case SDL_JOYBUTTONUP:
                    Controller::input.buttons[evenements.jbutton.button] = 0;
                    break;

                case SDL_JOYAXISMOTION:
                    Controller::input.axes[evenements.jaxis.axis] = evenements.jaxis.value;
                    break;

                case SDL_JOYHATMOTION:
                    Controller::input.hats[evenements.jhat.hat] = evenements.jhat.value;
                    break;

                case SDL_JOYBALLMOTION:
                    Controller::input.trackballs[evenements.jball.ball].xrel = evenements.jball.xrel;
                    Controller::input.trackballs[evenements.jball.ball].yrel = evenements.jball.yrel;
                    break;

                default:
                    break;
            }
        }
    }
}



int Controller::joystickListenerCallback(void *data) {

    while (Controller::listenerActive) {
        Controller::updateEvent();
        std::cout << Controller::input.axes[1] << std::endl;
    }

    return 0;
}
