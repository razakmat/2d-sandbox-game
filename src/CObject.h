#ifndef COBJECT_H
#define COBJECT_H

#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <vector>
#include "CVector.h"
#include "CMap.h"
#include "CInput.h"
#include <iostream>
#include <string>

#define ATTACK_PLAYER 10
#define ATTACK_ENEMY 5
#define HEALTH_PLAYER 100
#define HEALTH_ENEMY 50
#define HEALTH_TREE 30
#define RANGE_PLAYER 70
#define RANGE_ENEMY 70
#define AIR_PLAYER 10
#define GRAVITY 0.5
#define PREVIOUS_AIR 30
#define PREVIOUS_SUFFOCATON 4000
#define PREVIOUS_HEALTH 7000
#define HEALING 10
#define HIGH_HURT_DEATH 10
#define HIGH_HURT_MIN 7
#define PREVIOUS_ATTACK 800
#define TREE_HEIGHT_MAX 5
#define PREVIOUS_GROWTH 8000
#define TREE_GROWTH_RANGE 10000


using namespace std;

/**
*  Class CObject
*   holds objects which will be on map besides tiles
*   can easily be expanded
*/

class CObject
{
    public:
        CObject(SDL_Rect & src, SDL_Texture *& texture,SDL_Rect & dest, const string & name);
        virtual ~CObject();
        /**
		*  updates current situation of object
		*/
        virtual void update(vector<CObject*> & objects, CMap & maps) = 0 ;
        /**
		*  draws current situaton given from update in game window
		*/
        virtual void draw(SDL_Renderer * renderer, SDL_Rect & m_camera) = 0 ;
        /**
		*  return position of object on game map
		*/
        SDL_Rect & GetDest();
        int GetHealth() const; // return health of object
        virtual void SetHealth(int health,bool write = false) = 0; // sets Health of object
        string GetName() const; // return name(type) of object e.g. player,enemy 
    protected:
        SDL_Texture * m_texture; // image of object
        SDL_Rect m_src; // size of image of object
        SDL_Rect m_dest; // size of object on map
        string m_name; // name of object
        int m_health; // health of object
        int m_previousHealth = 0; // time since last change in health

};

/**
*  subclass CMovingObject
*   holds every moving object in class Object
*/


class CMovingObject : public CObject
{
    public:
        CMovingObject(SDL_Rect & src, SDL_Texture *& texture,SDL_Rect & dest,const string & name);
        virtual ~CMovingObject();
        virtual void update(vector<CObject*> & objects, CMap & maps) = 0 ;
        virtual void draw(SDL_Renderer * renderer, SDL_Rect & m_camera) = 0 ;
        void SetCollision(bool col); // sets if there is collision with some tile on map
        void SetAir(int air); // sets current amount of air object has (underwater)
        int GetAir() const; // returns how much air object has (underwater)
        int & GetPreSuf(); // time from last suffering from suffocation
        virtual void SetHealth(int health,bool write = false) = 0;
    protected:
        void Attacking(vector<CObject*> & objects); // method for objects attacking other objects
        bool CheckInAir(bool player, CMap & maps); // checks if objects is flying
        void CheckAndDoFlying(); // calculates gravity if flying
        CVector m_position; // current position
        CVector m_velocity; // current movement
        bool m_collision; // collision with tile with effect collision
        double m_speed; // speed of object
        int m_air; // amount of air object has
        int m_attack; // how much damage object causes
        int m_range; // range of attack
        bool m_flying =false; // objects status of flying
        int m_previousSuffocation = 0; // time from last suffocation
        int m_previousAttack = 0; // time from last attack
        int m_previousAir = 0; // time from last adjustment of object in air (gravity)
};

/**
*  subclass CPlayer
*    this class holds information about our player
*/

class CPlayer : public CMovingObject
{
    public:
        CPlayer(SDL_Rect & src, SDL_Event & event, SDL_Texture *& texture,SDL_Rect & dest,double speed);
        virtual void update(vector<CObject*> & objects, CMap & maps);
        virtual void draw(SDL_Renderer * renderer, SDL_Rect & m_camera);
        virtual void SetHealth(int health,bool write = false);
    protected:
        void Impact(int speed);
        SDL_Event & m_event; // for catching events like keyboard presses
        bool m_attacking = false; // indicator if player is attacking, based on event (pressed button) 
};

/**
*  subclass CEnemy
*    this class holds information about moving enemy
*	 if close to our player cases damage (attacking enemy)
*/

class CEnemy : public CMovingObject
{
    public:
        CEnemy(SDL_Rect & src, SDL_Texture *& texture,SDL_Rect & dest,double speed);
        virtual void update(vector<CObject*> & objects, CMap & maps);
        virtual void draw(SDL_Renderer * renderer, SDL_Rect & m_camera);
        virtual void SetHealth(int health,bool write = false);
    protected:
};

/**
*  Class CTree
*   object representing tree
*   over time tree grows if player is near
*/

class CTree : public CObject
{
    public:
        CTree(SDL_Rect & src, SDL_Texture *& texture,SDL_Rect & dest);
        virtual void update(vector<CObject*> & objects, CMap & maps);
        virtual void draw(SDL_Renderer * renderer, SDL_Rect & m_camera);
        virtual void SetHealth(int health,bool write = false);
    protected:
        int m_previousGrowth = 0; // time from last growth
        int m_height = 0; // current height of tree
};





#endif // COBJECT_H
