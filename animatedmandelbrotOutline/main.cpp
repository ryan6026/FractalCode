#include "MBset.h"
#include "Display.h"
#include <SDL.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include <vector>
#include <algorithm>
#include "vertex.h"
#define PI 3.14159265359
#define ImageWidth 150
#define ImageHeight ImageWidth //MUST BE SQUARE!!!

using namespace std;

int main(int argc, char* args[]) {

    SDL_Point P;
    P.x = 200;
    P.y = 100;
    GUI *Display;
    Display = new GUI(ImageHeight, ImageWidth, "Julia Set", P);
    Display->setGradCoef(1);
    GUI *DisplayLoc; // color points with render time
    DisplayLoc = new GUI(ImageHeight, ImageWidth, "C Location", Display->getTopRightCorner());
    DisplayLoc->plotAxes();

    const Uint8 *KBstate;
    bool control = true;
    double crci[] = {
        0,0
    };
    double cr = crci[0], ci = crci[1];
    double t = 0, dt = .035, tMAX = PI;
    MBset* lol = new MBset(ImageWidth, ImageHeight, 4);
    int** MBtmp;
    vector<vertex> ptsTmp, ptsTmp2;
    int ptSum=0;
    ofstream out;
    out.open("try1.stl");
    out << "solid fractal" << endl;

    while(t<tMAX) {
        SDL_PumpEvents();
        KBstate = SDL_GetKeyboardState(NULL);
        if (KBstate[SDL_SCANCODE_KP_8]) {
            lol->ShiftUp();
            Display->plotMBSet(lol->getSet(cr, ci));
        } else if (KBstate[SDL_SCANCODE_KP_2]) {
            lol->ShiftDown();
            Display->plotMBSet(lol->getSet(cr, ci));
        } else if (KBstate[SDL_SCANCODE_KP_6]) {
            lol->ShiftRight();
            Display->plotMBSet(lol->getSet(cr, ci));
        } else if (KBstate[SDL_SCANCODE_KP_4]) {
            lol->ShiftLeft();
            Display->plotMBSet(lol->getSet(cr, ci));
        } else if (KBstate[SDL_SCANCODE_KP_9]) {
            lol->ZoomIn();
            Display->plotMBSet(lol->getSet(cr, ci));
        } else if (KBstate[SDL_SCANCODE_KP_7]) {
            lol->ZoomOut();
            Display->plotMBSet(lol->getSet(cr, ci));
        } else if(KBstate[SDL_SCANCODE_KP_5]) {
            control = !control;
        }
        cr = .6*sin(3*t);
        ci = .6*sin(4*t);
        cout << cr << ", " << ci <<" T: " << t << endl;
        ptsTmp2=ptsTmp;
        MBtmp = lol->getDel(lol->getSet(cr, ci), cr, ci);
        ptsTmp = lol->getPoints(MBtmp, t);
        ptSum+=ptsTmp.size();
        sort(ptsTmp.begin(), ptsTmp.end());
        Display->plotMBSet(MBtmp);
        DisplayLoc->plotPoint(DisplayLoc->getSW()*(cr+1)/2, DisplayLoc->getSH()*(ci+1)/2);
        for(int i=1; i<min(ptsTmp.size(), ptsTmp2.size()); i++) {

            out << "facet normal 0 0 0" << endl << "outer loop" << endl;
            out <<"vertex " << ptsTmp.at(i).x  << " " << ptsTmp.at(i).y << " " << ptsTmp.at(i).z << endl;
            out <<"vertex " << ptsTmp2.at(i).x  << " " << ptsTmp2.at(i).y << " " << ptsTmp2.at(i).z << endl;
            out <<"vertex " << ptsTmp.at(i-1).x  << " " << ptsTmp.at(i-1).y << " " << ptsTmp.at(i-1).z << endl;
            out << "endloop" << endl << "endfacet" << endl;

            out << "facet normal 0 0 0" << endl << "outer loop" << endl;
            out <<"vertex " << ptsTmp.at(i-1).x  << " " << ptsTmp.at(i-1).y << " " << ptsTmp.at(i-1).z << endl;
            out <<"vertex " << ptsTmp2.at(i-1).x  << " " << ptsTmp2.at(i-1).y << " " << ptsTmp2.at(i-1).z << endl;
            out <<"vertex " << ptsTmp2.at(i).x  << " " << ptsTmp2.at(i).y << " " << ptsTmp2.at(i).z << endl;
            out << "endloop" << endl << "endfacet" << endl;

        }
        t+=dt;
    }
    cout << "number of points: " << ptSum << endl;

    out << "endsolid fractal";
    out.close();
    return 0;
}
