/** 
 * @mainpage
 * @section Description
 * 
 * it's sandbox type of game. Requirements were :
 *		Implement player and his attributes
 *  	Create differents models of hurts
 *   	Create simple model of environment
 *  	Implement saving and loading of game
 */
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include "string"
#include "stdio.h"
#include "CGame.h"
#include "CException.h"
#include <iostream>

#define FPS 60
#define FRAMEDELAY ( 1000 / FPS )
#define WIDTHWINDOW 800
#define HEIGHTWINDOW 640
using namespace std;

/**
 * Function - main
 *  main keeps program in loop as long as is
 *  bool return of game->isRunning true.
 *  Main class of game is CGame.
 *
 * @author Matej Razak
 */


int main (int argc, char * argv[]){

	int frameStart;
	int frameTime;

    char c;
    do{
        cout << "Controls : " <<endl;
        cout << "a - move left" << endl;
        cout << "d - move rigth" << endl;
        cout << "w - jump" << endl;
        cout << "e - attack" << endl;
        cout << "u - save game" << endl;
        cout << "q - quit game" << endl;

        cout << " Would you like to start (n)ew game or (s)aved game or to (e)xit ? : ";
        cin >> c;
        if (c == 'n' || c== 's'){
            CGame * game = NULL;
            try{
                game = new CGame( WIDTHWINDOW, HEIGHTWINDOW);
                game->InitGame("GameWindow");
                if ( c == 'n') game->Init();
                else game->LoadGame();
                while (game->IsRunning())
                {
                    frameStart = SDL_GetTicks();

                    game->HandleEvents();
                    game->Update();
                    game->Render();

                    frameTime = SDL_GetTicks() - frameStart;

                    if (FRAMEDELAY > frameTime)
                    {
                        SDL_Delay(FRAMEDELAY - frameTime);
                    }
                }
            }
            catch(CException & e){
                cout << e.what() << endl;
            }
            catch(exception & e){
                cout << "Standard exception: " << e.what() << endl;
            }
            if (game != NULL ) delete game;
        }
        else if ( c!= 'e' ){
                cout << "Incorrect choice!" << endl;
        }
    }while(c != 'e');



    return 0;
}
