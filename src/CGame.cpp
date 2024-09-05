#include "CGame.h"

CGame::CGame(int width, int height)
:m_map(NULL)
{
    m_widthW = width;
    m_heightW = height;
    m_camera = {0,0,width,height};
    m_isRunning = true;
    m_heightM = HEIGHT_MAP;
    m_widthM = WIDTH_MAP;
}

CGame::~CGame()
{
    if ( m_map != NULL ) delete m_map;
    for ( auto & x : m_images){
        SDL_DestroyTexture(x.second);
    }
    for ( auto & x : m_player){
        delete x;
    }
    for ( auto & x : m_objects){
        delete x;
    }

    if ( m_window != NULL ) SDL_DestroyWindow(m_window);
    if ( m_renderer != NULL ) SDL_DestroyRenderer(m_renderer);
    IMG_Quit();
	SDL_Quit();
}

void CGame::InitGame(const char * name)
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
        throw CException("SDL could not be initialized.");
    }
    m_window = SDL_CreateWindow(name,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,m_widthW,m_heightW,SDL_WINDOW_SHOWN);
    if( m_window == NULL ){
        throw CException("SDL window could not be created.");
    }
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    if( m_renderer == NULL ){
        throw CException("Renderer could not be created.");
    }
    int imageFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imageFlags ) & imageFlags ) ){
        throw CException("SDL_image could not be initialized.");
    }
    LoadImages();
}

void CGame::InitLoad(int width,int height,const string & tilemap)
{
    m_heightM = height;
    m_widthM = width;
    m_map = new CMap(TILE_SIZE,width,height);
    m_map->LoadImages(TILESET_PATH,NUMBER_OF_TILES,m_renderer);
    m_map->LoadMap(tilemap);
}

void CGame::Init()
{
    m_map = new CMap(TILE_SIZE,WIDTH_MAP,HEIGHT_MAP);
    m_map->LoadImages(TILESET_PATH,NUMBER_OF_TILES,m_renderer);
    m_map->LoadMap(TILEMAP_PATH);
    CreateObjects();
}


void CGame::HandleEvents()
{
    SDL_PollEvent(&m_event);
    switch (m_event.type){
        case SDL_QUIT :
            m_isRunning = false;
            break;
        case SDL_KEYDOWN :
            switch (m_event.key.keysym.sym){
                case SDLK_u:
                    SafeGame();
                    break;
                case SDLK_q:
                    m_isRunning = false;
                    break;
                default:
                    break;
			}
            break;
        default:
            break;
	}
}

void CGame::Update()
{
    for ( auto & x : m_objects){
        x->update(m_player,*m_map);
        if (x->GetHealth() <= 0){
            delete x;
            x = nullptr;
        }
    }
    m_objects.erase(std::remove_if(
            m_objects.begin(),
            m_objects.end(),
            [](CObject * e){return e == nullptr;}),
                    m_objects.end());

    for ( auto & x : m_player){
        x->update(m_objects,*m_map);
    }
    if (m_player.front()->GetHealth() <= 0){
        m_isRunning = false;
    }

    if ( m_objects.empty() ){
        cout << "You destroyed all there is to destroy." << endl;
        m_isRunning = false;
    }
}

void CGame::Render()
{
    SDL_RenderClear(m_renderer);
    m_map->drawMap(m_renderer,m_camera);
    for ( auto & x : m_objects){
        x->draw(m_renderer,m_camera);
    }
    for ( auto & x : m_player){
        x->draw(m_renderer,m_camera);
    }
    SDL_RenderPresent(m_renderer);
    SDL_Rect play = m_player.front()->GetDest();
    m_camera.x = play.x - m_widthW / 2;
    m_camera.y = play.y - m_heightW / 2;
    if (m_camera.x < 0 ) m_camera.x = 0;
    if (m_camera.y < 0 ) m_camera.y = 0;
    if (m_camera.x + m_camera.w > (m_widthM - 1) * TILE_SIZE) m_camera.x = (m_widthM - 1) * TILE_SIZE - m_camera.w;
    if (m_camera.y + m_camera.h > (m_heightM - 1) * TILE_SIZE) m_camera.y = (m_heightM - 1) * TILE_SIZE - m_camera.h;
}

void CGame::SafeGame() const
{
    string file_name;
    cout << "Write name of the new file : ";
    cin >> file_name;

    ofstream file;
    file.open(file_name);
    if (!file.good()){
        throw CException("Error with opening file " + file_name);
    }

    file << TILEMAP_PATH << "\n";
    file << HEIGHT_MAP << "\n";
    file << WIDTH_MAP << "\n";

    file << m_player[0]->GetHealth() << "\n";
    SDL_Rect rec = m_player[0]->GetDest();
    file << rec.x << " " << rec.y << " " << rec.h << " " << rec.w << "\n";
    for (auto & x : m_objects){
        file << x->GetName() << "\n";
        file << x->GetHealth()<< "\n";
        rec = x->GetDest();
        file << rec.x << " " << rec.y << " " << rec.h << " " << rec.w << "\n";
    }

    if (file.fail()){
        throw CException("Error with file " + file_name);
    }

    file.close();

    cout << "Game was succesfully saved." << endl;
}

void CGame::LoadGame()
{
    string file_name;
    cout << "Enter name of your saved file : ";
    cin >> file_name;

    ifstream file;
    file.open(file_name);

    if (!file.good()){
        throw CException("Error with opening file " + file_name);
    }

    string tilemap;
    int height;
    int width;

    file >> tilemap;
    file >> height;
    file >> width;

    InitLoad(width,height,tilemap);

    string name;
    int health;
    SDL_Rect dest;

    file >> health;
    file >> dest.x >> dest.y >> dest.h >> dest.w;

    if (dest.x <= 0 || dest.y <= 0 ||
        dest.x + dest.w >= width * TILE_SIZE ||
        dest.y + dest.h >= height * TILE_SIZE ){
        throw CException("Corrupted contents of file " + file_name);
    }

    auto it = m_images.find("player");
    SDL_Rect recP = {0,0,SRC_WIDTH_PLAYER,SRC_HEIGHT_PLAYER};
    CPlayer * player = new CPlayer(recP,m_event,it->second,dest,SPEED_PLAYER);
    player->SetHealth(health);
    m_player.push_back(player);

    SDL_Rect recE = {0,0,SRC_OF_ENEMY,SRC_OF_ENEMY};
    SDL_Rect recT = {0,0,SRC_OF_TREE,SRC_OF_TREE};
    auto itenemy = m_images.find("enemy");
    auto ittree = m_images.find("tree");


    while (file >> name){
        file >> health;
        file >> dest.x >> dest.y >> dest.h >> dest.w;
        if (dest.x <= 0 || dest.y <= 0 ||
            dest.x + dest.w >= width * TILE_SIZE ||
            dest.y + dest.h >= height * TILE_SIZE ){
            throw CException("Corrupted contents of file " + file_name);
        }

        if ( name == "Enemy" ){
            CEnemy * enemy = new CEnemy(recE,itenemy->second,dest,SPEED_ENEMY);
            enemy->SetHealth(health);
            m_objects.push_back(enemy);
        }
        else if ( name == "Tree" ){
            CTree * tree = new CTree(recT,ittree->second,dest);
            tree->SetHealth(health);
            m_objects.push_back(tree);
        }
    }

    if (file.fail()){
        throw CException("Error with file " + file_name);
    }
    file.close();
}

void CGame::LoadImages()
{
    SDL_Texture * tex = m_map->GetTexture(IMAGE_PLAYER,m_renderer);
    m_images.insert(pair<string,SDL_Texture*>("player",tex));
    tex = m_map->GetTexture(IMAGE_ENEMY,m_renderer);
    m_images.insert(pair<string,SDL_Texture*>("enemy",tex));
    tex = m_map->GetTexture(IMAGE_TREE,m_renderer);
    m_images.insert(pair<string,SDL_Texture*>("tree",tex));
}

bool CGame::IsRunning() const
{
    return m_isRunning;
}

int CGame::findPosition(int x)const
{
    string name ="Air";
    int i = 0;
    while ( name == "Air" || name == "Water" ){
        SDL_Rect rec;
        i += TILE_SIZE;
        CTile * tmp = m_map->GetTile(x,i,rec);
        name = tmp->GetName();
    }
    return i;
}


void CGame::CreateObjects()
{
    SDL_Rect tmp = {0,0,SRC_WIDTH_PLAYER,SRC_HEIGHT_PLAYER};
    auto it = m_images.find("player");
    int posY = findPosition( STARTING_X_PLAYER );
    SDL_Rect dest = {STARTING_X_PLAYER ,posY - DEST_HEIGHT_PLAYER ,DEST_WIDTH_PLAYER, DEST_HEIGHT_PLAYER};
    m_player.push_back(new CPlayer(tmp,m_event,it->second,dest,SPEED_PLAYER));

    srand (time(NULL));

    tmp = {0,0,SRC_OF_ENEMY,SRC_OF_ENEMY};
    it = m_images.find("enemy");
    for ( int i = 0 ; i < NUMBER_OF_ENEMYS ; i++){
        int random = ( rand() % ( m_widthM - 20 ) + 10 ) * TILE_SIZE;
        int posY = findPosition( random );
        dest = {random,posY - DEST_OF_ENEMY ,DEST_OF_ENEMY,DEST_OF_ENEMY};
        CObject * enemy = new CEnemy(tmp,it->second,dest,SPEED_ENEMY);
        m_objects.push_back(enemy);
    }

    tmp = {0,0,SRC_OF_TREE,SRC_OF_TREE};
    it = m_images.find("tree");
    for ( int i = 0 ; i < NUMBER_OF_TREES ; i++){
        int random = ( rand() % ( m_widthM - 20 ) + 10 ) * TILE_SIZE;
        int posY = findPosition( random );
        dest = {random,posY - DEST_OF_TREE ,DEST_OF_TREE,DEST_OF_TREE};
        CObject * tree = new CTree(tmp,it->second,dest);
        m_objects.push_back(tree);
    }
}












