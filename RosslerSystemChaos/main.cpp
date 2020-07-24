#include "Display.h"
#include <SDL.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include "AllocArray.h"
#define PI 3.14159265359
#define ImageW 500
#define ImageH ImageW
#define MAX_POINTS 100000
using namespace std;

template <typename T>
void GraphXYPoints(T** SET, GUI Display) {// converts from unit coordinates to renderer coordinates:
                                          //(linear scale from ([minx, maxx], [miny, maxy]) -> ([0, ImageW], [0, ImageH])
    T maxx=0, maxy=0,
      minx=0, miny=0;
    for(int i=0; i<MAX_POINTS; i++) {
        if(maxx<SET[i][0])
            maxx = SET[i][0];
        if(maxy<SET[i][1])
            maxy = SET[i][1];
        if(minx>SET[i][0])
            minx = SET[i][0];
        if(miny>SET[i][1])
            miny = SET[i][1];
    }
    for(int i=0; i<MAX_POINTS; i++) {
        SET[i][0] = ImageW*(SET[i][0]-minx)/(maxx-minx);
        SET[i][1] = ImageH*(SET[i][1]-miny)/(maxy-miny);
    }
    Display.plotSet(SET ,MAX_POINTS);
}

double** getSet(double x0, double y0, double a) {
    double dt = .001;
    double** points;
    points = Alloc2DArray<double>(MAX_POINTS, 2); //[Point][x=0, y=1]
    points[0][0] = x0;
    points[0][1] = y0;
    for(int p=0; p<MAX_POINTS-1; p++) {
        points[p+1][0] = points[p][0] - dt * (points[p][1]);                    //dx = -dt*y
        points[p+1][1] = points[p][1] + dt * (points[p][0] + a * points[p][1]); //dy = dt*(x+a*y)
    }
    return points;
}

int main(int argc, char* args[]) {
    SDL_Point P;
    P.x = 500;
    P.y = 25;
    GUI Display(ImageH, ImageW, "graph", P);
    Display.clearRender();
    //int** field;
    //field = Alloc2DArray<int>(ImageW, ImageH);
    double a = -2;
    while(a<1){
        a+=.1;
        Display.clearRender();
        GraphXYPoints(getSet(1, 1, a), Display);
        Display.render();
        //Display.delay(50);
    }

    cout << "done" << endl;
    cin.get();
    return 0;
}
