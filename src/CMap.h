#ifndef CMAP_H
#define CMAP_H
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include "CTile.h"
using namespace std;

/**
* Class CMap
*   manages map of our game
*   Loads map from file to m_srcMap,
*   then transforms this to to our game map m_map
*   Map is 2D array of different types of tiles
*/

class CMap
{
    public:
        /**
        *  constructor CMap
        *   name - name of map
        *   tileSize - size of tiles
        *   x,y - size of our game map
        */
        CMap(int tileSize, int x, int y);
        ~CMap();
         /**
        * method LoadImages
        *   loads images used for individuals tiles
        */
        void LoadImages(const string & path, int count, SDL_Renderer * renderer);
         /**
        *  method LoadMap
        *   loads map from file and creates from it our game map
        *   sizeX and sizeY are sizes of map in file
        */
        void LoadMap(const string & path);
        /**
        *  method drawMap
        *   draws our map in range of camera to our window
        */
        void drawMap(SDL_Renderer * renderer, SDL_Rect & camera);
        /**
        * method GetTile
        *   returns tile in specific location
        */
        CTile * GetTile(int x,int y,SDL_Rect & src) const;
        /**
        * method GetTexture
        *    loads image file to SDL_Texture
        */
        SDL_Texture * GetTexture(const string & path, SDL_Renderer * renderer) const;
    private:
        /**
        *  method CreateRealMap
        *   transforms map from file to our game map
        */
        void CreateRealMap();
        /**
        *  method RewriteOneLine
        *   is helping method for method CreateRealMap
        */
        void RewriteOneLine(int column);
        enum TileType{
            Air = 0,
            Grass = 1,
            Dirt = 2,
            Rock = 3,
            Water = 4,
        };
        struct Map{
            CTile *** m_map;
            int m_x;
            int m_y;
        };
        map< TileType, CTile*> m_tiles; // keeps every type of Tiles in one place
        Map m_map; // actual map for playing
        Map m_srcMap; // map that holds map from source file
        int m_tileSize; // size of individual tile
        SDL_Texture * m_tileTexture; // holds image of set of tiles for destuction 

};

#endif // CMAP_H

