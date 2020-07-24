#include "MBset.h"
#include "Display.h"
#include <SDL2\SDL.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <random>
#include <string>
#include "FFT2D.h"
#define PI 3.14159265359
#define ImageWidth 256
#define ImageHeight ImageWidth
#define maxit 30

using namespace std;

int main(int argc, char* args[]) {
    SDL_Point P;
        P.x = 5;
        P.y=25;
    GUI *Display;
        Display = new GUI(ImageHeight, ImageWidth, "Julia Set", P);
        Display->setGradCoef(1);
    GUI *DisplayLoc;
        DisplayLoc = new GUI(150, 150, "C Location", Display->getTopRightCorner());
        DisplayLoc->plotAxes();
    GUI *FFTDisp;
        FFTDisp = new GUI(ImageHeight, ImageWidth, "FFT", DisplayLoc->getTopRightCorner());
    //double cr=-0.779167, ci=  -0.139667;
    double cr=0, ci=0;
    double cr0=1, ci0=0;
    int x, y;
    MBset* lol = new MBset(ImageWidth, ImageHeight, maxit); int **tmp;
    FFT2D fft(ImageWidth);
    const Uint8 *KBstate;
    bool control = true;
    bool shiftZoom = false;
    while(true) {
        SDL_PumpEvents();
        KBstate = SDL_GetKeyboardState(NULL);
        if (KBstate[SDL_SCANCODE_W]) {
            lol->ShiftDown();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_S]) {
            lol->ShiftUp();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_D]) {
            lol->ShiftRight();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_A]) {
            lol->ShiftLeft();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_F]) {
            lol->ZoomIn();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_R]) {
            lol->ZoomOut();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_T]) {
            Display->setGradCoef(Display->gradCoef*1.2);
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_G]) {
            Display->setGradCoef(Display->gradCoef*0.8);
            shiftZoom = true;
        } else if(KBstate[SDL_SCANCODE_E]){
            control = !control;
        }
        /*else if(KBstate[SDL_SCANCODE_Q]){
            Display->saveBMP(to_string(cr));
            cout << "pic saved!" << endl;
        }*/

        if(SDL_GetMouseState(&x, &y)&&SDL_BUTTON((SDL_BUTTON_LEFT))){
            cr += .1*((double)x/ImageWidth-.5);
            ci += .1*((double)y/ImageHeight-.5);
            if(abs(cr)>5)
                cr=cr0;
            if(abs(ci)>5)
                ci=ci0;
        }

        if(!control){
            cr -= ci/100;
            ci += cr/100;
        }

        if((cr0!=cr)||shiftZoom){
            cout << cr << ", " << ci << endl;
            cr0=cr; ci0=ci; shiftZoom=false;
            tmp = lol->getSet(cr, ci);
            Display->plotMBSetBW(tmp);
            DisplayLoc->plotPoint(DisplayLoc->getSW()*(cr+1)/2, DisplayLoc->getSH()*(ci+1)/2);
            fft.setPic(tmp);
            fft.Transform(1);
            //fft.Transform(-1);
            FFTDisp->plotMBSetBW(fft.getReal());
        }
    }
    return 0;
}
