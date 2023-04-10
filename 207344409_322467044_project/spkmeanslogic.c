#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "spkmeans.h"
#include <string.h>



double** wam(double** A, int n, int m){
    int i,j;
    double** res = createMatrix(n,n);
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            res[i][j] = W(A,i,j,m);
        }
    }
    return res;
}
double **ddg(double **wamMat, int n){
    int i,j;
    double sum;
    double** res = createMatrix(n,n);
    for(i=0;i<n;i++){
        sum = 0;
        for(j = 0;j<n;j++){
            sum+=wamMat[i][j];
            res[i][j]=0;
        }
        res[i][i]=sum;
    }
    return res;
}
double** gl(double **wamMat,double **ddgMat, int n){
    int i,j;
    double** res =createMatrix(n,n);
    for(i=0;i<n;i++){
        for(j = 0;j<n;j++){
            res[i][j]= ddgMat[i][j]-wamMat[i][j];
        }
    }
    return res;
}


double*** jacobi(double** A,int n){
    int i,j;
    char * helperStr = calloc(100, sizeof(char));

    double currDelta=1;
    int iter=0;
    double** V = createIMatrix(n);
    double ** Vtmp,**Atemp;
    double *** res;

    double ** P, **PT;
    int maxI,maxJ;
    double offA,offAt;
    int* biggestEl;

    while(iter<100 && currDelta>0.00001 && isDiagonal(A,n) == 0 ){
        biggestEl= offDiaglargestAbsVal(A,n);

        offA = off(A, n);
        
        maxI=biggestEl[0];
        maxJ=biggestEl[1];
        
        free(biggestEl);
        P = createP(A,maxI,maxJ,n);
        PT = Transpose(P,n,n);

        Atemp = multMatrix(PT,A,n,n,n,n);
        freeMatrix(A,n);
        freeMatrix(PT,n);
        A = multMatrix(Atemp,P,n,n,n,n);
        
        freeMatrix(Atemp,n);

        Vtmp = multMatrix(V,P,n,n,n,n); 
        freeMatrix(V,n);
        freeMatrix(P,n);
        V = Vtmp;
        offAt = off(A, n);
        currDelta = offA-offAt;
        iter++;
    }
    for(i=0;i<n;i++){
      sprintf(helperStr, "%.4f", A[i][i]);
        if(strcmp(helperStr,"-0.0000")==0)
        {
            A[i][i] = 0;
            for(j=0;j<n;j++) 
            {
                V[j][i] = -1*V[j][i];
            }
        }
    }
    free(helperStr);
    res = calloc(2, sizeof(double**));
    res[0]=A;
    res[1]=V;
    return res;
}
