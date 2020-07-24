#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#define PI 3.14159265359
using namespace std;

class GUI {
  private:
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Renderer* renderer = NULL;
    int R, G, B;
    double var;
    SDL_Point pos;
    int SCREEN_HEIGHT, SCREEN_WIDTH;
    double gradCoef = 0;
  public:
    GUI(int SH, int SW, const char* name) { //SH:screen height, SW:Screen Width, WC:window count
        SCREEN_HEIGHT = SH;
        SCREEN_WIDTH = SW;
        pos.y = 30;
        pos.x = 10; //default window position
        cout << "creating " << SCREEN_WIDTH << " by " << SCREEN_HEIGHT << " window" << endl;
        window = SDL_CreateWindow(name, pos.x, pos.y, SW, SH, SDL_WINDOW_SHOWN);
        renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
        screenSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
    };

    GUI(int SH, int SW, const char* name, SDL_Point p) { //SH:screen height, SW:Screen Width, WC:window count

        SCREEN_HEIGHT = SH;
        SCREEN_WIDTH = SW;
        pos.x = p.x;
        pos.y = p.y;
        cout << "creating " << SCREEN_WIDTH << " by " << SCREEN_HEIGHT << " window" << endl;
        window = SDL_CreateWindow(name, pos.x, pos.y, SW, SH, SDL_WINDOW_SHOWN);
        renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
        screenSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
        SDL_SetSurfaceRLE(screenSurface, 1);
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
    };

    int getMax(int** n){
        int max=0;
        for(int x=0;x< SCREEN_WIDTH;x++){
            for(int y=0;y<SCREEN_HEIGHT;y++){
                if(max<n[x][y]) max = n[x][y];
            }
        }
        return max;
    }

    void setGradCoef(double g) {
        gradCoef=g;
    }

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

    void plotAxes() {
        SDL_SetRenderDrawColor( renderer, 150, 150, 150, 255 );
        SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2);
    }

    void plotPoint(int x, int y){
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderDrawPoint(renderer, x, y);
        SDL_RenderPresent(renderer);
    }


    void plotMBSet(int** n){
        for(int x=0;x<SCREEN_WIDTH;x++){
            for(int y=0;y<SCREEN_HEIGHT;y++){
                if(n[x][y]>0){
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                }
                else if(n[x][y]==0){
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
        SDL_RenderPresent(renderer);
    }


};

