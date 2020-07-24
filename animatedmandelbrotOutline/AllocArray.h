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
