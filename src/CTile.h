#ifndef CTILE_H
#define CTILE_H
#include <string>
#include "SDL2/SDL.h"
#include "CEffect.h"
using namespace std;

/**
*  Class CTile
*   Holds individual tiles from our game map
*   Different subclasses of class CTile differs
*   in specific effect they carry viz. class CEffect
*   CTile holds info about their image and their effect
*/


class CTile
{
    public:
        CTile( const string & name, int tileSize, int src, SDL_Texture * tex);
        virtual ~CTile();
        void DrawTile(SDL_Renderer * renderer,SDL_Rect & dest);
        CEffect * GetEffect() const;
        string GetName() const;
    protected:
        string m_name; // name of Tile e.g. Water or Grass
        int m_tileSize; // Size of Tile
        SDL_Texture * m_texture; // holds image of set of Tiles
        CEffect * m_effect; // effect of this Tile
        SDL_Rect m_src; // specify which image in set of m_texture
};

class CGrass : public CTile
{
    public:
        CGrass(int tileSize,int src,SDL_Texture * tex);
};

class CDirt : public CTile
{
    public:
        CDirt(int tileSize,int src,SDL_Texture * tex);
};

class CWater : public CTile
{
    public:
        CWater(int tileSize,int src,SDL_Texture * tex);
};

class CRock : public CTile
{
    public:
        CRock(int tileSize,int src,SDL_Texture * tex);
};

class CAir : public CTile
{
    public:
        CAir(int tileSize,int src,SDL_Texture * tex);
};



#endif // CTILE_H
