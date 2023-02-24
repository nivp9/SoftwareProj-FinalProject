#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include "spkmeans.h"
#include "spkmeanslogic.c"
 


#define EPSILON 0.001
#define assertAndReturn(cond)                   \
        if (!(cond)) {                          \
            printf("An Error Has Occurred");    \
            exit(1);                           \
        }                                       \



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


double** convertLinkedListToArray(struct vector *v, int vectors, int cords);
void freeLinkedList(struct vector *vec);
void freeCordsList(struct cord *cord);

char *goal, *file;
int rowsCount=0;
int columnCount=0;
double **data =NULL;

int main(int argc, char *argv[] )
{
    // if(argc !=3){
    //     printf("An Error Has Occurred");
    //     return 0;
    // }
    goal ="wam";// argv[1];
    file = "tests/tests/input_1.txt";//argv[2];
    prepData(file);
    if(strcmp(goal, "wam")==0){
        if(rowsCount!= columnCount){
            printf("An Error Has Occurred");
            return 0;
        }
        double **wamRes = wam(data,rowsCount);
        printMatrix(wamRes, columnCount, columnCount);
    }
}










void prepData(char *filename){
    FILE *f = fopen(filename, "r");
    assertAndReturn(f!=NULL);
    double n;
    char c;
    int columnsCountInitialized=0;

    struct vector *head_vec, *curr_vec,*prev_vec;
    struct cord *head_cord, *curr_cord;


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
    prev_vec->next = NULL;
    free(head_cord); 
    free(curr_vec); 
    fclose(f);
    data = convertLinkedListToArray(head_vec, rowsCount,columnCount);
    freeLinkedList(head_vec);

}



double** convertLinkedListToArray(struct vector *v, int vectors, int cords){
    int i,j;
    double **res=malloc(vectors*sizeof(double*)) ;
    assertAndReturn(res!=NULL);

    for (i=1;i<=vectors;i++){
        struct cord *currCord= v->cords;
        res[i-1] = malloc(cords*sizeof(double));
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
void freeCordsList(struct cord *cord){
    if(cord != NULL){
        freeCordsList(cord->next);
        free(cord);
    }
}
void freeLinkedList(struct vector *vec){
    if(vec!= NULL){

        freeLinkedList(vec->next);

        freeCordsList (vec->cords); 

        free(vec);
    }
}



void printMatrix(double ** d, int n, int m ){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            
            printf("%0.4f  ,",d[i][j]);
        }
        printf("\n");
    }
}