#ifndef CINPUT_H
#define CINPUT_H

#include "CVector.h"
#include "SDL2/SDL.h"
#include <iostream>

/**
*  Class CInput
*   class for being able to use keyboard
*   for interacting with our player
*/

class CInput
{
    public:
        static void update(SDL_Event & event,CVector & vel,int speed,bool flying,bool & attacking);
    protected:
};

#endif // CINPUT_H
