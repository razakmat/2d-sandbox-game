#ifndef CGAME_H
#define CGAME_H

#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include "CMap.h"
#include "CObject.h"
#include "CException.h"
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <map>

#define TILE_SIZE 32
#define NUMBER_OF_TILES 5
#define NUMBER_OF_ENEMYS 3
#define NUMBER_OF_TREES 5
#define SRC_HEIGHT_PLAYER 64
#define SRC_WIDTH_PLAYER 32
#define DEST_HEIGHT_PLAYER 64
#define DEST_WIDTH_PLAYER 32
#define STARTING_X_PLAYER 256
#define SPEED_PLAYER 3
#define SPEED_ENEMY 0.5
#define SRC_OF_ENEMY 16
#define SRC_OF_TREE 16
#define DEST_OF_ENEMY 32
#define DEST_OF_TREE 32
#define HEIGHT_MAP 100
#define WIDTH_MAP 200
#define TILESET_PATH "examples/tileset3.png"
#define TILEMAP_PATH "examples/tilemap2.txt"
#define IMAGE_PLAYER "examples/player2.png"
#define IMAGE_ENEMY "examples/enemy.png"
#define IMAGE_TREE "examples/tree.png"



using namespace std;

/**
 * Class CGame
 *  main class of game.
 *  functions in this class are called
 *  from main function till end of program
 */

class CGame
{
    public:
        CGame(int width, int height); // constructor with sizes of game window
        virtual ~CGame();
        /**
 		* method for inicialization of map and map objects
 		*  from saved game
 		*/
        void InitLoad(int width, int height, const string & tilemap); 
        void Init(); // inicialization of map and map objects
        void InitGame(const char * name); // inicialization of SDL features
        /**
        * method handleEvents is used for
        * knowing if we want to quit and saving game
        */
        void HandleEvents();
        /**
        * method update is used for
        *   updating current status of the game
        *   based on our actions
        */
        void Update();
        /**
        * method render is used for
        *   drawing current state of game into our window
        */
        void Render();
        void SafeGame() const;
        void LoadGame();
        /**
        * method isRunning is used for finding out
        * if game is supposed to end
        */
        void LoadImages();
        bool IsRunning() const; // checks if game is running
    private:
        int findPosition(int x) const; // finds where it's safe to put object on map
        void CreateObjects(); // creates objects on map on random locations, together with method findPosition
        SDL_Renderer * m_renderer; // game renderer
        SDL_Window * m_window; // game window
        int m_heightW; // height of game window
        int m_widthW; // windth of game winoow
        SDL_Event m_event; // for catching events like keyboard presses
        SDL_Rect m_camera; // shows which area of map to show in game window
        bool m_isRunning; // status of game
        CMap * m_map; // everything about map of game
        int m_heightM; // height of game map
        int m_widthM; // width of game map
        map< string, SDL_Texture*> m_images; // holds game images like player, enemy, ...
        vector<CObject*> m_player; // contains player
        vector<CObject*> m_objects;// conatins game objects ( minus player )

};

#endif // CGAME_H
