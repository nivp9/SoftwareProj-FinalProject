#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "spkmeans.h"

#define assertAndReturn(cond)                   \
        if (!(cond)) {                          \
            printf("An Error Has Occurred");    \
            exit(1);                           \
        }                                       \



double ** multMatrix(double **A, double ** B, int An, int Am, int Bn, int Bm){
    double **res;
    int i, j, k;
    double cellVal;
    if(Am!= Bn){
        return NULL;
    }
    res = (double **)(calloc(An, sizeof(double*)));
    assertAndReturn(res !=NULL);
    for(i=0;i<An;i++){
        res[i]=(double *) calloc(Bm,sizeof (double));
        assertAndReturn(res[i] !=NULL);
        for(j=0;j<Bm;j++){
            cellVal=0;
            for(k=0;k<Am;k++){
                cellVal+=A[i][k]*B[k][j];
            }
            res[i][j]= cellVal;
        }
    }
    return res;
}

double euclideanDist(double* a, double* b, int n){
    int i=0;
    int sum =0;
    double t=0;
    for(;i<n;i++){
        t=(a[i]-b[i]);
        sum += (t*t);
    }
    return sum;
}



double W(double** A,int i, int j, int n) {
    if(i==j){
        return 0;
    }
    double exponent  = -1 * euclideanDist(A[i],A[j],n)/2;   // add -1
    return exp(exponent);
}

double** Transpose(double**A, int n, int m){
    double **res;
    int i, j;
    res = (double **)(calloc(m, sizeof(double*)));
    assertAndReturn(res !=NULL);
    for(i=0;i<m;i++){
        res[i]=(double *) calloc(n,sizeof (double));
        assertAndReturn(res[i] !=NULL);
        for(j=0;j<n;j++){
            res[i][j]= A[j][i];
        }
    }
    return res;
}

double ** calculateNewA (double**A, int n, int i, int j, int c, int s){
    int r;
    for (r = 0; r < n; r++){
        if (r != i && r != j){
            A[r][i] = c*A[r][i] - s*A[r][j];
            A[r][j] = c*A[r][j] + s*A[r][i];
        }
    }
    A[i][i] = pow(c,2)*A[i][i] + pow(s,2)*A[j][j] - 2*s*c*A[i][j];
    A[j][j] = pow(s,2)*A[i][i] + pow(c,2)*A[j][j] + 2*s*c*A[i][j];
    A[i][j] = 0;
    return A;
}


//int Convergence (double**A, int n)

double sumOfSquaresOffDiagonal (double**A, int n){
    double sum = 0;
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if (i != j){
                sum += (A[i][j] * A[i][j]);
            }
        }
    }
    return sum;
}

int isDiagonal (double**A, int n, int m){
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if (i != j && A[i][j] != 0){
                return 0;
            }
        }
    }
    return 1;
}

double ** createPMat (int i, int j, int c, int s, int n){
    double **res = createMatrix(n,n);
    int k;
    for (k = 0; k < n; k++) {
        res[K][K] = 1;        
    }
    A[i][i] = c;
    A[j][j] = c;
    A[i][j] = s;
    A[j][i] = -1 * s;

    return res;
}


/*
void freeMatrix(double** A, int n, int m ){
    int i;
    for(i=0;i<n;i++){
        free(A[i]);
    }
    free(A);
}
*/

double ** createMatrix(int n, int m) {
    double **res;
    int i, j;
    res = (double **) (calloc(n, sizeof(double *)));
    assertAndReturn(res != NULL);
    for (i = 0; i < m; i++) {
        res[i] = (double *) calloc(m, sizeof(double));
    }
    return res;
}


void freeMatrix(double**A, int n) {
    int i;
    for (i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
}

void printMatrix(double ** d, int n, int m ){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            
            printf("%0.4f  ,",d[i][j]);
        }
        printf("\n");
    }
}