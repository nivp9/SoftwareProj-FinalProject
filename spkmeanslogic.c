#include "spkmeans.h"
int main(int argc, char *argv[] )
{
    return 0;
}
double** wam(double** A, int n){
    int i,j;
    double** res = createMatrix(n,n);
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            res[i][j] = W(A,i,j,n);
        }
    }
    return res;
}