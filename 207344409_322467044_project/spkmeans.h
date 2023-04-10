# ifndef SPKMEANS_H_
# define SPKMEANS_H_
struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};

double** multMatrix(double **A, double ** B, int An, int Am, int Bn, int Bm);
double euclideanDist(double* a, double* b, int n);
double W(double** A,int i, int j, int n);
double** Transpose(double**A, int n, int m);

void calculateNewA (double**A, int maxi, int maxj, int n);
double sumOfSquaresOffDiagonal (double**A, int n);
int isDiagonal (double**A, int n);
double ** createP (double**A, int maxi, int maxj, int n);
int* offDiaglargestAbsVal (double**A, int n);
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

double** spk(double **icentroids, double **idata, int ik,int irowcount, int icolumncount);
void prepData(char *filename);


double off(double** A, int n);




void setZerosToNewCentroids();
void initCentroidsLinkedList(double **icentroid);
void initDataLinkedList(double **idata);
void copyFirstKToNew();
double** convertLinkedListToArray(struct vector *v, int vectors, int cords);
void freeLinkedList(struct vector *vec);
void freeCordsList(struct cord *cord);

void setDataFromPython(double **iCentroid, double **idata, int ik,int irowcount, int icolumncount);
double calcMaxCentroidsDelta();

# endif