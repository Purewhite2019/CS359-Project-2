/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";

// 439, 1699, 2411
// void transpose_submit(int M, int N, int A[N][M], int B[M][N]){  
//     register int ii, jj, i, j, k, t0, t1, t2, t3;
//     for(ii = 0; ii < N/4; ++ii){
//         for(jj = 0; jj < M/4; ++jj){
//             // for(k = 0; k < 4; ++k)
//             //     for(l = 0; l < 4; ++l)
//             //         B[jj*4+l][ii*4+k] = A[ii*4+k][jj*4+l];
//             for(k = 0; k < 4; ++k){
//                 t0 = A[ii*4+k][jj*4];
//                 t1 = A[ii*4+k][jj*4+1];
//                 t2 = A[ii*4+k][jj*4+2];
//                 t3 = A[ii*4+k][jj*4+3];
//                 B[jj*4][ii*4+k] = t0;
//                 B[jj*4+1][ii*4+k] = t1;
//                 B[jj*4+2][ii*4+k] = t2;
//                 B[jj*4+3][ii*4+k] = t3;
//             }
//         }
//         for(j = jj*4; j < M; ++j){
//             // for(k = 0; k < 4; ++k)
//             //     B[j][ii*4+k] = A[ii*4+k][j];
//             t0 = A[ii*4][j];
//             t1 = A[ii*4+1][j];
//             t2 = A[ii*4+2][j];
//             t3 = A[ii*4+3][j];
//             B[j][ii*4] = t0;
//             B[j][ii*4+1] = t1;
//             B[j][ii*4+2] = t2;
//             B[j][ii*4+3] = t3;
//         }
//     }
//     for(i = ii*4; i < N; ++i){
//         for(jj = 0; jj < M/4; ++jj){
//             // for(l = 0; l < 4; ++l)
//             //     B[jj*4+l][i] = A[i][jj*4+l];
//             t0 = A[i][jj*4];
//             t1 = A[i][jj*4+1];
//             t2 = A[i][jj*4+2];
//             t3 = A[i][jj*4+3];
//             B[jj*4][i] = t0;
//             B[jj*4+1][i] = t1;
//             B[jj*4+2][i] = t2;
//             B[jj*4+3][i] = t3;
//         }
//         for(j = jj*4; j < M; ++j){
//             B[j][i] = A[i][j];
//         }
//     }
// }

// STRIDE 8, 343, 4691, 2197
// #define STRIDE 8
// void transpose_submit(int M, int N, int A[N][M], int B[M][N]){  
//     register int ii, jj, i, j, k, l;
//     for(ii = 0; ii < N/STRIDE; ++ii){
//         for(jj = 0; jj < M/STRIDE; ++jj){
//             for(k = 0; k < STRIDE; ++k)
//                 for(l = 0; l < STRIDE; ++l)
//                     B[jj*STRIDE+l][ii*STRIDE+k] = A[ii*STRIDE+k][jj*STRIDE+l];
//         }
//         for(j = jj*STRIDE; j < M; ++j){
//             for(k = 0; k < STRIDE; ++k)
//                 B[j][ii*STRIDE+k] = A[ii*STRIDE+k][j];
//         }
//     }
//     for(i = ii*STRIDE; i < N; ++i){
//         for(jj = 0; jj < M/STRIDE; ++jj){
//             for(l = 0; l < STRIDE; ++l)
//                 B[jj*STRIDE+l][i] = A[i][jj*STRIDE+l];
//         }
//         for(j = jj*STRIDE; j < M; ++j){
//             B[j][i] = A[i][j];
//         }
//     }
// }

// Merge!, 343, 1891, 2197
void transpose_submit(int M, int N, int A[N][M], int B[M][N]){  
    register int ii, jj, i, j, k, l, STRIDE = 8;
    if(M == 64 && N == 64) STRIDE = 4;
    for(ii = 0; ii < N/STRIDE; ++ii){
        for(jj = 0; jj < M/STRIDE; ++jj){
            for(k = 0; k < STRIDE; ++k)
                for(l = 0; l < STRIDE; ++l)
                    B[jj*STRIDE+k][ii*STRIDE+l] = A[ii*STRIDE+l][jj*STRIDE+k];
        }
        for(j = jj*STRIDE; j < M; ++j){
            for(k = 0; k < STRIDE; ++k)
                B[j][ii*STRIDE+k] = A[ii*STRIDE+k][j];
        }
    }
    for(i = ii*STRIDE; i < N; ++i){
        for(jj = 0; jj < M/STRIDE; ++jj){
            for(l = 0; l < STRIDE; ++l)
                B[jj*STRIDE+l][i] = A[i][jj*STRIDE+l];
        }
        for(j = jj*STRIDE; j < M; ++j){
            B[j][i] = A[i][j];
        }
    }
}


/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

