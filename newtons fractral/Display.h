#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <sstream>
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

    void delay(int a) {
        SDL_Delay(a);
    }

    void plotPoint(int x, int y){
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderDrawPoint(renderer, x, y);
    }

    void plotPoint(int x, int y, int r, int b, int g){
        SDL_SetRenderDrawColor(renderer, r, b, g, 255);
        SDL_RenderDrawPoint(renderer, x, y);
    }

    void Plot3MBPointHist(int x, int y, int* nrgb) { //nrgb[i+1] = maxN ||| nrgb[i] = N for i<6 (only three color channels

        SDL_SetRenderDrawColor(renderer, 255-255*nrgb[0]/(nrgb[1]+1), 255-255*nrgb[2]/(nrgb[3]+1), 255-255*nrgb[4]/(nrgb[5]+1), 255);
        SDL_RenderDrawPoint(renderer, x, y);
    }
    void plot3MBsetHist(int*** n) {

        int* nrgb;
        nrgb = new int[6];
        for(int c=0; c<3; c++) {
            nrgb[2*c+1] = getMax(n[c]);
        }
        for(int i=0; i<SCREEN_WIDTH; i++) {
            for(int j=0; j<SCREEN_HEIGHT; j++) {
                for(int c=0; c<3; c++) {
                    nrgb[c*2] = n[c][i][j];
                    Plot3MBPointHist(i, j, nrgb);
                }
            }
        }
    }

    void plotMBPoint(int x, int y, int n, int maxN) {
        n = (int)maxN*pow((double)((double)n/maxN), gradCoef);//*exp(((double)n-maxN)/maxN);

        if(n<maxN/7) {
            SDL_SetRenderDrawColor(renderer, 255*sin(PI*n*7/(maxN*2))*sin(PI*n*7/(maxN*2)), 255-255*tan(PI*n*7/(maxN*2))*tan(PI*n*7/(maxN*2)), 255*cos(PI*n*7/(maxN*2))*cos(PI*n*7/(maxN*2)), 255);
        } else if(n>2*maxN/7&&n<3*maxN/7) {
            SDL_SetRenderDrawColor(renderer, 255*sin(PI*n*7/(maxN*6))*sin(PI*n*7/(maxN*6)), 0, 255*sin(PI*n*7/(maxN*6))*sin(PI*n*7/(maxN*6)), 255);
        } else if(n>3*maxN/7&&n<4*maxN/7) {
            SDL_SetRenderDrawColor(renderer, 255*cos(PI*n*7/(maxN*8))*sin(PI*n*7/(maxN*8)), 255*sin(PI*n*7/(maxN*8))*sin(PI*n*7/(maxN*8)), 0, 255);
        } else if(n>4*maxN/7&&n<5*maxN/7) {
            SDL_SetRenderDrawColor(renderer, 255*cos(PI*n*7/(maxN*2)), 255*sin(PI*n*7/(maxN*10))*sin(PI*n*7/(maxN*10)), 255*sin(PI*n*7/(maxN*10))*sin(PI*n*7/(maxN*10)), 255);
        } else if(n>5*maxN/7&&n<6*maxN/7) {
            SDL_SetRenderDrawColor(renderer, 255*sin(PI*n*7/(maxN*12))*sin(PI*n*7/(maxN*12)), 0, 255*sin(PI*n*7/(maxN*12))*sin(PI*n*7/(maxN*12)), 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 255-255*sin(PI*n/(maxN*2))*sin(PI*n/(maxN*2)), 0, 0, 255);
        }
        SDL_RenderDrawPoint(renderer, x, y);
    }

    void plotMBSet(int** n){
        int MAX = getMax(n);
        for(int x=0;x<SCREEN_WIDTH;x++){
            for(int y=0;y<SCREEN_HEIGHT;y++){
                plotMBPoint(x, y, n[x][y], MAX);
            }
        }
    }

    void render() {
        SDL_RenderPresent(renderer);
    }
    void save(double cr, double ci) {
        stringstream strs;
        strs << (cr*10);
        strs << ci*10;
        strs << ".bmp";
        const char* str = strs.str().c_str();
        SDL_Surface *sshot = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
        SDL_SaveBMP(sshot, str);
        SDL_FreeSurface(sshot);
        delay(300);
    }
};

