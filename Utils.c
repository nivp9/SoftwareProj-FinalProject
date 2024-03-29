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
    double exponent;
    if(i==j){
        return 0;
    }
    exponent  = -1 * euclideanDist(A[i],A[j],n)/2; 
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

            A[i][r] = c*A[r][i] - s*A[r][j];
            A[j][r] = c*A[r][j] + s*A[r][i];
        }
    }
    A[i][i] = pow(c,2)*A[i][i] + pow(s,2)*A[j][j] - 2*s*c*A[i][j];
    A[j][j] = pow(s,2)*A[i][i] + pow(c,2)*A[j][j] + 2*s*c*A[i][j];
    A[i][j] = 0;
    A[j][i] = 0;
    return A;
}


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

double* offDiaglargestAbsVal (double**A, int n){
    int i, j;
    double maxVal = 0;
    double* res = calloc(3, sizeof(int));

    for(i = 0; i < n; i++){
        for(j = i+1; j < n; j++) {
            if (abs(A[i][j]) > abs(maxVal) && i!=j) {
                maxVal = A[i][j];
                res[0] = i;
                res[1] = j;
            }
        }
    }
    res[2] = maxVal;
    return res;
}


double* obtainCS (double**A, int i, int j){
    double teta = (A[j][j] - A[i][i]) / (2 * A[i][j]);
    int signTeta = teta > 0 ? 1 : -1;
    double t = signTeta / (abs(teta) + sqrt(pow(teta,2) + 1));
    double c = 1 / (sqrt(pow(t,2) + 1));
    double s = t * c;
    double* res = (double *)calloc(2,sizeof(double ));
    assertAndReturn(res !=NULL);
    res[0] = c;
    res[1] = s;
    return res;
}
double obtainT (double**A, int i, int j){
    double teta = (A[j][j] - A[i][i]) / (2 * A[i][j]);
    int signTeta = teta > 0 ? 1 : -1;
    double t = signTeta / (abs(teta) + sqrt(pow(teta,2) + 1));
    return t;
}
double obtainC (double**A, int i, int j){
    double t = obtainT(A,i,j);
    return  1 / (sqrt(pow(t,2) + 1));
}
double obtainS (double**A, int i, int j){
    double t = obtainT(A,i,j);
    double c = obtainC(A,i,j);
    return t*c;
}



int isDiagonal (double**A, int n){
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

double ** createPMat (double**A, int maxi, int maxj, int n){
    double **res = createMatrix(n,n);
    int k;
    double c,s;
    c = obtainC (A, maxi, maxj);
    s = obtainS (A, maxi, maxj);
    for (k = 0; k < n; k++) {
        res[k][k] = 1;
    }
    A[maxi][maxi] = c;
    A[maxj][maxj] = c;
    A[maxi][maxj] = s;
    A[maxj][maxi] = -1 * s;

    return res;
}



double ** createMatrix(int n, int m) {
    double **res;
    int i;
    res = (double **) (calloc(n, sizeof(double *)));
    assertAndReturn(res != NULL);
    for (i = 0; i < m; i++) {
        res[i] = (double *) calloc(m, sizeof(double));
    }
    return res;
}

double ** createIMatrix(int n) {
    double **res = createMatrix(n,n);
    int i;
    for (i = 0; i < n; i++) {
        res[i][i] = 1;
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
            if (j==m-1){
                printf("%0.4f",d[i][j]);
            }
            else{
                printf("%0.4f,",d[i][j]);
            }

        }
        printf("\n");
    }
}
void printDiag(double ** d, int n){
    int i;
    for(i=0;i<n;i++){
        if (i==n-1){
            printf("%0.4f",d[i][i]);
        }
        else{
            printf("%0.4f,",d[i][i]);
        }

    }
}
double off(double** A, int n){
    int i,j;
    double res=0;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(i!=j){
                res += pow(A[i][j],2);
            }
        }
    }
    return res;
}