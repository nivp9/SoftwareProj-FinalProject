#include "spkmeans.h"
#include "Utils.c"

double** wam(double** A, int n, int k){
    int i,j;
    double** res = createMatrix(n,n);
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            res[i][j] = W(A,i,j,n);
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