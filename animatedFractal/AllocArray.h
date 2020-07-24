#pragma once

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

template <typename T>
T **Alloc2DArray( int nRows, int nCols) {
    T **dynamicArray;
    dynamicArray = new T*[nRows];
    for( int i = 0 ; i < nRows ; i++ )
        dynamicArray[i] = new T [nCols];
    return dynamicArray;
}

class line{
public:
    int x1, y1, x2, y2;
    line(int X1, int Y1, int X2, int Y2){
        x1=X1;x2=X2;y1=Y1;y2=Y2;
    }
};
