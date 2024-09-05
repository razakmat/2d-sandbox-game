#include "CObject.h"


/*------------------------------------*/
/*----------------COBJECT-------------*/
/*------------------------------------*/


CObject::CObject(SDL_Rect & src, SDL_Texture *& texture,SDL_Rect & dest,const string & name)
{
    m_src = src;
    m_texture = texture;
    m_dest = dest;
    m_name = name;
}

CObject::~CObject()
{
}

SDL_Rect & CObject::GetDest()
{
    return m_dest;
}

int CObject::GetHealth() const
{
    return m_health;
}

string CObject::GetName() const
{
    return m_name;
}


/*------------------------------------*/
/*-----------CMOVINGOBJECT------------*/
/*------------------------------------*/


CMovingObject::CMovingObject(SDL_Rect & src, SDL_Texture *& texture,SDL_Rect & dest,const string & name)
:CObject(src,texture,dest,name),m_position(dest.x,dest.y),m_velocity(0,0)
{
}

CMovingObject::~CMovingObject()
{
}

void CMovingObject::SetCollision(bool col)
{
    m_collision = col;
}

void CMovingObject::SetAir(int air)
{
    m_air = air;
}

int CMovingObject::GetAir() const
{
    return m_air;
}

int & CMovingObject::GetPreSuf()
{
    return m_previousSuffocation;
}

void CMovingObject::Attacking(vector<CObject*> & objects){


    if ( SDL_GetTicks() - m_previousAttack > PREVIOUS_ATTACK ){
        for (auto & x : objects){
            SDL_Rect rec = x->GetDest();
            int length = (rec.x - m_dest.x) *
                         (rec.x - m_dest.x) +
                         (rec.y - m_dest.y) *
                         (rec.y - m_dest.y);
            if ( length < m_range * m_range){
                int health = x->GetHealth();
                health -= m_attack;
                x->SetHealth(health,true);
            }
        }
        m_previousAttack = SDL_GetTicks();
    }
}

bool CMovingObject::CheckInAir(bool player, CMap & maps)
{
    SDL_Rect Rec;
    bool inAir = true;
    for (int i = m_position.GetX(); i <= m_position.GetX() + m_dest.w; i+=m_dest.w){
        CTile * tmp = maps.GetTile(i,m_position.GetY() + m_dest.h,Rec);
        CEffect * effect = tmp->GetEffect();
        if (effect != nullptr) effect->Action(this,Rec,player);
        if (m_collision == true){
            inAir = false;
            int pos = m_position.GetY();
            pos = pos / Rec.h;
            pos = pos * Rec.h;
            m_position.SetY(pos);
            break;
        }
        m_collision = false;
    }
    return inAir;
}

void CMovingObject::CheckAndDoFlying()
{
    if (m_flying){
        int current = SDL_GetTicks();
        if (current - m_previousAir > PREVIOUS_AIR){
            m_velocity.Add(CVector(0,GRAVITY));
            m_position.Add(m_velocity);
            m_previousAir = current;
        }
    }
    m_position.Add(m_velocity);
}


/*------------------------------------*/
/*----------------CPLAYER-------------*/
/*------------------------------------*/


CPlayer::CPlayer(SDL_Rect & src,SDL_Event & event, SDL_Texture *& texture,SDL_Rect & dest,double speed)
:CMovingObject(src,texture,dest,"Player"),m_event(event)
{
    m_speed = speed;
    m_health = HEALTH_PLAYER;
    m_attack = ATTACK_PLAYER;
    m_air = AIR_PLAYER;
    m_range = RANGE_PLAYER;
}

void CPlayer::update(vector<CObject*> & objects, CMap & maps)
{
    m_collision = false;
    CInput::update(m_event,m_velocity,m_speed,m_flying,m_attacking);
    CheckAndDoFlying();

    SDL_Rect Rec;
    for (int i = m_position.GetX(); i <= m_position.GetX() + m_dest.w ; i+=m_dest.w){
        for (int j = m_position.GetY(); j < m_position.GetY() + m_dest.h ;j+=m_dest.h/2){
            CTile * tmp = maps.GetTile(i,j,Rec);
            CEffect * effect = tmp->GetEffect();
            if (effect != nullptr) effect->Action(this,Rec,true);
            if (m_collision == true){
                m_position.SetX(m_dest.x);
                m_collision = false;
            }
        }
    }
    bool inAir = CheckInAir(true,maps);

    if (inAir == true) m_flying = true;
    else {
        m_flying = false;
        Impact(m_velocity.GetY());
        m_velocity.SetY(0);
    }

    if ( m_air < AIR_PLAYER && SDL_GetTicks() - m_previousSuffocation > PREVIOUS_SUFFOCATON){
        m_air = AIR_PLAYER;
        cout << "Your air supply is full." << endl;
    }

    if ( m_health < HEALTH_PLAYER && SDL_GetTicks() - m_previousHealth > PREVIOUS_HEALTH ){
        if (m_health + HEALING > HEALTH_PLAYER) SetHealth(HEALTH_PLAYER);
        else SetHealth(m_health + HEALING);
        cout << "You healed by 10. Your healt is " << m_health << endl;
    }

    if ( m_attacking == true ){
        Attacking(objects);
    }

    m_dest.x = m_position.GetX();
    m_dest.y = m_position.GetY();
}

void CPlayer::draw(SDL_Renderer * renderer, SDL_Rect & m_camera)
{
    SDL_Rect tmp = {m_dest.x - m_camera.x,m_dest.y - m_camera.y,m_dest.w,m_dest.h};
    SDL_RenderCopy(renderer,m_texture,&m_src,&tmp);
}

void CPlayer::SetHealth(int health,bool write)
{
    if (health < 0 ) health = 0;

    if ( write == true ){
        if (health == 0) cout << "You were killed" << endl;
        else if (m_health > health) cout << "You were injured. You have " << health << endl;
    }

    m_health = health;
    m_previousHealth = SDL_GetTicks();
}

void CPlayer::Impact(int speed)
{
    if (m_flying == true) return;
    if ( speed > HIGH_HURT_DEATH ){
        SetHealth(0);
        cout << "You died from the impact on ground." << endl;
    }
    else if ( speed > HIGH_HURT_MIN ){
        SetHealth(m_health - speed);
        if (m_health <= 0){
            m_health = 0;
            cout << "You died from the impact on ground." << endl;
        }
        else cout << "You were injured from impact. You have " << m_health << endl;
    }
}


/*------------------------------------*/
/*----------------CENEMY--------------*/
/*------------------------------------*/


CEnemy::CEnemy(SDL_Rect & src, SDL_Texture *& texture,SDL_Rect & dest,double speed)
:CMovingObject(src,texture,dest,"Enemy")
{
    m_speed = speed;
    m_health = HEALTH_ENEMY;
    m_attack = ATTACK_ENEMY;
    m_range = RANGE_ENEMY;
}

void CEnemy::update(vector<CObject*> & objects, CMap & maps)
{
    if (m_velocity.GetX() == 0 ) m_velocity.SetX(m_speed);
    m_collision = false;

    CheckAndDoFlying();

    SDL_Rect Rec;
    for (int i = m_position.GetX(); i <= m_position.GetX() + m_dest.w ; i+=m_dest.w){
        CTile * tmp = maps.GetTile(i,m_position.GetY(),Rec);
        CEffect * effect = tmp->GetEffect();
        if (effect != nullptr) effect->Action(this,Rec);
        if (m_collision == true){
            m_velocity.SetX(m_velocity.GetX()*-1);
            m_position.SetX(m_dest.x);
            m_collision = false;
        }
    }

    bool inAir = CheckInAir(false,maps);
    if (inAir == true) m_flying = true;
    else {
        m_flying = false;
        if ( m_velocity.GetY() > HIGH_HURT_DEATH ){ m_health = 0;}
        m_velocity.SetY(0);
    }
    Attacking(objects);

    m_dest.x = m_position.GetX();
    m_dest.y = m_position.GetY();
}

void CEnemy::draw(SDL_Renderer * renderer, SDL_Rect & m_camera)
{
    SDL_Rect tmp = {m_dest.x - m_camera.x,m_dest.y - m_camera.y,m_dest.w,m_dest.h};
    SDL_RenderCopy(renderer,m_texture,&m_src,&tmp);
}

void CEnemy::SetHealth(int health,bool write)
{
    if (health < 0 ) health = 0;

    if ( write == true ){
        if (health == 0) cout << "Enemy was killed" << endl;
        else if (m_health > health) cout << "Enemy was injured. He has " << health << endl;
    }
    m_health = health;
}


/*------------------------------------*/
/*----------------CTREE---------------*/
/*------------------------------------*/


CTree::CTree(SDL_Rect & src, SDL_Texture *& texture,SDL_Rect & dest)
:CObject(src,texture,dest,"Tree")
{
    m_health = HEALTH_TREE;
}

void CTree::update(vector<CObject*> & objects, CMap & maps)
{
    SDL_Rect rec = objects.front()->GetDest();
    int length = (rec.x - m_dest.x) *
                         (rec.x - m_dest.x) +
                         (rec.y - m_dest.y) *
                         (rec.y - m_dest.y);

    if ( length < TREE_GROWTH_RANGE && m_height <= TREE_HEIGHT_MAX && SDL_GetTicks() - m_previousGrowth > PREVIOUS_GROWTH ){
        m_height++;
        m_previousGrowth = SDL_GetTicks();
    }
}

void CTree::draw(SDL_Renderer * renderer, SDL_Rect & m_camera)
{
    for ( int i = 0 ; i <= m_dest.h * m_height ; i+=m_dest.h){
        SDL_Rect tmp = {m_dest.x - m_camera.x,m_dest.y - i - m_camera.y,m_dest.w,m_dest.h};
        SDL_RenderCopy(renderer,m_texture,&m_src,&tmp);
    }
}

void CTree::SetHealth(int health,bool write)
{
    if (health < 0 ) health = 0;

    if ( write == true ){
        if (health == 0) cout << "You destroyed tree" << endl;
        else if (m_health > health) cout << "You are attacking tree." << endl;
    }
    m_health = health;
}






