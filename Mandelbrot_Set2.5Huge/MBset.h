#include <cmath>
#include <iostream>
using namespace std;

template <typename T>
T **Alloc2DArray( int nRows, int nCols) {
    T **dynamicArray;
    dynamicArray = new T*[nRows];
    for( int i = 0 ; i < nRows ; i++ )
        dynamicArray[i] = new T [nCols];
    return dynamicArray;
}

class MBset {
  private:
    double w=1.8, h, xmin, ymin, xmax, ymax, dx, dy,
           a, b, aa, bb, ab, aaaa, bbbb, aaab, abbb, aabb,
           bbb, abb, aab, atmp, btmp,
           cr, ci, maxiterations, y, x;;
    int ImageW, ImageH;// 40-50
    int** n;

  public:
    MBset(int ImageWidth, int ImageHeight, int maxIt) {
        maxiterations = maxIt;
        ImageW = ImageWidth;
        ImageH = ImageHeight;
        n = Alloc2DArray<int>(ImageW, ImageH);
        h = (w * ImageH) / ImageW;
        xmin = -w/2;
        ymin = -h/2;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        };

    int** getSet(double Cr, double Ci, int setNum) {
        cr = Cr; //-0.096, 0.407
        ci = Ci;

        y = ymin;
        for(int i=0; i<ImageW; i++) {
            x = xmin;
            for (int j=0; j<ImageH; j++) {
                a = x;
                b = y;
                n[i][j]=0;
                while (n[i][j] < maxiterations) {
                    if(setNum==4){
                        aaaa = a*a*a*a;
                        bbbb = b*b*b*b;
                        aabb = a*a*b*b;
                        abbb = a*b*b*b;
                        aaab = a*a*a*b;
                        a = aaaa - 6*aabb + bbbb + cr;
                        b = 4*aaab - 4*abbb + ci;
                    }else if(setNum==3){
                        aaa = a*a*a;
                        bbb = b*b*b;
                        aab = a*a*b;
                        abb = a*b*b;
                        a = aaa - 3*abb + cr;
                        b = 3*aab - bbb + ci;
                    }else if(setNum==2){
                        aaa = a*a*a;
                        bbb = b*b*b;
                        aab = a*a*b;
                        abb = a*b*b;
                        a = exp(aaa-3*abb)*cos(3*aab-bbb) + cr;
                        b = exp(aaa-3*abb)*sin(3*aab-bbb) + ci;
                    }
                    if((a*a+b*b)>50){
                        break;
                    }
                    n[i][j]++;
                }
                x += dx;
            }
            y += dy;
        }
        return n;
    }

    void ZoomOut() {
        w+=.2;
        h = (w * ImageH) / ImageW;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        cout << "W: " << w << endl;
    }
    void ZoomIn() {
        w-=.2;
        h = (w * ImageH) / ImageW;
        xmax = xmin + w;
        ymax = ymin + h;
        dx = (xmax - xmin) / ImageW;
        dy = (ymax - ymin) / ImageH;
        cout << "W: " << w << endl;
    }
    void ShiftRight() {
        ymin+=(ymax-ymin)/16;
        ymax+=(ymax-ymin)/16;
        cout << "xmin, ymin: " << xmin << ", " << ymin << endl;
    }
    void ShiftLeft() {
        ymin-=(ymax-ymin)/16;
        ymax-=(ymax-ymin)/16;
        cout << "xmin, ymin: " << xmin << ", " << ymin << endl;
    }
    void ShiftDown() {
        xmin+=(xmax-xmin)/16;
        xmax+=(xmax-xmin)/16;
        cout << "xmin, ymin: " << xmin << ", " << ymin << endl;
    }
    void ShiftUp() {
        xmin-=(xmax-xmin)/16;
        xmax-=(xmax-xmin)/16;
        cout << "xmin, ymin: " << xmin << ", " << ymin << endl;
    }

};

/*

Z^2//doesnt work???
aa = a*a;
bb = b*b;
ab = a*b;
a = aa-bb+cr;
b = 2*a*b+ci;

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

sqrt(sinh(Z)) OK
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

