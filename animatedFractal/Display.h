#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <sstream>
#include "AllocArray.h"
#define PI 3.14159265359
//working on data fitting graphs
using namespace std;

class GUI {
  private:
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int R, G, B;
    double var;
    SDL_Point pos;
    int ImageH, ImageW;
    double gradCoef = 2;
  public:
    GUI(int SH, int SW, const char* name) { //SH:screen height, SW:Screen Width, WC:window count
        ImageH = SH;
        ImageW = SW;
        pos.y = 30;
        pos.x = 10; //default window position
        //cout << "creating " << ImageW << " by " << ImageH << " window" << endl;
        window = SDL_CreateWindow(name, pos.x, pos.y, SW, SH, SDL_WINDOW_SHOWN);
        renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
    };

    GUI(int SH, int SW, const char* name, SDL_Point p) { //SH:screen height, SW:Screen Width, WC:window count
        ImageH = SH;
        ImageW = SW;
        pos.x = p.x;
        pos.y = p.y;
        //cout << "creating " << ImageW << " by " << ImageH << " window" << endl;
        window = SDL_CreateWindow(name, pos.x, pos.y, SW, SH, SDL_WINDOW_SHOWN);
        renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
    };

    template <typename T>
    T getMax(T** n) {
        int max=0;
        for(int x=0; x< ImageW; x++) {
            for(int y=0; y<ImageH; y++) {
                if(max<n[x][y]) max = n[x][y];
            }
        }
        return max;
    }

    void setGradCoef(double g) {
        gradCoef=g;
    }
    int getSH() {
        return ImageH;
    }
    int getSW() {
        return ImageW;
    }
    SDL_Point getBottomLeftCorner() {
        SDL_Point g;
        g.x = pos.x;
        g.y = pos.y + ImageH + 30;
        return g;
    }
    SDL_Point getTopRightCorner() {
        SDL_Point g;
        g.x = pos.x + ImageW + 15;
        g.y = pos.y;
        return g;
    }
    void plotAxes() {
        SDL_SetRenderDrawColor( renderer, 150, 150, 150, 255 );
        SDL_RenderDrawLine(renderer, ImageW/2, 0, ImageW/2, ImageH);
        SDL_RenderDrawLine(renderer, 0, ImageH/2, ImageW, ImageH/2);
    }
    void delay(int a) {
        SDL_Delay(a);
    }
    void render() {
        SDL_RenderPresent(renderer);
    }

    void plotPoint(int x, int y, int c) {
        SDL_SetRenderDrawColor(renderer, c, c, 255-c, 255);
        SDL_RenderDrawPoint(renderer, x, y);
    }

    void plotSet(double **n){
        double MAX = getMax(n);
        for(int i=0;i<ImageW;i++){
            for(int j=0;j<ImageH;j++){
                plotPoint(i, j, n[i][j]*255/MAX);

            }
        }
    }

    void clear(){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for(int x=0;x<ImageW;x++){
            SDL_RenderDrawLine(renderer, x, 0, x, ImageH);
        }
    }

    void plotLine(line a, int c){
        SDL_SetRenderDrawColor(renderer, c, 255-c, 255, 255);
        SDL_RenderDrawLine(renderer, a.x1, a.y1, a.x2, a.y2);
    }
    void save(double cr, double ci) {
        stringstream strs;
        strs << cr*10;
        strs << ci*10;
        strs << ".bmp";
        const char* str = strs.str().c_str();
        SDL_Surface *sshot = SDL_CreateRGBSurface(0, ImageW, ImageH, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
        SDL_SaveBMP(sshot, str);
        SDL_FreeSurface(sshot);
        delay(300);
    }
};

