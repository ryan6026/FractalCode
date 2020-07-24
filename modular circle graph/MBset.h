#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <complex>

using namespace std;

template <typename T>
T **Alloc2DArray( int nRows, int nCols) {
    T **dynamicArray;

    dynamicArray = new T*[nRows];
    for( int i = 0 ; i < nRows ; i++ )
        dynamicArray[i] = new T [nCols];

    return dynamicArray;
}

template <typename T>
T ***Alloc3DArray( int nRows, int nCols, int nDepth) {
    T ***dynamicArray;
    dynamicArray = new T**[nRows];
    for( int i = 0 ; i < nRows ; i++ ) {
        dynamicArray[i] = new T* [nCols];
        for(int u = 0; u<nCols; u++) {
            dynamicArray[i][u] = new T[nDepth];
        }
    }
    return dynamicArray;
}

class MBset {
  private:
    GUI* Display;
    double MinRe = -2.0;
    double MaxRe = 1.0;
    double MinIm = -1.2;
    double MaxIm;
    double Re_factor, Im_factor;
    double c_im, c_re, Z_re;
    unsigned maxiterations = 50; //default iterations
    bool isInside;
    double Z_re2, Z_im, Z_im2;
    int x, y, n;

  public:
    MBset(int IH, int IW) {
        Display = new GUI(IH, IW, "Mandlebrot set");
        MaxIm = MinIm+(MaxRe-MinRe)*Display->getSH()/Display->getSW();
        Re_factor = (MaxRe-MinRe)/(Display->getSW()-1);
        Im_factor = (MaxIm-MinIm)/(Display->getSH()-1);
    };

    MBset(int IH, int IW, int it) { //iterations overload
        maxiterations = it;
        Display = new GUI(IH, IW, "Mandlebrot set");
        MaxIm = MinIm+(MaxRe-MinRe)*Display->getSH()/Display->getSW();
        Re_factor = (MaxRe-MinRe)/(Display->getSW()-1);
        Im_factor = (MaxIm-MinIm)/(Display->getSH()-1);
    };

    void graphSet() {
        for(unsigned y=0; y<Display->getSH(); ++y) {
            c_im = MaxIm - y*Im_factor;
            for(unsigned x=0; x<Display->getSW(); ++x) {
                c_re = MinRe + x*Re_factor;
                Z_re = c_re;
                Z_im = c_im;
                isInside = true;
                for(n=0; n<maxiterations; ++n) {
                    Z_re2 = Z_re*Z_re;
                    Z_im2 = Z_im*Z_im;
                    if(Z_re2 + Z_im2 > 4) {
                        isInside = false;
                        break;
                    }
                    Z_im = 2*Z_re*Z_im + c_im;
                    Z_re = Z_re2 - Z_im2 + c_re;
                }
                if(isInside) {
                    Display->plotPoint(x, y, n);
                }
            }
        }
        Display->render();
    }
};
/*        for(unsigned y=0; y<Display->getSH(); ++y)
        {
            double c_im = MaxIm - y*Im_factor;
            for(unsigned x=0; x<Display->getSW(); ++x)
            {
                double c_re = MinRe + x*Re_factor;

                double Z_re = c_re, Z_im = c_im;
                bool isInside = true;
                for(n=0; n<MaxIterations; ++n)
                {
                    double Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
                    if(Z_re2 + Z_im2 > 4)
                    {
                        //isInside = false;
                        break;
                    }
                    Z_im = 2*Z_re*Z_im + c_im;
                    Z_re = Z_re2 - Z_im2 + c_re;
                }
                if(isInside)
                {
                */


