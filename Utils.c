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
        sum+=t*t;
    }
    return sum;
}



double W(double** A,int i, int j, int n) {
    if(i==j){
        return 0;
    }
    double exponent  = euclideanDist(A[i],A[j],n)/2;
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

void freeMatrix(double** A, int n, int m ){
    int i;
    for(i=0;i<n;i++){
        free(A[i]);
    }
    free(A);
}
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