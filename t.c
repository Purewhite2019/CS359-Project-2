#include <stdio.h>
#include <stdlib.h>
#define M 64
#define N 64

void trans(int A[N][M], int B[M][N]);
void transpose_submit(int A[N][M], int B[M][N]);

int main(int argc, char *argv[]){
    int **A = (int**)malloc(N*sizeof(int*)), **B = (int**)malloc(M*sizeof(int*));
    printf("A:\n");
    for(int i = 0; i < N; ++i){
        A[i] = (int*)malloc(M*sizeof(int));
        for(int j = 0; j < M; ++j){
            A[i][j] = M*i+j;
            // printf("%d\t", A[i][j]);
        }
        // putchar('\n');
    }
    
    for(int i = 0; i < M; ++i)
        B[i] = (int*)malloc(N*sizeof(int));

    // trans(A, B);
    int ii, jj, i, j, k, l;
    for(ii = 0; ii < N/8; ++ii){
        printf("ii = %d\n", ii);
        for(jj = 0; jj < M / 8; ++jj){
            for(k = 0; k < 8; ++k)
                for(l = 0; l < 8; ++l)
                    B[jj*8+l][ii*8+k] = A[ii*8+k][jj*8+l];
            printf("\tjj = %d\n", jj);
        }
        for(j = jj*8; j < M; ++j){
            for(k = 0; k < 8; ++k)
                B[j][ii*8+k] = A[ii*8+k][j];
        }
    }
    for(i = ii*8; i < N; ++i){
        for(jj = 0; jj < M / 8; ++jj){
            for(l = 0; l < 8; ++l)
                B[jj*8+l][i] = A[i][jj*8+l];
            printf("jj = %d\n", jj);
        }
        for(j = jj*8; j < M; ++j){
            B[j][i] = A[i][j];
        }
    }


    // printf("B:\n");
    // for(int i = 0; i < M; ++i){
    //     for(int j = 0; j < N; ++j){
    //         printf("%d\t", B[i][j]);
    //         fflush(stdout);
    //     }
    //     putchar('\n');
    // }
    return 0;
}

void transpose_submit(int A[N][M], int B[M][N]){
    int ii, jj, i, j, k, l;
    for(ii = 0; ii < N/8; ++ii){
        for(jj = 0; jj < M / 8; ++jj){
            for(k = 0; k < 8; ++k)
                for(l = 0; l < 8; ++l)
                    B[jj*8+l][ii*8+k] = A[ii*8+k][jj*8+l];
        }
        for(j = jj*8; j < M; ++j){
            for(k = 0; k < 8; ++k)
                B[j][ii*8+k] = A[ii*8+k][j];
        }
    }
    for(i = ii*8; i < N; ++i){
        for(jj = 0; jj < M / 8; ++jj){
            for(l = 0; l < 8; ++l)
                B[jj*8+l][i] = A[i][jj*8+l];
        }
        for(j = jj*8; j < M; ++j){
            B[j][i] = A[i][j];
        }
    }
}

void trans(int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            printf("%d\t", tmp);
            B[j][i] = tmp;
        }
        putchar('\n');
    }    

}