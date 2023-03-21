#include "spkmeans.h"
#include "Utils.c"


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
    double currDelta=1;
    int iter=0;
    double** V = createIMatrix(n);
    double ** Vtmp,**Atemp;
    double *** res;

    double ** P, **PT;
    int maxI,maxJ;
    double offA,offAt;
    double* biggestEl;

    while(iter<100 && currDelta>0.00001 && !isDiagonal(A,n)){

        biggestEl= offDiaglargestAbsVal(A,n);

        offA = off(A, n);

        maxI=biggestEl[0];
        maxJ=biggestEl[1];
        free(biggestEl);
        P=createPMat(A,maxI,maxJ,n);
        PT=Transpose(P,n,n);

        Atemp= multMatrix(PT,A,n,n,n,n);
        freeMatrix(A,n);
        freeMatrix(PT,n);
        A= multMatrix(Atemp,P,n,n,n,n);
        freeMatrix(Atemp,n);

        Vtmp =multMatrix(V,P,n,n,n,n);
        freeMatrix(V,n);
        freeMatrix(P,n);
        V=Vtmp;
        offAt = off(A, n);
        currDelta =offAt-offA;
        iter++;

    }
    res = calloc(2, sizeof(double**));
    res[0]=A;
    res[1]=V;
    return res;
}