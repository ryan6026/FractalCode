#include "Display.h"
#include <SDL.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <vector>

#define PI 3.14159265359
#define ImageW 1400
#define ImageH 1000

using namespace std;

int main(int argc, char* args[]) {
    SDL_Point p;
    p.x = 400; p.y = 0;
    GUI *Display;
    Display = new GUI(ImageH, ImageW, "circle mod", p);
    Display->clearRender();

    for(int i=1;i<1000;i++){
        Display->plotModN(1000, i);
    }
    cout << "press enter to exit" << endl;
    cin.get();
    return 0;
}
