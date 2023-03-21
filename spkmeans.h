# ifndef SPKMEANS_H_  // added
# define SPKMEANS_H_

double** multMatrix(double **A, double ** B, int An, int Am, int Bn, int Bm);
double euclideanDist(double* a, double* b, int n);
double W(double** A,int i, int j, int n);
double** Transpose(double**A, int n, int m);

double** calculateNewA (double**A, int n, int i, int j, int c, int s);
double sumOfSquaresOffDiagonal (double**A, int n);
int isDiagonal (double**A, int n);
double ** createPMat (double**A, int maxi, int maxj, int n);
double* offDiaglargestAbsVal (double**A, int n);
double obtainT (double**A, int i, int j);
double obtainC (double**A, int i, int j);
double obtainS (double**A, int i, int j);



double** createMatrix(int n, int m);
double ** createIMatrix(int n);
void printMatrix(double ** d, int n, int m );
void printDiag(double ** d, int n);
void freeMatrix(double**A, int n);



double** wam(double** A, int n, int m);
double** ddg(double **wamMat, int n);
double** gl(double **wamMat, double **ddgMat ,int n);
double*** jacobi(double** A,int n);

double** spk(double **icentroids, double **idata, int ik,int iiter, double iepsilon,int irowcount, int icolumncount);
void prepData(char *filename);


double off(double** A, int n);

# endif
