#ifndef MBCHANNELS_H
#define MBCHANNELS_H
#include "MBset.h"
#include "AllocArray.h"
using namespace std;

class MBchannels {
  private:
    MBset* R;
    MBset* G;
    MBset* B;

    int ImageW, ImageH;
    int*** n;
  public:
    MBchannels(int IW, int IH) {
        ImageW = IW;
        ImageH = IH;
        R = new MBset(IW, IH);
        B = new MBset(IW, IH);
        G = new MBset(IW, IH);
        n = new int**[3];
        n = Alloc3DArray<int>(3, ImageW, ImageH);
    };
/*
    int*** getMBchannelArray(double cr, double ci, int maxit) {
        n[0] = R->getSet2(cr, ci, maxit);
        n[1] = G->getSet3(cr, ci, maxit);
        n[2] = B->getSet4(cr, ci, maxit);
        return n;
    }
*/
    int*** getMBchannelArrayMulti(double cr, double ci, int maxit, int SetsI) {
        n[0] = R->getMultiSet2(cr, ci, maxit, SetsI);
        n[1] = n[0];//G->getMultiSet2(cr, ci, maxit, SetsI);
        n[2] = n[0];//B->getMultiSet2(cr, ci, maxit, SetsI);
        return n;
    }

    void ZoomIn() {
        R->ZoomIn();
        G->ZoomIn();
        B->ZoomIn();
    }
    void ZoomOut() {
        R->ZoomOut();
        G->ZoomOut();
        B->ZoomOut();
    }
    void ShiftRight() {
        R->ShiftRight();
        G->ShiftRight();
        B->ShiftRight();
    }
    void ShiftLeft() {
        R->ShiftLeft();
        G->ShiftLeft();
        B->ShiftLeft();
    }
    void ShiftUp() {
        R->ShiftUp();
        G->ShiftUp();
        B->ShiftUp();
    }
    void ShiftDown() {
        R->ShiftDown();
        G->ShiftDown();
        B->ShiftDown();
    }
    double getW(){
        return R->getW();}
    double getxmin(){
        return R->getxmin();
    }
    double getymin(){
        return R->getymin();
    }
    void setW(double WW){
        R->setW(WW);
        G->setW(WW);
        B->setW(WW);
    }
};
#endif
