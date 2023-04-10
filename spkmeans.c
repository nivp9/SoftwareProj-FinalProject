#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "spkmeans.h"


#define assertAndReturn(cond)                   \
        if (!(cond)) {                          \
            printf("An Error Has Occurred");    \
            exit(1);                           \
        }                                       \


void freeLinkedList(struct vector *vec);
void freeCordsList(struct cord *cord);
double d(struct cord v1,struct cord v2);
int errorOccurred = 0;
struct vector *data =NULL;
struct vector *newCentroids;
struct vector *prevCentroids;


int k;






char *goal, *file;
int rowsCount=0;
int columnCount=0;
double **data_points =NULL;

int main(int argc, char *argv[] )
{
    if(argc != 3){ 
        printf("An Error Has Occurred");
        return 0;
    }
    goal = argv[1];
    file = argv[2];
    prepData(file);


    if(strcmp(goal,"wam")==0){
        double **wamRes = wam(data_points, rowsCount,columnCount);
        printMatrix(wamRes, rowsCount, rowsCount);
        freeMatrix(wamRes,rowsCount);
        freeMatrix(data_points,rowsCount);
    }
    else if(strcmp(goal,"ddg")==0){  
        double **wamRes = wam(data_points, rowsCount,columnCount);
        double **ddgRes = ddg(wamRes,rowsCount);
        printMatrix(ddgRes, rowsCount, rowsCount);
        freeMatrix(wamRes,rowsCount);
        freeMatrix(ddgRes,rowsCount);
        freeMatrix(data_points,rowsCount);

    }
    else if(strcmp(goal,"gl")==0){
        double **wamRes = wam(data_points, rowsCount,columnCount);
        double **ddgRes = ddg(wamRes,rowsCount);
        double **glRes = gl(wamRes,ddgRes,rowsCount);
        printMatrix(glRes, rowsCount, rowsCount);
        freeMatrix(wamRes,rowsCount);
        freeMatrix(ddgRes,rowsCount);
        freeMatrix(glRes,rowsCount);
        freeMatrix(data_points,rowsCount);
    } 
    else if(strcmp(goal,"jacobi")==0){
        double ***jacobiRes;
        assertAndReturn(rowsCount==columnCount);
        jacobiRes = jacobi(data_points, rowsCount);
        printDiag(jacobiRes[0], rowsCount);    
        printf("\n");
        printMatrix(jacobiRes[1], rowsCount, rowsCount);

        freeMatrix(jacobiRes[1],rowsCount);
        freeMatrix(jacobiRes[0],rowsCount);
        free(jacobiRes);
    }
    else {
        printf("An Error Has Occurred");
        return 0;
    }
    return 1;
}


void prepData(char *filename){

    double n;
    char c;
    int columnsCountInitialized=0;
    int firstrun=1;
    struct vector *head_vec, *curr_vec,*prev_vec = malloc(sizeof(struct vector));
    struct cord *head_cord, *curr_cord;
    FILE *f = fopen(filename, "r");
    assertAndReturn(f!=NULL);

    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;


    while (fscanf(f,"%lf%c", &n, &c) == 2)
    {
        if(!columnsCountInitialized){
            columnCount++;
        }

        if (c == '\n' || c=='\r')
        {
            columnsCountInitialized=1;
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            if(firstrun==1){
                free(prev_vec);
                firstrun=0;
            }
            prev_vec=curr_vec;
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
            rowsCount++;
            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;

    }
    if(prev_vec!=NULL){
        prev_vec->next = NULL;
    }
    free(head_cord); 
    free(curr_vec); 
    fclose(f);
    data_points = convertLinkedListToArray(head_vec, rowsCount,columnCount);
    freeLinkedList(head_vec);

}






double** spk(double **icentroids, double **idata, int ik,int irowcount, int icolumncount){
    int i =0,j=0;
    double minDist;
    struct vector *currPrevCentroid;
    double **newCentrioidsAsArray;
    double maxCentroidDelta;
    int iteration_number=0;
    setDataFromPython(icentroids, idata, ik, irowcount, icolumncount);
    maxCentroidDelta = 1;
    while(iteration_number < 300 && maxCentroidDelta>=0){
        struct vector *currNewCentroid;

        struct vector *currVector = data;
        int *assignmentsCounter = calloc(k, sizeof (int));
        assertAndReturn(assignmentsCounter!=NULL);
        freeLinkedList(prevCentroids);
        prevCentroids = newCentroids;
        setZerosToNewCentroids();
        i = 0;
        for(; i< rowsCount; i++){
            int minIndex= 0;
            struct vector *minPointer;


            struct cord *minDistCentCord;
            struct cord *cordsToAdd;


            currNewCentroid = newCentroids;
            currPrevCentroid = prevCentroids;
            minDist = d(*currPrevCentroid->cords,*currVector->cords);
            minPointer = currNewCentroid;

            j =1;
            for (; j< k; j++){
                double dis;
                currPrevCentroid = currPrevCentroid->next;
                currNewCentroid = currNewCentroid->next;  
                dis = d(*currPrevCentroid->cords,*currVector->cords);
                if(dis<minDist) {
                    minDist=dis;
                    minPointer = currNewCentroid;
                    minIndex=j;
                }
            }   
            assignmentsCounter[minIndex]++;


            minDistCentCord = minPointer->cords;
            cordsToAdd = currVector->cords;
            j = 0;
            for(; j<columnCount; j++){
                minDistCentCord->value += cordsToAdd->value;
                minDistCentCord = minDistCentCord->next;
                cordsToAdd = cordsToAdd->next;
            }

            currVector= currVector->next;
        }


        currNewCentroid= newCentroids;
        i = 0;
        for (;i<k;i++){
            if(assignmentsCounter[i]>0) {
                struct cord *cordsToChange = currNewCentroid->cords;
                int j=0;
                for(;j<columnCount;j++){
                    cordsToChange->value = cordsToChange->value / assignmentsCounter[i];
                    if (j != columnCount) {
                        cordsToChange = cordsToChange->next;
                    }
                }
            }
            currNewCentroid=currNewCentroid->next;
        }


        free(assignmentsCounter);
        iteration_number++;
        maxCentroidDelta = calcMaxCentroidsDelta();
    }
    newCentrioidsAsArray = convertLinkedListToArray(newCentroids, k, columnCount);
    freeLinkedList(newCentroids);
    freeLinkedList(prevCentroids);
    freeLinkedList(data);
    return newCentrioidsAsArray; 
}


double** convertLinkedListToArray(struct vector *v, int vectors, int cords){
    int i,j;
    double **res=malloc(vectors*sizeof(double*)) ;
    assertAndReturn(res!=NULL);

    for (i=1;i<=vectors;i++){
        struct cord *currCord= v->cords;
        res[i-1] = calloc (sizeof(double),cords);
        assertAndReturn(res[i-1]!=NULL);

        for (j=1;j<=cords;j++){
            res[i-1][j-1]=currCord->value;
            if(j<cords){
                currCord=currCord->next;
            }

        }
        if(i<vectors){
            v=v->next;
        }
    }
    return res;
}
void setDataFromPython(double **icentroid, double **idata, int ik ,int irowcount, int icolumncount){
    k=ik;
    rowsCount=irowcount;
    columnCount=icolumncount;
    initDataLinkedList(idata);
    initCentroidsLinkedList(icentroid);

}
void initDataLinkedList(double **idata){
    int i,j;
    struct vector *head_vec, *curr_vec, *tempvec, *warningFixVec;
    struct cord *head_cord, *curr_cord, *tempcord,*warningFixCord;
    warningFixVec = malloc(sizeof(struct vector));
    assertAndReturn(warningFixVec!=NULL);
    warningFixCord = malloc(sizeof(struct cord));
    assertAndReturn(warningFixCord!=NULL);
    head_vec=warningFixVec;
    head_cord=warningFixCord;
    curr_vec=warningFixVec;
    curr_cord=warningFixCord;
    for(i=0;i<rowsCount;i++){
        tempvec = malloc(sizeof(struct vector));
        assertAndReturn(tempvec!=NULL);

        tempvec->next = NULL;
        if(i!=0){
            curr_vec->next=tempvec;
        }
        curr_vec= tempvec;
        if(i==0){
            head_vec=curr_vec;
        }
        for(j=0;j<columnCount;j++){
            tempcord= malloc(sizeof(struct cord));
            assertAndReturn(tempcord!=NULL);

            tempcord->next = NULL;
            tempcord->value = idata[i][j];
            if(j!=0){
                curr_cord->next=tempcord;
            }
            curr_cord= tempcord;
            if(j==0){
                head_cord= tempcord;
            }
        }
        if(head_cord!=NULL){
            curr_vec->cords=head_cord;
        }
    }
    if(head_vec!=NULL){
        data=head_vec;
    }
    free(warningFixVec);
    free(warningFixCord);
}
void initCentroidsLinkedList(double **icentroid){
    int i,j;
    struct vector *head_vec, *curr_vec, *tempvec, *warningFixVec;
    struct cord *head_cord, *curr_cord, *tempcord,*warningFixCord;
    warningFixVec = malloc(sizeof(struct vector));
    assertAndReturn(warningFixVec!=NULL);

    warningFixCord = malloc(sizeof(struct cord));
    assertAndReturn(warningFixCord!=NULL);

    head_vec=warningFixVec;
    head_cord=warningFixCord;
    curr_vec=warningFixVec;
    curr_cord=warningFixCord;
    for(i=0;i<k;i++){
        tempvec = malloc(sizeof(struct vector));
        assertAndReturn(tempvec!=NULL);

        tempvec->next = NULL;
        if(i!=0){
            curr_vec->next=tempvec;
        }
        curr_vec= tempvec;
        if(i==0){
            head_vec=curr_vec;
        }
        for(j=0;j<columnCount;j++){
            tempcord= malloc(sizeof(struct cord));
            assertAndReturn(tempcord!=NULL);

            tempcord->next = NULL;
            tempcord->value = icentroid[i][j];
            if(j!=0){
                curr_cord->next=tempcord;
            }
            curr_cord= tempcord;
            if(j==0){
                head_cord= tempcord;
            }
            
        }
        if(head_cord!=NULL){
            curr_vec->cords=head_cord;
        }
    }
    if(head_vec!=NULL){
        newCentroids=head_vec;
    }
    free(warningFixVec);
    free(warningFixCord);
}




void copyFirstKToNew() {   
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    int i=1;

    struct vector *curr_data_vec = data;
    struct cord *curr_data_cord;
    head_vec = malloc(sizeof(struct vector));
    if(!(head_vec!=NULL)){
        errorOccurred = 1;
        return;
    }

    curr_vec = head_vec;
    curr_vec->next = NULL;
    for(;i<=k;i++){
        int j=1;

        curr_data_cord = curr_data_vec->cords;
        head_cord = malloc(sizeof(struct cord));
        if(!(head_cord!=NULL)){
            errorOccurred = 1;
            return;
        }
        curr_cord = head_cord;
        for (;j<=columnCount; j++){
            curr_cord->value = curr_data_cord->value;

            if(j != columnCount) {
                curr_cord->next = malloc(sizeof(struct cord));
                if(!(curr_cord->next!=NULL)){
                    errorOccurred = 1;
                    return;
                }
                curr_cord = curr_cord->next;
            }
            curr_cord->next = NULL;
            curr_data_cord=curr_data_cord->next; 
        }

        curr_data_vec = curr_data_vec->next;
        curr_vec->cords=head_cord;
        if(i!=k){
            curr_vec->next = malloc(sizeof(struct vector));
            if(!(curr_vec->next!=NULL)){
                errorOccurred = 1;
                return;
            }
            curr_vec = curr_vec->next;
        }
        curr_vec->next=NULL;

    }
    newCentroids = head_vec;

}

double calcMaxCentroidsDelta() {

    struct vector *currNewCentroid = newCentroids;
    struct vector *currPrevCentroid = prevCentroids;
    double maxCentroidDelta;
    int i =1;

    maxCentroidDelta = d(*currNewCentroid->cords,*currPrevCentroid->cords);
    if(!(maxCentroidDelta>=0)){
        errorOccurred = 1;
        return -1;
    }

    for(;i<k;i++){
        double dis;
        currNewCentroid = currNewCentroid->next;
        currPrevCentroid = currPrevCentroid->next;
        dis =  d(*currNewCentroid->cords,*currPrevCentroid->cords);
        if(!(dis>=0)){
            errorOccurred = 1;
            return -1;
        }

        if(dis > maxCentroidDelta)
            maxCentroidDelta = dis;
    }
    return maxCentroidDelta;

}

void setZerosToNewCentroids(){
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    int i=1;

    head_vec= malloc(sizeof (struct vector));
    assertAndReturn(head_vec!=NULL);

    curr_vec = head_vec;
    for(;i<=k;i++) {
        int j=1;

        head_cord = malloc(sizeof (struct cord));
        assertAndReturn(head_cord!=NULL);

        curr_cord = head_cord;
        curr_vec->cords=head_cord;
        for (;j<=columnCount; j++) {
            curr_cord->value=0;
            if(j != columnCount){
                curr_cord->next = malloc(sizeof(struct cord));
                assertAndReturn(curr_cord->next!=NULL);

                curr_cord = curr_cord->next;
            }
            curr_cord->next=NULL;
        }
        if(i!= k){
            curr_vec->next= malloc(sizeof (struct vector));
            assertAndReturn(curr_vec->next!=NULL);

             if(!(curr_vec!=NULL)){
                errorOccurred = 1;
                return;
            }
            curr_vec=curr_vec->next;
        }
        curr_vec->next = NULL;
    }
    newCentroids = head_vec;
}
double d(struct cord v1,struct cord v2){
    double sum = 0;
    int i =1;
    for(;i<=columnCount; i++){
        sum += pow(v1.value-v2.value,2);
        if(i!=columnCount) {
            v1 = *v1.next;
            v2 = *v2.next;
        }
    }
    return sqrt(sum);
}
void freeLinkedList(struct vector *vec){
    if(vec!= NULL){

        freeLinkedList(vec->next);

        freeCordsList (vec->cords); 

        free(vec);
    }
}
void freeCordsList(struct cord *cord){
    if(cord != NULL){
        freeCordsList(cord->next);
        free(cord);
    }
}