#include "CTile.h"

CTile::CTile( const string & name, int tileSize, int src, SDL_Texture * tex)
{
    m_name = name;
    m_tileSize = tileSize;
    m_src = {src * tileSize,0,tileSize,tileSize};
    m_texture = tex;
    m_effect = NULL;
}

CTile::~CTile()
{
    if (m_effect != NULL) delete m_effect;
}

void CTile::DrawTile(SDL_Renderer * renderer,SDL_Rect & dest)
{
    SDL_RenderCopy(renderer,m_texture,&m_src,&dest);
}

CEffect * CTile::GetEffect() const
{
    return m_effect;
}

string CTile::GetName() const
{
    return m_name;
}

CGrass::CGrass(int tileSize,int src,SDL_Texture * tex)
:CTile("Grass",tileSize,src,tex)
{
    m_effect = new CCollider();
}

CDirt::CDirt(int tileSize,int src,SDL_Texture * tex)
:CTile("Dirt",tileSize,src,tex)
{
    m_effect = new CCollider();
}

CWater::CWater(int tileSize,int src,SDL_Texture * tex)
:CTile("Water",tileSize,src,tex)
{
    m_effect = new CSuffocation(1);
}

CRock::CRock(int tileSize,int src,SDL_Texture * tex)
:CTile("Rock",tileSize,src,tex)
{
    m_effect = new CCollider();
}

CAir::CAir(int tileSize,int src,SDL_Texture * tex)
:CTile("Air",tileSize,src,tex)
{
    m_effect = NULL;
}

