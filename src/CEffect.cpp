#include "CEffect.h"
#include "CObject.h"


CEffect::CEffect()
{
}

CEffect::~CEffect()
{
}

void CCollider::Action(CMovingObject * object,SDL_Rect & src,bool player) const
{
    bool col = Collision(object->GetDest(),src);
    object->SetCollision(col);
}

bool CCollider::Collision(SDL_Rect & a, SDL_Rect & b) const
{
    if (a.x + a.w >= b.x &&
		b.x + b.w >= a.x &&
		a.y + a.h >= b.y &&
		b.y + b.h >= a.y) return true;
    return true;
}

CSuffocation::CSuffocation(int amount)
{
    m_amount = amount;
}

void CSuffocation::Action(CMovingObject * object,SDL_Rect & src,bool player) const
{
    if (player == false) return;
    SDL_Rect rec = object->GetDest();
    if (src.y <= rec.y){
        int & prev = object->GetPreSuf();
        int now = SDL_GetTicks();
        if (now - prev > m_time){
            int air = object->GetAir();
            if (air == 0){
                int health = object->GetHealth();
                health -= m_amount * 2;
                object->SetHealth(health);
                if (health <= 0 ) cout << "you died from suffocation." << endl;
                else cout << "You are suffocating. You have " << health << " of health."  << endl;
            }
            else{
                air -= m_amount;
                if ( air < 0) air = 0;
                object->SetAir(air);
                cout << "You have " << air << " of air." << endl;
            }
        prev = now;
        }
    }
}





