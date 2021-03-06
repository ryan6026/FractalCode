#include <math.h>
using namespace std;

template <typename T>
T MAX(T a, T b)
{
    if(a>b)
        return a;
    else
        return b;
}
double** flipMat(double **m, int n){//square matrix
    double MAX;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(m[i][j]>MAX)
                MAX = m[i][j];
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(m[i][j]>MAX)
                m[i][j]=MAX-m[i][j];
        }
    }
    return m;
}
double** getGaussFilter(double s, int n)
    {
        double ** k;
        //generate kernel
        k = new double*[n];
        double xtmp = 0, ytmp = 0;
        for(int i=0; i<n; i++)
        {
            k[i] = new double[n];
            xtmp = (((double)i/(double)n)-0.5);
            for(int j=0; j<n; j++)
            {
                ytmp = (((double)j/(double)n)-0.5);
                k[i][j] = exp(-(xtmp*xtmp+ytmp*ytmp)/(2*s*s))/(2*PI*s*s);
            }
        }

        //find max
        xtmp=0;
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
             if(xtmp<k[i][j])
                xtmp=k[i][j];
            }
        }
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
             k[i][j]*=10*xtmp;
            }
        }
        /*
        //flip matrix
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                k[i][j] = xtmp - k[i][j];
            }
        }
        */

        return k;
    }

class FFT2D
{
private:
    int n;
    double *real, *imag, **imageR, **imageI, **outTemp;

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

    void FFT(int dir,int m,double *x,double *y)
    {
        long nn,i,i1,j,k,i2,l,l1,l2;
        double c1,c2,tx,ty,t1,t2,u1,u2,z;

        /* Calculate the number of points */
        nn = 1;
        for (i=0; i<m; i++)
            nn *= 2;

        /* Do the bit reversal */
        i2 = nn >> 1;
        j = 0;
        for (i=0; i<nn-1; i++)
        {
            if (i < j)
            {
                tx = x[i];
                ty = y[i];
                x[i] = x[j];
                y[i] = y[j];
                x[j] = tx;
                y[j] = ty;
            }
            k = i2;
            while (k <= j)
            {
                j -= k;
                k >>= 1;
            }
            j += k;
        }

        /* Compute the FFT */
        c1 = -1.0;
        c2 = 0.0;
        l2 = 1;
        for (l=0; l<m; l++)
        {
            l1 = l2;
            l2 <<= 1;
            u1 = 1.0;
            u2 = 0.0;
            for (j=0; j<l1; j++)
            {
                for (i=j; i<nn; i+=l2)
                {
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
        if (dir == 1)
        {
            for (i=0; i<nn; i++)
            {
                x[i] /= (double)nn;
                y[i] /= (double)nn;
            }
        }
    }

public:
    FFT2D(int N) //length and width must be a power of two
    {
        n=N;
        imageI = Alloc2DArray<double>(n, n);
        imageR = Alloc2DArray<double>(n, n);
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                imageI[i][j] = 0;
                imageR[i][j] = 0;
            }
        }
        outTemp =  Alloc2DArray<double>(n, n);
    };

    void setPic(int **pic)
    {
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                imageR[i][j] = pic[i][j];
                imageI[i][j] = 0;
            }
        }
    }

    void setPic(double **pic)
    {
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                imageR[i][j] = pic[i][j];
                imageI[i][j] = 0;
            }
        }
    }

    void Transform(int dir)
    {
        int i, j;
        int m = log(n)/log(2);
        /* Transform the rows */
        double real[n];
        double imag[n];

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
    }

    void centerZero(){
        double **rtmp, **itmp;
            rtmp = Alloc2DArray<double>(n, n);
            itmp = Alloc2DArray<double>(n, n);
            for(int i=0; i<n/2; i++)//x
            {
                for(int j=0; j<n/2; j++)//y
                {
                    rtmp[i][j] = imageR[i+n/2][j+n/2];//goog
                    itmp[i][j] = imageI[i+n/2][j+n/2];
                }
                for(int j=n/2; j<n; j++)//y
                {
                    rtmp[i][j] = imageR[i+n/2][j-n/2];
                    itmp[i][j] = imageI[i+n/2][j-n/2];
                }
            }
            for(int i=n/2; i<n; i++)//x
            {
                for(int j=0; j<n/2; j++)//y
                {
                    rtmp[i][j] = imageR[i-n/2][j+n/2];
                    itmp[i][j] = imageI[i-n/2][j+n/2];
                }
                for(int j=n/2; j<n; j++)//y
                {
                    rtmp[i][j] = imageR[i-n/2][j-n/2];
                    itmp[i][j] = imageI[i-n/2][j-n/2];
                }
            }
            for(int i=0;i<n;i++){
                delete[] imageR[i];
                delete[] imageI[i];
            }
            imageR = rtmp;
            imageI = itmp;


    }

    void Filter(FFT2D a){
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                imageR[i][j] = imageR[i][j]*a.imageR[i][j] - imageI[i][j]*a.imageI[i][j];
                imageI[i][j] = imageI[i][j]*a.imageR[i][j] + imageR[i][j]*a.imageI[i][j];
            }
        }
    }

    void picMult(FFT2D a){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                imageR[i][j] *= a.getReal()[i][j];
                imageI[i][j] *= a.getImag()[i][j];
            }
        }
    }

    double** getMag()
    {
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                outTemp[i][j] = sqrt(imageR[i][j]*imageR[i][j]+imageI[i][j]*imageI[i][j]);
            }
        }
        return outTemp;
    }

    double** getLogMag()
    {
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                outTemp[i][j] = log(imageR[i][j]*imageR[i][j]+imageI[i][j]*imageI[i][j]);
            }
        }
        return outTemp;
    }

    double** getLogReal()
    {
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                outTemp[i][j] = log(imageR[i][j]);
            }
        }
        return outTemp;
    }

    double **getReal()
    {
        return imageR;}

    double ** getImag(){
        return imageI;}
    //add class operators for +-*/

};
