#include <cstdio>

#define THRESHOLD 4

int main(){
    int i, j;

    // for(int k = 0; k < THRESHOLD; ++k){
    //     for(int l = 0; l < THRESHOLD; ++l)
    //         printf("c%d = A[ii*THRESHOLD+%d][jj*THRESHOLD+%d];\n", l, k, l);
    //     for(int l = 0; l < THRESHOLD; ++l)
    //         printf("B[jj*THRESHOLD+%d][ii*THRESHOLD+%d] = c%d;\n", l, k, l);
    // }
    // putchar('\n');
    
    for(i = 0; i < 4; ++i){
        for(j = 0; j < 4; ++j)
            printf("c%d = A[ii*8+%d][jj*8+%d];\n", j, i, j);
        for(j = 0; j < 4; ++j)
            printf("B[jj*8+%d][ii*8+%d] = c%d;\n", j, i, j);
    }
    for(i = 4; i < 8; ++i){
        for(j = 0; j < 4; ++j)
            printf("c%d = A[ii*8+%d][jj*8+%d];\n", j, i, j);
        for(j = 0; j < 4; ++j)
            printf("B[jj*8+%d][ii*8+%d] = c%d;\n", j, i, j);
    }
    for(i = 4; i < 8; ++i){
        for(j = 4; j < 8; ++j)
            printf("c%d = A[ii*8+%d][jj*8+%d];\n", j, i, j);
        for(j = 4; j < 8; ++j)
            printf("B[jj*8+%d][ii*8+%d] = c%d;\n", j, i, j);
    }
    for(i = 0; i < 4; ++i){
        for(j = 4; j < 8; ++j)
            printf("c%d = A[ii*8+%d][jj*8+%d];\n", j, i, j);
        for(j = 4; j < 8; ++j)
            printf("B[jj*8+%d][ii*8+%d] = c%d;\n", j, i, j);
    }

    putchar('\n');

    // for(i = 0; i < 4; ++i)
    //     for(j = 0; j < 4; ++j)
    //         printf("B[jj*8+%d][ii*8+%d] = A[ii*8+%d][jj*8+%d];\n", j, j, i, j);
    // putchar('\n');
    // for(i = 4; i < 8; ++i)
    //     for(j = 0; j < 4; ++j)
    //         printf("B[jj*8+%d][ii*8+%d] = A[ii*8+%d][jj*8+%d];\n", j, j, i, j);
    // putchar('\n');
    // for(i = 4; i < 8; ++i)
    //     for(j = 4; j < 8; ++j)
    //         printf("B[jj*8+%d][ii*8+%d] = A[ii*8+%d][jj*8+%d];\n", j, j, i, j);
    // putchar('\n');
    // for(i = 0; i < 4; ++i)
    //     for(j = 4; j < 8; ++j)
    //         printf("B[jj*8+%d][ii*8+%d] = A[ii*8+%d][jj*8+%d];\n", j, j, i, j);
    // putchar('\n');



    for(int k = 0; k < THRESHOLD; ++k)
        printf("c%d = A[ii*THRESHOLD+%d][j];\n", k, k);
    for(int k = 0; k < THRESHOLD; ++k)
        printf("B[j][ii*THRESHOLD+%d] = c%d;\n", k, k);
    putchar('\n');
    for(int l = 0; l < THRESHOLD; ++l)
        printf("c%d = A[i][jj*THRESHOLD+%d];\n", l, l);
    for(int l = 0; l < THRESHOLD; ++l)
        printf("B[jj*THRESHOLD+%d][i] = c%d;\n", l, l);
    return 0;
}