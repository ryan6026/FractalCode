//#include <cmath>
#include <iostream>
#define SSample 0
#define space 1
using namespace std;

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
    //.1999, .5379
    double cr, ci, w=3.4, h, xmin, ymin, xmax, ymax, dx, dy, y, x, a, b, aa, bb, ab, aaa, bbb, abb, aab, atmp, btmp;
    int maxiterations, ImageW, ImageH;// 40-50

public:
    int** n;
    MBset(int ImageWidth, int ImageHeight, int maxIt)
    {
        maxiterations = maxIt;
        ImageW = ImageWidth;
        ImageH = ImageHeight;

        h = (w * ImageH) / ImageW;
        xmin = -w/2;
        ymin = -h/2;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        n = Alloc2DArray<int>(ImageW, ImageH);
    };


    int supersample(double xc, double yc, double cr, double ci)
    {
        int n[5];
        a=xc;
        b=yc;
        n[0]=0;
        while (n[0] < maxiterations)
        {
            aa = a*a;
            bb = b*b;
            ab = a*b;
            a = aa-bb+cr;
            b = 2*ab+ci;
            if((a*a+b*b)>4)
            {
                break;
            }
            n[0]++;
        }
        n[1]=0;
        a=xc+(dx/space);
        b=yc+(dy/space);
        while (n[1] < maxiterations)
        {
            aa = a*a;
            bb = b*b;
            ab = a*b;
            a = aa-bb+cr;
            b = 2*ab+ci;
            if((a*a+b*b)>4)
            {
                break;
            }
            n[1]++;
        }
        n[2]=0;
        a=xc-(dx/space);
        b=yc+(dy/space);
        while (n[2] < maxiterations)
        {
            aa = a*a;
            bb = b*b;
            ab = a*b;
            a = aa-bb+cr;
            b = 2*ab+ci;
            if((a*a+b*b)>4)
            {
                break;
            }
            n[2]++;
        }
        n[3]=0;
        a=xc-(dx/space);
        b=yc-(dy/space);
        while (n[3] < maxiterations)
        {
            aa = a*a;
            bb = b*b;
            ab = a*b;
            a = aa-bb+cr;
            b = 2*ab+ci;
            if((a*a+b*b)>4)
            {
                break;
            }
            n[3]++;
        }
        n[4]=0;
        a=xc+(dx/space);
        b=yc-(dy/space);
        while (n[4] < maxiterations)
        {
            aa = a*a;
            bb = b*b;

            ab = a*b;
            a = aa-bb+cr;
            b = 2*ab+ci;
            if((a*a+b*b)>4)
            {
                break;
            }
            n[4]++;
        }
        return (n[0]+n[1]+n[2]+n[3]+n[4])/5;
    }

    int** getSet(double Cr, double Ci)
    {
        cr = Cr;
        ci = Ci;

        y = ymin;
        for (int j = 0; j < ImageW; j++)
        {
            x = xmin;
            for (int i = 0; i < ImageH; i++)
            {
                if(SSample)
                {
                    n[i][j] = supersample(x, y, cr, ci);
                }
                else
                {
                    a=x;
                    b=y;
                    n[i][j]=0;
                    while (n[i][j] < maxiterations)
                    {
                        aa = a*a;
                        bb = b*b;
                        ab = a*b;
                        a = aa-bb+cr;
                        b = 2*ab+ci;
                        if((a*a+b*b)>4)
                        {
                            break;
                        }
                        n[i][j]++;
                    }
                }
                x += dx;
            }
            y += dy;
        }
        return n;
    }

    void ZoomOut()
    {
        w*=1.2;
        h = w * ImageH / ImageW;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        cout << "W: " << w << endl;
    }
    void ZoomIn()
    {
        w*=.8;
        h = w * ImageH / ImageW;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        cout << "W: " << w << endl;
    }
    void ShiftUp()
    {
        ymin+=(ymax-ymin)/4;
        ymax+=(ymax-ymin)/4;
    }
    void ShiftDown()
    {
        ymin-=(ymax-ymin)/4;
        ymax-=(ymax-ymin)/4;
    }
    void ShiftRight()
    {
        xmin+=(xmax-xmin)/4;
        xmax+=(xmax-xmin)/4;
    }
    void ShiftLeft()
    {
        xmin-=(xmax-xmin)/4;
        xmax-=(xmax-xmin)/4;
    }
};


/*
Z^2//doesnt work???
aa = a*a;
bb = b*b;
ab = a*b;
a = aa-bb;
b = 2*a*b

Z^3(-.8, .2), (.60596, .492761) ??cr=.1397451, ci=.812731;??
aaa = a*a*a;
bbb = b*b*b;
aab = a*a*b;
abb = a*b*b;
a = aaa - 3*abb + cr;
b = 3*aab - bbb + ci;

Z^4 //Z^3 is faster
aaaa = a*a*a*a;
bbbb = b*b*b*b;
aabb = a*a*b*b;
abbb = a*b*b*b;
aaab = a*a*a*b;
a = aaaa - 6*aabb + bbbb + cr;
b = 4*aaab - 4*abbb + ci;

e^Z^3 // cool
aaa = a*a*a;
bbb = b*b*b;
aab = a*a*b;
abb = a*b*b;
a = exp(aaa-3*abb)*cos(3*aab-bbb) + cr;
b = exp(aaa-3*abb)*sin(3*aab-bbb) + ci;

sinh(Z) //meh
a = .5*cos(b)*(exp(a)-exp(-a)) + cr;
b = .5*sin(b)*(exp(a)+exp(-a)) + ci;

sinh(Z^2) //cool
aa = a*a;
bb = b*b;
ab = a*b;
a = .5*cos(2*ab)*(exp(aa-bb) - exp(bb-aa)) + cr;
b = .5*sin(2*ab)*(exp(aa-bb) + exp(bb-aa)) + ci;

(Z^2+Z)/ln(Z) cool
aa = a*a;
bb = b*b;
ab = a*b;
atmp = a;
btmp = b;
a = cr + (.5*log(aa+bb)*(aa-bb+atmp) + atan2(btmp,atmp)*(2*ab+btmp))/
         (.25*pow(log(aa+bb), 2) + pow(atan2(btmp, atmp), 2));
b = ci + (.5*log(aa+bb)*(2*ab+btmp) - atan2(btmp, atmp)*(aa-bb+atmp))/
         (.25*pow(log(aa+bb), 2) + pow(atan2(btmp, atmp), 2));

sqrt(sinh(Z)) OK
atmp = a;
btmp = b;
a = pow(.5*exp(2*atmp)+.5*exp(-2*atmp), .25)*cos(btmp/2) + cr;
b = pow(.5*exp(2*atmp)+.5*exp(-2*atmp), .25)*sin(btmp/2) + ci;
*/

