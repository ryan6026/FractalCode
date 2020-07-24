#include <SDL.h>
//#include "SDL_g
#include <stdio.h>
#include <cmath>
#include <vector>
#include <iostream>
#define PI 3.14159265359

using namespace std;

class GUI {
  private:
    vector<SDL_Point> points;
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Renderer* renderer = NULL;
    int R, G, B;
    double var;
    SDL_Point pos;
    int SCREEN_HEIGHT, SCREEN_WIDTH;

  public:
    GUI(int SH, int SW, const char* name) { //SH:screen height, SW:Screen Width, WC:window count
        SCREEN_HEIGHT = SH;
        SCREEN_WIDTH = SW;
        pos.y = 30; //default window position
        pos.x = 10;
        cout << "creating " << SCREEN_WIDTH << " by " << SCREEN_HEIGHT << " window" << endl;
        window = SDL_CreateWindow(name, pos.x, pos.y, SW, SH, SDL_WINDOW_SHOWN);
        renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
    };

    GUI(int SH, int SW, const char* name, SDL_Point posInit) { //SH:screen height, SW:Screen Width, WC:window count
        SCREEN_HEIGHT = SH;
        SCREEN_WIDTH = SW;
        pos = posInit;
        cout << "creating " << SCREEN_WIDTH << " by " << SCREEN_HEIGHT << " window" << endl;
        window = SDL_CreateWindow(name, pos.x, pos.y, SW, SH, SDL_WINDOW_SHOWN);
        renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
    };

    int getSH() {
        return SCREEN_HEIGHT;
    }
    int getSW() {
        return SCREEN_WIDTH;
    }

    SDL_Point getBottomLeftCorner() {
        SDL_Point g;
        g.x = pos.x;
        g.y = pos.y + SCREEN_HEIGHT + 30;
        return g;
    }

    SDL_Point getTopRightCorner() {
        SDL_Point g;
        g.x = pos.x + SCREEN_WIDTH + 15;
        g.y = pos.y;
        return g;
    }

    void delay(int a) {
        SDL_Delay(a);
    }

    void plotLine(SDL_Point a, SDL_Point b) {
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
        SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
    }

    void plotPoint(int x, int y, int n) {
        SDL_Rect r;
        r.x = x;
        r.y = y;
        r.w = 1;
        r.h = 1;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &r);
    }

    void plotModN(int Npoints, int mod) {

        SDL_Point p;
        float x = SCREEN_WIDTH/2, //mess with these if the picture is cut off...
              y = SCREEN_HEIGHT/2,
              theta=0;
              int it; //r=1
        for(it=0; it<Npoints; it++) {
            x+=SCREEN_WIDTH*cos(theta)/2;
            y+=SCREEN_HEIGHT*sin(theta)/2;
            p.x = x;
            p.y = y;
            points.push_back(p);
            x-=SCREEN_WIDTH*cos(theta)/2;
            y-=SCREEN_HEIGHT*sin(theta)/2;
            theta+=2*PI/Npoints;
        }
        //polygon
        plotLine(points.at(0), points.at(Npoints-1));
        for(it=1; it<Npoints; it++) {
            plotLine(points.at(it-1), points.at(it));
        }
        //lines
        for(int n=0; n<points.size(); n++) {
            plotLine(points.at(n), points.at((n*mod)%Npoints));
        }
        SDL_RenderPresent(renderer);
        clearRender();
    }

    void clearRender() {
        for(int a=0;a<SCREEN_WIDTH;a++){
            for(int b=0;b<SCREEN_HEIGHT;b++){
                plotPoint(a, b, 0);
            }
        }
    }

    void render() {
        SDL_RenderPresent(renderer);
    }
};
