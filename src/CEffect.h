#ifndef CEFFECT_H
#define CEFFECT_H
#include "SDL2/SDL.h"

#define SUFFOCATION_TIME 2000

class CObject;
class CMovingObject;
/**
*  Class CEffect
*   subclasses of different types of effects
*   each effect has different action,
*   which class CObject calls using method Action
*/

/**
*  class CEffect
*   abstract class for different types of effects
*/

class CEffect
{
    public:
        CEffect();
        virtual ~CEffect();
        virtual void Action(CMovingObject * object,SDL_Rect & src,bool player = false) const = 0;
    protected:

    private:
};

/**
*  subclass CCollider
*   moving objects cannot move throght it
*/

class CCollider : public CEffect
{
    public:
        virtual void Action(CMovingObject * object,SDL_Rect & src,bool player = false) const;
    protected:
        bool Collision(SDL_Rect & a, SDL_Rect & b) const;
};

/**
*  subclass CSuffocation
*   lowers healt over time (m_amount)
*   water has this effect
*/

class CSuffocation : public CEffect
{
    public:
        CSuffocation(int amount);
        virtual void Action(CMovingObject * object,SDL_Rect & src,bool player = false) const;
    protected:
        int m_amount; // gives how much damage it causes
        const int m_time = SUFFOCATION_TIME;
};




#endif // CEFFECT_H

