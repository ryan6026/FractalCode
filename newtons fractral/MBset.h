#include <cmath>
#include <iostream>
#include <complex>
#define PI 3.14159265359

using namespace std;

template <typename T>
T ***Alloc3DArray( int nRows, int nCols, int nDepth)
{
    T ***dynamicArray;
    dynamicArray = new T**[nRows];
    for( int i = 0 ; i < nRows ; i++ )
    {
        dynamicArray[i] = new T* [nCols];
        for(int u = 0; u<nCols; u++)
        {
            dynamicArray[i][u] = new T[nDepth];
        }
    }
    return dynamicArray;
}

template <typename T>
T **Alloc2DArray( int nRows, int nCols)
{
    T **dynamicArray;
    dynamicArray = new T*[nRows];
    for( int i = 0 ; i < nRows ; i++ )
        dynamicArray[i] = new T [nCols];
    return dynamicArray;
}

class MBset
{
private:
    double w=4, h, xmin, ymin, xmax, ymax, dx, dy,
           a, b, aa, bb, ab, aaa,
           bbb, abb, aab, atmp, btmp,
           cr, ci, maxiterations, y, x, xc=0, yc=0;;
    int ImageW, ImageH;// 40-50
    int*** n;

public:
    MBset(int ImageWidth, int ImageHeight, int maxIt)
    {
        maxiterations = maxIt;
        ImageW = ImageWidth;
        ImageH = ImageHeight;
        n = Alloc3DArray<int>(3, ImageW, ImageH);
        h = (w * ImageH) / ImageW;
        xmin = -w/2+xc;
        ymin = -h/2+yc;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
    };


    int*** Newton(complex<double> N) // make 3d array to color basins of attraction
    {
        float Tol = .025;
        float stp3 = sin(2*PI/3);
        complex<double> r1(1,               0);//z^3-1
        complex<double> r2(-0.5,  stp3);
        complex<double> r3(-0.5, -stp3);
        complex<double> const2(8, 0);
        complex<double> const1(3, 0);
        y = ymin;
        for(int i=0; i<ImageW; i++)
        {
            x = xmin;
            for (int j=0; j<ImageH; j++)
            {
                a = x;
                b = y;
                n[0][i][j]=0;
                n[1][i][j]=0;
                n[2][i][j]=0;
                int count=0;
                complex<double> z(x, y);
                while ((count < maxiterations) &&
                        (abs(z) > Tol))

                {


                    z -= (z*z*z-r1)/(N*z*z);
                    count++;



                }
                            if(abs(z-r1)<Tol)
                             n[0][i][j] = count;

                             if(abs(z-r2) <= Tol)
                             n[1][i][j] = count;

                             if(abs(z-r3) <= Tol)
                             n[2][i][j] = count;

                             x += dx;
            }
            y += dy;
        }
        return n;
    }

    void ZoomOut()
    {
        w*=1.2;
        cout << w << endl;
        h = (w * ImageH) / ImageW;
        xmin = -w/2 + xc;
        ymin = -h/2 + yc;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
    }
    void ZoomIn()
    {
        w*=.8;
        cout << w << endl;
        h = (w * ImageH) / ImageW;
        xmin = -w/2 + xc;
        ymin = -h/2 +  yc;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
    }
    void ShiftRight()
    {
        yc+=(ymax-ymin)/4;
        xmin = -w/2 + xc;
        ymin = -h/2 +  yc;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        cout << xc << ", " << yc << endl;
    }
    void ShiftLeft()
    {
        xmin = -w/2 + xc;
        ymin = -h/2 +  yc;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        yc-=(ymax-ymin)/4;
        cout << xc << ", " << yc << endl;
    }
    void ShiftDown()
    {
        xmin = -w/2 + xc;
        ymin = -h/2 +  yc;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        xc+=(xmax-xmin)/4;
        cout << xc << ", " << yc << endl;
    }
    void ShiftUp()
    {
        xmin = -w/2 + xc;
        ymin = -h/2 +  yc;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        xc-=(xmax-xmin)/4;
        cout << xc << ", " << yc << endl;
    }
    void AddIt()
    {
        maxiterations*=2;
    }
    void SubIt()
    {
        maxiterations*=.2;
    }
};
