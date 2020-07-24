#include "MBset.h"
#include "Display.h"
#include <SDL2\SDL.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <random>
#define PI 3.14159265359
#define ImageWidth 300
#define ImageHeight 300
using namespace std;

int main(int argc, char* args[]) {
    SDL_Point P;
        P.x = 400;
        P.y=0;
    GUI *Display;
        Display = new GUI(ImageHeight, ImageWidth, "Julia Set", P);
        Display->setGradCoef(.01);
    GUI *DisplayLoc;
        DisplayLoc = new GUI(150, 150, "C Location", Display->getTopRightCorner());
        DisplayLoc->plotAxes();
        DisplayLoc->render();
    double cr=.14, ci=.762;
    int x, y;
    MBset* lol = new MBset(ImageWidth, ImageHeight, 50);

    bool control = true;
    const Uint8 *KBstate;
    while(true) {
        SDL_PumpEvents();
        KBstate = SDL_GetKeyboardState(NULL);
        if (KBstate[SDL_SCANCODE_W]) {
            lol->ShiftDown();
        } else if (KBstate[SDL_SCANCODE_S]) {
            lol->ShiftUp();
        } else if (KBstate[SDL_SCANCODE_D]) {
            lol->ShiftRight();
        } else if (KBstate[SDL_SCANCODE_A]) {
            lol->ShiftLeft();
        } else if (KBstate[SDL_SCANCODE_R]) {
            lol->ZoomIn();
        } else if (KBstate[SDL_SCANCODE_F]) {
            lol->ZoomOut();
        } else if(KBstate[SDL_SCANCODE_E]){
            control = !control;
        }/*else if(KBstate[SDL_SCANCODE_Q]){
            Display->save("test");
            cout << "pic saved!" << endl;
        }*/
        if(control){
            if(SDL_GetMouseState(&x, &y)&&SDL_BUTTON((SDL_BUTTON_LEFT))){
            cr += .1*((double)x/ImageWidth-.5);
            ci += .1*((double)y/ImageHeight-.5);
            }
        }else{
            cr -= ci/100;
            ci += cr/100;
        }

        cout << cr << ", " << ci << endl;
        Display->plotMBSet(lol->getSet(cr, ci));
        Display->render();
        DisplayLoc->plotPoint(DisplayLoc->getSW()*(cr+1)/2, DisplayLoc->getSH()*(ci+1)/2);
        DisplayLoc->render();
    }
    return 0;
}




/*
        //cr -= ci/20;
        //ci += cr/20;
        theta+=.01;
        if(radius<2){
            radius+=.001;
        }else{radius-=.01;}
        cr = radius*cos(theta)*(cos(theta)*cos(theta)-sin(theta)*sin(theta));
        ci = radius*sin(theta)*(cos(theta)*cos(theta)-sin(theta)*sin(theta));
        cout << "radius: " << radius << ", theta: " << theta << "), " << endl;

*/
