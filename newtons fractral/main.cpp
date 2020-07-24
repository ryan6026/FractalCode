#include "MBset.h"
#include "Display.h"
#include <SDL.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <random>
#include <complex>
#define PI 3.14159265359
#define ImageWidth 300
#define ImageHeight ImageWidth
using namespace std;

int main(int argc, char* args[])
{
    SDL_Point P;
    P.x = 200;
    P.y=0;
    GUI *Display;
    Display = new GUI(ImageHeight, ImageWidth, "Julia Set", P);
    Display->setGradCoef(1);
    double cr=3, ci=0;
    int x, y;
    const Uint8 *KBstate;

    MBset* lol = new MBset(ImageWidth, ImageHeight, 15);

    while(true)
    {
        SDL_PumpEvents();
        KBstate = SDL_GetKeyboardState(NULL);
        if (KBstate[SDL_SCANCODE_KP_8])
        {
            lol->ShiftUp();
            Display->render();
        }
        else if (KBstate[SDL_SCANCODE_KP_2])
        {
            lol->ShiftDown();
            Display->render();
        }
        else if (KBstate[SDL_SCANCODE_KP_6])
        {
            lol->ShiftRight();
            Display->render();
        }
        else if (KBstate[SDL_SCANCODE_KP_4])
        {
            lol->ShiftLeft();
            Display->render();
        }
        else if (KBstate[SDL_SCANCODE_KP_9])
        {
            lol->ZoomIn();
            Display->render();
        }
        else if (KBstate[SDL_SCANCODE_KP_7])
        {
            lol->ZoomOut();
            Display->render();
        }
        else if (KBstate[SDL_SCANCODE_KP_PLUS])
        {
            lol->AddIt();
            cout << "iterations*2" << endl;
        }
        else if (KBstate[SDL_SCANCODE_KP_MINUS])
        {
            lol->SubIt();
            cout << "iterations/2" << endl;
        }
        else if(KBstate[SDL_SCANCODE_KP_0])
        {
            cout << "saved " << rand() << ".bmp" << endl;
            Display->save(rand(), rand());
        }
        if(SDL_GetMouseState(&x, &y)){
            if(SDL_BUTTON((SDL_BUTTON_LEFT))){
                cr += .5*((double)x/ImageWidth-.5);
                ci += .5*((double)y/ImageHeight-.5);
            }
        }

        Display->plot3MBsetHist(lol->Newton(complex<double>(cr, ci)));
        Display->render();

    }
    return 0;
}

