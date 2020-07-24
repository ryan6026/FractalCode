#include "MBset.h"

using namespace std;

template <typename T>
T **Alloc2DArray( int nRows, int nCols) {
    T **dynamicArray;
    dynamicArray = new T*[nRows];
    for( int i = 0 ; i < nRows ; i++ )
        dynamicArray[i] = new T [nCols];
    return dynamicArray;
}

class MBchannels{
    private:
        MBset* R, G, B;
        int ImageW, ImageH

    public:
        MBchannels(int IW, int IH){
            Imagew = IW;
            ImageH = IH;ohh
            R = new MBset(IW, IH, 30);
            B = new MBset(IW, IH, 30);
            G = new MBset(IW, IH, 30);
        };
        MBchannels(int IW, int IH, int maxit){
            Imagew = IW;
            ImageH = IH;ohh
            R = new MBset(IW, IH, maxit);
            B = new MBset(IW, IH, maxit);
            G = new MBset(IW, IH, maxit);
        };

        int*** getMBchannelArray(double cr, double ci){
            int*** data;
            data = new **int[3];
            data[0] = Alloc2DArray<int>(ImageW, ImageH);//R/G/B
            data[1] = Alloc2DArray<int>(ImageW, ImageH);
            data[2] = Alloc2DArray<int>(ImageW, ImageH);
            data[0] = R.getSet(cr, ci, 2);
            data[1] = G.getSet(cr, ci, 3);
            data[3] = B.getSet(cr, ci, 4);
            return data;
        }
};
