#include "MBset.h"
#include "Display.h"
#include <SDL.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <random>
#define PI 3.14159265359
#define ImageWidth 100
#define ImageHeight 100
using namespace std;


int main(int argc, char* args[]) {
    SDL_Point P;
        P.x = 300;
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
    int x, y;

    MBset* lol = new MBset(ImageWidth, ImageHeight, 255);
        double cr=.059315, ci=.767697, crp, cip; //0.059315, 0.767697

    while(true) {
        SDL_PumpEvents();
        KBstate = SDL_GetKeyboardState(NULL);
        if (KBstate[SDL_SCANCODE_KP_8]) {
            lol->ShiftUp();
            Display->plotMBSet(lol->getSet(cr, ci));
            Display->render();
        } else if (KBstate[SDL_SCANCODE_KP_2]) {
            lol->ShiftDown();
            Display->plotMBSet(lol->getSet(cr, ci));
            Display->render();
        } else if (KBstate[SDL_SCANCODE_KP_6]) {
            lol->ShiftRight();
            Display->plotMBSet(lol->getSet(cr, ci));
            Display->render();
        } else if (KBstate[SDL_SCANCODE_KP_4]) {
            lol->ShiftLeft();
            Display->plotMBSet(lol->getSet(cr, ci));
            Display->render();
        } else if (KBstate[SDL_SCANCODE_KP_9]) {
            lol->ZoomIn();
            Display->plotMBSet(lol->getSet(cr, ci));
            Display->render();
        } else if (KBstate[SDL_SCANCODE_KP_7]) {
            lol->ZoomOut();
            Display->plotMBSet(lol->getSet(cr, ci));
            Display->render();
        } else if(KBstate[SDL_SCANCODE_KP_5]){
            control = !control;
            cout << "control: " << control << endl;
        }
        if(control&&SDL_GetMouseState(&x, &y)&SDL_BUTTON((SDL_BUTTON_LEFT))){
                cr += .001*((double)x/ImageWidth-.5);
                ci += .001*((double)y/ImageHeight-.5);
        }
        else if(!control){
            cr -= ci/100;
            ci += cr/100;
        }
        if(cr!=crp||ci!=cip){
            cout << cr << ", " << ci << endl;
            Display->plotMBSet(lol->getSet(cr, ci));
            Display->render();
            DisplayLoc->plotPoint(DisplayLoc->getSW()*(cr+1)/2, DisplayLoc->getSH()*(ci+1)/2);
            DisplayLoc->render();
            crp = cr; cip = ci;
        }
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
