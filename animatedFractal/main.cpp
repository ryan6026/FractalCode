//add hi res image save function
//gausian blur or like antialiasing filter for picture
#include "Display.h"
#include <SDL.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#define PI 3.14159265359
#define ImageW 500
#define ImageH 500
using namespace std;

void plotLines(vector<line> lines){

}

int main(int argc, char* args[]) {
    SDL_Point P;
        P.x = 300;
        P.y=0;
    GUI *Display;
        Display = new GUI(ImageH, ImageW, "fractal", P);
        Display->setGradCoef(1);

    double x0 = ImageW/2,
           y0 = ImageH/2,
           x1=0, y1=0, x2, y2, t=0;
    vector<line> lines;

    Display->plotAxes();
    while((x1<ImageW)&(y1<ImageH)){
        x1 = x0 + x0*sin(t)*cos(13*t);
        y1 = y0 + y0*sin(14*t)*sin(t);
        x2 = x0 + x0*cos(5*t)*cos(4*t);
        y2 = y0 + y0*sin(3*t)*cos(8*t);
        lines.push_back(line(x1, y1, x2, y2));
        if(t>.15){
            lines.erase(lines.begin());
        }
        Display->clear();
        Display->plotAxes();
        for(unsigned int i=0;i<lines.size();i++){
            Display->plotLine(lines.at(i), (int)(t*1000)%255);
        }

        Display->render();
        t+=.001;

    }

    cin.get();

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
