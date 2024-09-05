#include "CInput.h"


void CInput::update(SDL_Event & event,CVector & vel,int speed,bool flying,bool & attacking)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
			{
			case SDLK_w:
                if (flying == false ) vel.SetY(-5);
				break;
			case SDLK_a:
				vel.SetX(-1 * speed);
				break;
			case SDLK_d:
				vel.SetX(1 * speed);
				break;
            case SDLK_e:
				attacking = true;
				break;
			default:
				break;
			}
    }

    if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
			case SDLK_w:
				break;
			case SDLK_a:
				vel.SetX(0);
				break;
			case SDLK_d:
				vel.SetX(0);
				break;
            case SDLK_e:
				attacking = false;
				break;
			default:
				break;
        }
    }
}
