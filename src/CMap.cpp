#include "CMap.h"
#include "CGame.h"

CMap::CMap(int tileSize,int x, int y)
{
    m_srcMap.m_map = NULL;
    m_tileSize = tileSize;
    m_map.m_x = x;
    m_map.m_y = y;
    m_map.m_map = new CTile**[x];
    for (int i = 0 ; i < x; i++){
        m_map.m_map[i] = new CTile*[y];
    }
}

CMap::~CMap()
{
    for (int i = 0 ; i < m_map.m_x; i++){
        delete m_map.m_map[i];
    }
    delete m_map.m_map;

    if ( m_srcMap.m_map != NULL ){
        for (int i = 0 ; i < m_srcMap.m_x; i++){
            delete m_srcMap.m_map[i];
        }
        delete m_srcMap.m_map;
    }

    for (auto & x : m_tiles){
        delete x.second;
    }
    SDL_DestroyTexture(m_tileTexture);
}

void CMap::LoadImages(const string & path ,int count, SDL_Renderer * renderer)
{
    m_tileTexture = GetTexture(path,renderer);
    m_tiles.insert(pair<TileType,CTile*>(Grass,new CGrass(m_tileSize,1,m_tileTexture)));
    m_tiles.insert(pair<TileType,CTile*>(Dirt,new CDirt(m_tileSize,2,m_tileTexture)));
    m_tiles.insert(pair<TileType,CTile*>(Water,new CWater(m_tileSize,4,m_tileTexture)));
    m_tiles.insert(pair<TileType,CTile*>(Rock,new CRock(m_tileSize,3,m_tileTexture)));
    m_tiles.insert(pair<TileType,CTile*>(Air,new CAir(m_tileSize,0,m_tileTexture)));
}

void CMap::LoadMap(const string & path)
{
	ifstream file;
	file.open(path);

	if (!file.good()){
        throw CException("Error with opening file " + path);
    }

	int sizeX = -1;
    int sizeY = -1;
    file >> sizeX;
    file >> sizeY;


    if ( sizeX < 5 ||
         sizeY < 5 ||
         sizeX > m_map.m_x - 10 ||
         sizeY > m_map.m_y - 10 ){
         throw CException("Wrong sizes of maps.");
    }



    m_srcMap.m_map = new CTile**[sizeX];
    for (int i = 0 ; i < sizeX; i++){
        m_srcMap.m_map[i] = new CTile*[sizeY];
    }
    m_srcMap.m_x = sizeX;
    m_srcMap.m_y = sizeY;

    for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
            int c;
            file >> c;
			auto it = m_tiles.find((TileType)c);
			if (it == m_tiles.end()){
                throw CException("Wrong contents of file " + path);
			}
			m_srcMap.m_map[x][y] = it->second;
		}
	}

	if (file.fail()){
        throw CException("Error with file " + path);
    }

	file.close();
    CreateRealMap();
}

void CMap::drawMap(SDL_Renderer * renderer, SDL_Rect & camera)
{
    int x = camera.x / m_tileSize;
    int y = camera.y / m_tileSize;
    int height = camera.h / m_tileSize;
    int width = camera.w / m_tileSize;

    for ( int i = x ; i < x + width + 1 ; i++){
        for ( int j = y ; j < y + height + 1 ; j++){
                SDL_Rect dest = {   i * m_tileSize - camera.x,
                                    j * m_tileSize - camera.y,
                                    m_tileSize ,
                                    m_tileSize };
                m_map.m_map[i][j]->DrawTile(renderer,dest);
        }
    }
}

CTile * CMap::GetTile(int x,int y,SDL_Rect & src) const
{
    src = {x,y,m_tileSize,m_tileSize};
    x = x / m_tileSize;
    y = y / m_tileSize;
    return m_map.m_map[x][y];
}

SDL_Texture * CMap::GetTexture(const string & path, SDL_Renderer * renderer) const
{
    SDL_Surface* tempSurface = IMG_Load(path.c_str());
    if( tempSurface == NULL ){
        throw CException("Could not load image " + path);
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer,tempSurface);
    if( tex == NULL ){
        throw CException("Could not create texture from " + path);
    }
    SDL_FreeSurface(tempSurface);
    return tex;
}

void CMap::CreateRealMap()
{
    auto itRock = m_tiles.find(Rock);
    auto itAir = m_tiles.find(Air);
    for (int i = 0; i < m_map.m_y; i++){
        m_map.m_map[0][i] = itRock->second;
        m_map.m_map[1][i] = itAir->second;
        m_map.m_map[2][i] = itAir->second;
        m_map.m_map[m_map.m_x-1][i] = itRock->second;
        m_map.m_map[m_map.m_x-2][i] = itAir->second;
        m_map.m_map[m_map.m_x-3][i] = itAir->second;
    }
    m_map.m_map[1][m_map.m_y-2] = itRock->second;
    m_map.m_map[2][m_map.m_y-2] = itRock->second;
    m_map.m_map[m_map.m_x-2][m_map.m_y-2] = itRock->second;
    m_map.m_map[m_map.m_x-3][m_map.m_y-2] = itRock->second;

    for (int i = 3; i < m_map.m_x - 3; i++){
        RewriteOneLine(i);
    }
}

void CMap::RewriteOneLine(int column)
{
    auto itAir = m_tiles.find(Air);
    auto itRock = m_tiles.find(Rock);
    int i = 0;
    int startMap = ( m_map.m_y - m_srcMap.m_y ) / 2;
    for ( ; i < startMap ; i++){
        m_map.m_map[column][i] = itAir->second;
    }
    for ( ; i < startMap + m_srcMap.m_y ; i++){
        m_map.m_map[column][i] = m_srcMap.m_map[column % m_srcMap.m_x][i - startMap];
    }
    for ( ; i < m_map.m_y ; i++){
        m_map.m_map[column][i] = itRock->second;
    }
}










