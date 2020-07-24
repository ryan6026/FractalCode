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
#define ImageHeight ImageWidth //must be square (FFT2D, main)
#define maxit 100

using namespace std;

int main(int argc, char* args[]) {
    SDL_Point P;
        P.x = 5;
        P.y=25;
    GUI Display1(ImageHeight, ImageWidth, "Julia Set", P);
    MBset lol(ImageWidth, ImageHeight, maxit); int **tmp;
    double cr=-0.757814, ci= 0.207809;
    double cr0=1, ci0=0;
    int x, y;
    double s = 0.1;

    FFT2D pic(ImageWidth);
    FFT2D filter(ImageWidth);

    const Uint8 *KBstate;
    bool control = true;
    bool shiftZoom = false;
    while(true) {
        SDL_PumpEvents();
        KBstate = SDL_GetKeyboardState(NULL);
        //KEYS: wasd(move), fr(zoom), tg(blur), e(guide) q(big render)
        if (KBstate[SDL_SCANCODE_W]) {
            lol.ShiftDown();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_S]) {
            lol.ShiftUp();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_D]) {
            lol.ShiftRight();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_A]) {
            lol.ShiftLeft();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_F]) {
            lol.ZoomIn();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_R]) {
            lol.ZoomOut();
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_T]) {
            s+=0.01;
            cout << s << endl;
            shiftZoom = true;
        } else if (KBstate[SDL_SCANCODE_G]) {
            s-=0.01;
            cout << s << endl;
            shiftZoom = true;
        }  else if (KBstate[SDL_SCANCODE_Y]) {
            Display1.gradCoef*=0.8;
            cout << Display1.gradCoef << endl;
            Display1.plotMBSet(pic.getReal());
        } else if (KBstate[SDL_SCANCODE_H]) {
            Display1.gradCoef*=1.2;
            cout << Display1.gradCoef << endl;
            Display1.plotMBSet(pic.getReal());
        } else if(KBstate[SDL_SCANCODE_E]){
            control = !control;
        }
        /*
        else if(KBstate[SDL_SCANCODE_Q]){
            MBset bigSet(512, 512, maxit);
            GUI BigGUI(512, 512, "bigass_render");
            BigGUI.gradCoef = Display1.gradCoef;
            FFT2D big(512);
            big.setPic(bigSet.getSet(cr, ci));
            FFT2D filt(512);
            filt.setPic(getGaussFilter(s, 512));
            big.Filter(filter);
            BigGUI.plotMBSet(big.getReal());
            //Display->saveBMP(to_string(cr));
            //cout << "pic saved!" << endl;
        }
        */

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
            //cout << cr << ", " << ci << endl;
            cr0=cr; ci0=ci; shiftZoom=false;
            tmp = lol.getSet(cr, ci);
            //pic.setPic(tmp);
            //filter.setPic(getGaussFilter(s, ImageHeight));
            //pic.Transform(1);
            //pic.centerZero();
            //pic.Filter(filter);
            //pic.centerZero();
            //pic.Transform(-1);
            //Display1.plotMBSet(pic.getReal());
            Display1.plotMBSet(tmp);

        }
    }
    return 0;
}
