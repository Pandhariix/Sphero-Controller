#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <SDL.h>
#include <SDL_thread.h>


typedef struct InputTrackball
{
    int xrel;
    int yrel;

} InputTrackball;


typedef struct Input
{
    SDL_Joystick   *joystick;
    char           *buttons;
    int            *axes;
    int            *hats;
    InputTrackball *trackballs;
    int             numero;

} Input;



class Controller {

public:

    Controller();
    ~Controller();

    void displayJoysticksInformations();
    void launchJoystickListener(int joystickNum);
    void stopJoystickListener();
    void clear();

private:

    static Input input;
    static bool  listenerActive;

    SDL_Thread   *listenerThread;

    void        initializeSDL();
    void        destroySDL();
    void        initializeInput(int joystickNum);
    void        destroyInput();
    static void updateEvent();
    static int  joystickListenerCallback(void *data);
};


#endif //CONTROLLER_H
