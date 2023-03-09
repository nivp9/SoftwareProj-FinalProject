

double** multMatrix(double **A, double ** B, int An, int Am, int Bn, int Bm);
double euclideanDist(double* a, double* b, int n);
double W(double** A,int i, int j, int n);
double** Transpose(double**A, int n, int m);

double** calculateNewA (double**A, int n, int i, int j, int c, int s);
double sumOfSquaresOffDiagonal (double**A, int n);
int isDiagonal (double**A, int n, int m);
double ** createPMat (double**A, int i, int j, int c, int s, int n);
double* largestAbsVal (double**A, int n);

double** createMatrix(int n, int m);
void printMatrix(double ** d, int n, int m );
void freeMatrix(double**A, int n);



double** wam(double** A, int n, int k);
double** ddg(double **wamMat, int n);
double** gl(double **wamMat, double **ddgMat,int n);
void prepData(char *filename);


