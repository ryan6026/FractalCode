#include <math.h>
#include <complex>
using namespace std;


template <typename T>
T MAX(T a, T b){
    if(a>b)
        return a;
    else
        return b;
}

class FFT2D
{
private:
    int n;
    double *real, *imag, **imageR, **imageI, **imageMag;
public:
    FFT2D(int N) //length and width must be a power of two
    {
       n=N;
        imageI = Alloc2DArray<double>(n, n);
        imageR = Alloc2DArray<double>(n, n);
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                imageI[i][j] = 0;
                imageR[i][j] = 0;
            }
        }

    };

    void setPic(int **pic){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                imageR[i][j] = pic[i][j];
                imageI[i][j] = 0;
            }
        }
    }

    int Powerof2(int n,int *m,int *twopm)
    {
        if (n <= 1)
        {
            *m = 0;
            *twopm = 1;
            return(false);
        }

        *m = 1;
        *twopm = 2;
        do
        {
            (*m)++;
            (*twopm) *= 2;
        }
        while (2*(*twopm) <= n);

        if (*twopm != n)
            return(false);
        else
            return(true);
    }

    int FFT(int dir,int m,double *x,double *y)
{
   long nn,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   nn = 1;
   for (i=0;i<m;i++)
      nn *= 2;

   /* Do the bit reversal */
   i2 = nn >> 1;
   j = 0;
   for (i=0;i<nn-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0;
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0;
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<nn;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1;
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1)
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == 1) {
      for (i=0;i<nn;i++) {
         x[i] /= (double)nn;
         y[i] /= (double)nn;
      }
   }

   return(true);
}

    int Transform(int dir)
    {
        int i, j;
        int m = log(n)/log(2);
        /* Transform the rows */
        real = new double[n];
        imag = new double[n];

        for (j=0; j<n; j++)
        {
            for (i=0; i<n; i++)
            {
                real[i] = imageR[i][j];
                imag[i] = imageI[i][j];
            }
            FFT(dir,m,real,imag);
            for (i=0; i<n; i++)
            {
                imageR[i][j] = real[i];
                imageI[i][j] = imag[i];
            }
        }

        /* Transform the columns */
        for (i=0; i<n; i++)
        {
            for (j=0; j<n; j++)
            {
                real[j] = imageR[i][j];
                imag[j] = imageI[i][j];
            }
            FFT(dir,m,real,imag);
            for (j=0; j<n; j++)
            {
                imageR[i][j] = real[j];
                imageI[i][j] = imag[j];
            }
        }
        delete[] real;
        delete[] imag;
        return true;
    }

    double** getMag(){
        imageMag =  Alloc2DArray<double>(n, n);
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                imageMag[i][j] = sqrt(imageR[i][j]*imageR[i][j]+imageI[i][j]*imageI[i][j]);
            }
        }
        return imageMag;
    }

    double** getLogMag(){
        imageMag =  Alloc2DArray<double>(n, n);
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                imageMag[i][j] = log(imageR[i][j]*imageR[i][j]+imageI[i][j]*imageI[i][j]);
            }
        }
        return imageMag;
    }

    double **getReal(){
    return imageR;}

    double ** getImag(){
    return imageI;}
};
