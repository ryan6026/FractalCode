#include "MBset.h"
#include "Display.h"
#include <SDL.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <random>
#define PI 3.14159265359
#define ImageWidth 1000
#define ImageHeight 1000
using namespace std;

int main(int argc, char* args[]) {
    SDL_Point P;
        P.x = 200;
        P.y=0;
    GUI *Display;
        Display = new GUI(ImageHeight, ImageWidth, "Julia Set", P);
        Display->setGradCoef(1);
    GUI *DisplayLoc; // color points with render time
        DisplayLoc = new GUI(150, 150, "C Location", Display->getTopRightCorner());
        DisplayLoc->plotAxes();
        DisplayLoc->render();
    const Uint8 *KBstate;
    bool control = true;

    MBset* lol = new MBset(ImageWidth, ImageHeight, 1000);
        double crci[] = {
0.277625, -0.006975
    };
    double cr = crci[0], ci = crci[1];
    double crp, cip;
        int x, y;

    while(true) {
        SDL_PumpEvents();
        KBstate = SDL_GetKeyboardState(NULL);
        if (KBstate[SDL_SCANCODE_KP_8]) {
            lol->ShiftUp();
        } else if (KBstate[SDL_SCANCODE_KP_2]) {
            lol->ShiftDown();
        } else if (KBstate[SDL_SCANCODE_KP_6]) {
            lol->ShiftRight();
        } else if (KBstate[SDL_SCANCODE_KP_4]) {
            lol->ShiftLeft();
        } else if (KBstate[SDL_SCANCODE_KP_9]) {
            lol->ZoomIn();
        } else if (KBstate[SDL_SCANCODE_KP_7]) {
            lol->ZoomOut();
        } else if(KBstate[SDL_SCANCODE_KP_5]){
            control = !control;
        }
        if(control&&SDL_GetMouseState(&x, &y)){
            if(SDL_BUTTON((SDL_BUTTON_LEFT))){
                cr += .01*((double)x/ImageWidth-.5);
                ci += .01*((double)y/ImageHeight-.5);
            }
            else if(SDL_BUTTON(SDL_BUTTON_RIGHT)){
                lol->addW(((double)y/ImageHeight-.5)*((double)y/ImageHeight-.5) + ((double)x/ImageWidth-.5)*((double)x/ImageWidth-.5));
            }
        }
        else if(!control){
            cr -= ci/1000;
            ci += cr/1000;
        }
//        if(cr!=crp||ci!=cip){
            cout << cr << ", " << ci << endl;
            Display->plotMBSet(lol->getSet(cr, ci));
            Display->render();
            DisplayLoc->plotPoint(DisplayLoc->getSW()*(cr+1)/2, DisplayLoc->getSH()*(ci+1)/2);
            DisplayLoc->render();
            crp = cr; cip = ci;
        //}
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
