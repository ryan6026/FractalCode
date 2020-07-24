/*
I think i can optimize the algorithm
***combine while loop and setsI for loop***
*/
#pragma once
#include <cmath>
#include "AllocArray.h"
#include "M_APM.H"
#include <stdlib.h>
#include <iostream>
#define MAX_DIGITS 5

using namespace std;

void	m_apm_set_to_zero(M_APM z) {
    z->m_apm_datalength = 1;
    z->m_apm_sign       = 0;
    z->m_apm_exponent   = 0;
    z->m_apm_data[0]    = 0;
}

char* toString(M_APM a) {
    char str[8];
    m_apm_to_string(str, 4, a);
    return str;
}

class MBset {
  private:
    double w=3, h, xmin, ymin, xmax, ymax, dx, dy, x, y;
    M_APM a, b, aa, bb, ab, cr, ci, sum;
    int ImageW, ImageH;// 40-50
    int** n;
    int** tmp;
    char str[8];
  public:
    MBset(int ImageWidth, int ImageHeight) {
        ImageW = ImageWidth;
        ImageH = ImageHeight;
        n = Alloc2DArray<int>(ImageW, ImageH);
        tmp = Alloc2DArray<int>(ImageW, ImageH);
        h = (w * ImageH) / ImageW;
        xmin = -w/2;
        ymin = -h/2;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
    };

    int** getMultiSet2(double Cr, double Ci, int maxiterations, int SetsI) {
        for(int i=0; i<ImageW; i++) {
            for(int j=0; j<ImageW; j++) {
                n[i][j] = 0;                //clears previous set
            }
        }
        m_apm_cpp_precision(MAX_DIGITS);
        a = m_apm_init();
        b = m_apm_init();
        aa = m_apm_init();
        bb = m_apm_init();
        ab = m_apm_init();
        cr = m_apm_init();
        ci = m_apm_init();
        sum = m_apm_init();
        m_apm_set_double(cr, Cr);//cr = Cr;
        m_apm_set_double(ci, Ci);//ci = Ci;
        for(int I=0; I<SetsI; I++) {
            cout << I+1 << " of " << SetsI << "..." << endl;
            y = ymin;
            for(int i=0; i<ImageW; i++) {
                x = xmin;
                for (int j=0; j<ImageH; j++) {
                    m_apm_set_double(a, x);//a = x;
                    m_apm_set_double(b, y);//b = y;
                    tmp[i][j]=0;
                    while (tmp[i][j] < maxiterations*(1+I)) {
                        m_apm_multiply(aa, a, a); //aa=a*a
                        m_apm_multiply(bb, b, b); //bb = b*b
                        m_apm_multiply(ab, a, b); //ab = a*b;
                        m_apm_absolute_value(ab, ab);
                        m_apm_set_to_zero(a);
                        m_apm_set_to_zero(b);
                        //intermediate vars set, overwrite a, b
                        m_apm_add(a, a, aa);
                        m_apm_subtract(a, a, bb);
                        m_apm_add(a, a, cr);
                        m_apm_add(b, b, ab);
                        m_apm_add(b, b, ab);
                        m_apm_add(b, b, ci);
                        m_apm_round(a, MAX_DIGITS, a);
                        m_apm_round(b, MAX_DIGITS, b);
                        m_apm_set_to_zero(sum);
                        m_apm_add(sum, aa, bb);
                        m_apm_round(sum, MAX_DIGITS, sum);
                        m_apm_to_string(str, 8, sum);
                        if(atof(str)>4.0) {
                            //n[i][j] = n[i][j] + atan2(b, a);//for phase coloring
                            break;
                        }
                        tmp[i][j]++;
                    }
                    x += dx;
                }
                y += dy;
            }
            MatrixAdd(n, n, tmp, ImageW, ImageH);
        }
        m_apm_free(a);
        m_apm_free(b);
        m_apm_free(aa);
        m_apm_free(ab);
        m_apm_free(bb);
        m_apm_free(cr);
        m_apm_free(ci);
        m_apm_free(sum);
        return n;
    }

    void ZoomOut() {
        w+=.2;
        h = (w * ImageH) / ImageW;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        //cout << "W: " << w << endl;
    }
    void ZoomIn() {
        w-=.2;
        h = (w * ImageH) / ImageW;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        //cout << "W: " << w << endl;
    }
    void ShiftRight() {
        ymin+=(ymax-ymin)/16;
        ymax+=(ymax-ymin)/16;
        //cout << "xmin, ymin: " << xmin << ", " << ymin << endl;
    }
    void ShiftLeft() {
        ymin-=(ymax-ymin)/16;
        ymax-=(ymax-ymin)/16;
        //cout << "xmin, ymin: " << xmin << ", " << ymin << endl;
    }
    void ShiftDown() {
        xmin+=(xmax-xmin)/16;
        xmax+=(xmax-xmin)/16;
        //cout << "xmin, ymin: " << xmin << ", " << ymin << endl;
    }
    void ShiftUp() {
        xmin-=(xmax-xmin)/16;
        xmax-=(xmax-xmin)/16;
        //cout << "xmin, ymin: " << xmin << ", " << ymin << endl;
    }
    double getxmin() {
        return xmin;
    }
    double getymin() {
        return ymin;
    }
    void setW(double WW) {
        w = WW;
    }
    double getW() {
        return w;
    }
};

/*

Z^2//doesnt work???
aa = a*a;
bb = b*b;
ab = a*b;
a = aa-bb+cr;
b = 2*ab+ci;

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

Z^5 doesnt work?
aaaaa = a*a*a*a*a;
bbbbb = b*b*b*b*b;
aaabb = a*a*a*b*b;
aabbb = a*a*b*b*b;
aaaab = a*a*a*a*b;
a = aaaaa-10*aaabb+5*abbbb + cr;
b = 5*aaaab-10*aabbb+bbbbb + ci;

e^Z^2 //untested
aa = a*a;
bb = b*b;
ab = a*b;
a = exp(aa-bb)*cos(2ab)
b = exp(aa-bb)*sin(2ab)

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

sqrt(sinh(Z)) //OK
atmp = a;
btmp = b;
a = pow(.5*exp(2*atmp)+.5*exp(-2*atmp), .25)*cos(btmp/2) + cr;
b = pow(.5*exp(2*atmp)+.5*exp(-2*atmp), .25)*sin(btmp/2) + ci;

sinh(Z^2) //cool
aa = a*a;
bb = b*b;
ab = a*b;
a = .5*cos(2*ab)*(exp(aa-bb) - exp(bb-aa)) + cr;
b = .5*sin(2*ab)*(exp(aa-bb) + exp(bb-aa)) + ci;

a = .5*cos(2*ab)*(exp(aa-bb) - exp(bb-aa)) + cr;
b = .5*sin(2*ab)*(exp(aa-bb) + exp(bb-aa)) + ci;

sqrt(sinh(Z^2))
aa = a*a;
bb = b*b;
ab = a*b;
a = pow(.5*exp(2*(aa-bb))+.5*exp(2*(bb-aa)), .25)*cos(ab) + cr;
b = pow(.5*exp(2*(aa-bb))+.5*exp(2*(bb-aa)), .25)*sin(ab) + ci;

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


*/

