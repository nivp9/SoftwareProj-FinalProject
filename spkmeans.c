#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define EPSILON 0.001
#include <ctype.h>
#define assertAndReturn(cond)                   \
        if (!(cond)) {                          \
            printf("An Error Has Occurred");    \
            exit(1);                           \
        }                                       \
string goal, file;
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


int main(int argc, char *argv[] )
{
    if(argc !=3){
        printf("An Error Has Occurred");
        return 0;
    }
    goal = argv[1];
    file = argv[2];
}
