/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 * 
 * Student: Qi Liu (519021910529, purewhite@sjtu.edu.cn)
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


#define BLOCK_SIZE 23
// 259/300, 1203/1300, 1928/2000
void transpose_submit(int M, int N, int A[N][M], int B[M][N]){
    register int ii, jj, i, j, c0, c1, c2, c3, c4, c5, c6, c7;
    if(M == 32 && N == 32){
        for(ii = 0; ii < 4; ++ii){
            for(jj = 0; jj < 4; ++jj){
                if(ii == jj){
                    for(i = 0; i < 8; ++i){
                        c0 = A[ii*8+i][jj*8+0];
                        c1 = A[ii*8+i][jj*8+1];
                        c2 = A[ii*8+i][jj*8+2];
                        c3 = A[ii*8+i][jj*8+3];
                        c4 = A[ii*8+i][jj*8+4];
                        c5 = A[ii*8+i][jj*8+5];
                        c6 = A[ii*8+i][jj*8+6];
                        c7 = A[ii*8+i][jj*8+7];
                        B[ii*8+i][jj*8+0] = c0;
                        B[ii*8+i][jj*8+1] = c1;
                        B[ii*8+i][jj*8+2] = c2;
                        B[ii*8+i][jj*8+3] = c3;
                        B[ii*8+i][jj*8+4] = c4;
                        B[ii*8+i][jj*8+5] = c5;
                        B[ii*8+i][jj*8+6] = c6;
                        B[ii*8+i][jj*8+7] = c7;
                    }
                    for(i = 0; i < 8; ++i)
                        for(j = 0; j < i; ++j){
                            c0 = B[ii*8+i][jj*8+j];
                            B[ii*8+i][jj*8+j] = B[jj*8+j][ii*8+i];
                            B[jj*8+j][ii*8+i] = c0;
                        }
                }
                else
                    for(i = 0; i < 8; ++i)
                        for(j = 0; j < 8; ++j)
                            B[jj*8+i][ii*8+j] = A[ii*8+j][jj*8+i];
                
            }
        }
    }

    else if(M == 64 && N == 64){
        for(ii = 0; ii < N/8; ++ii){
            for(jj = 0; jj < M/8; ++jj){
                if(ii == jj){
                    for(i = 0; i < 4; ++i){
                        for(j = 0; j < 4; ++j){
                            if(i % 4 == j % 4){
                                if(i % 4 != 0)
                                    B[jj*8+j-1][ii*8+i-1] = c0;
                                c0 = A[ii*8+i][jj*8+j];
                                continue;
                            }
                            B[jj*8+j][ii*8+i] = A[ii*8+i][jj*8+j];
                        }
                    }
                    for(i = 4; i < 8; ++i){
                        for(j = 0; j < 4; ++j){
                            if(i % 4 == j % 4){
                                if(i % 4 != 0)
                                    B[jj*8+j-1][ii*8+i-1] = c1;
                                c1 = A[ii*8+i][jj*8+j];
                                continue;
                            }
                            B[jj*8+j][ii*8+i] = A[ii*8+i][jj*8+j];
                        }
                    }
                    B[jj*8+3][ii*8+3] = c0;
                    B[jj*8+3][ii*8+7] = c1;
                    for(i = 4; i < 8; ++i){
                        for(j = 4; j < 8; ++j){
                            if(i % 4 == j % 4){
                                if(i % 4 != 0)
                                    B[jj*8+j-1][ii*8+i-1] = c0;
                                c0 = A[ii*8+i][jj*8+j];
                                continue;
                            }
                            B[jj*8+j][ii*8+i] = A[ii*8+i][jj*8+j];
                        }
                    }
                    for(i = 0; i < 4; ++i){
                        for(j = 4; j < 8; ++j){
                            if(i % 4 == j % 4){
                                if(i % 4 != 0)
                                    B[jj*8+j-1][ii*8+i-1] = c1;
                                c1 = A[ii*8+i][jj*8+j];
                                continue;
                            }
                            B[jj*8+j][ii*8+i] = A[ii*8+i][jj*8+j];
                        }
                    }
                    B[jj*8+7][ii*8+3] = c1;
                    B[jj*8+7][ii*8+7] = c0;
                }
                else{
                    for(i = 0; i < 4; ++i){
                        for(j = 0; j < 4; ++j){
                            B[jj*8+j][ii*8+i] = A[ii*8+i][jj*8+j];
                        }
                    }
                    for(i = 0; i < 4; ++i)                          // trans(A[0~3][4~7]) -> B[0~3][4~7]
                        for(j = 4; j < 8; ++j)
                            B[jj*8+j-4][ii*8+i+4] = A[ii*8+i][jj*8+j];

                    for(j = 0; j < 4; ++j){                         // B[0~3][4~7] -> B[4~7][0~3]
                        c4 = B[jj*8+j][ii*8+4];
                        c5 = B[jj*8+j][ii*8+5];
                        c6 = B[jj*8+j][ii*8+6];
                        c7 = B[jj*8+j][ii*8+7];

                        for(i = 4; i < 8; ++i)
                            B[jj*8+j][ii*8+i] = A[ii*8+i][jj*8+j];

                        B[jj*8+j+4][ii*8+0] = c4;
                        B[jj*8+j+4][ii*8+1] = c5;
                        B[jj*8+j+4][ii*8+2] = c6;
                        B[jj*8+j+4][ii*8+3] = c7;
                    }
                    for(i = 4; i < 8; ++i){
                        for(j = 4; j < 8; ++j){
                            B[jj*8+j][ii*8+i] = A[ii*8+i][jj*8+j];
                        }
                    }
                }
            }
        }
    }

    else{
        // Original Block-transposition Code
        //
        // for(ii = 0; ii < N/BLOCK_SIZE; ++ii){
        //     for(jj = 0; jj < M/BLOCK_SIZE; ++jj){
        //         for(i = 0; i < BLOCK_SIZE; ++i)
        //             for(j = 0; j < BLOCK_SIZE; ++j)
        //                 B[jj*BLOCK_SIZE+j][ii*BLOCK_SIZE+i] = A[ii*BLOCK_SIZE+i][jj*BLOCK_SIZE+j];
        //     }
        //     for(j = jj*BLOCK_SIZE; j < M; ++j){
        //         for(i = 0; i < BLOCK_SIZE; ++i)
        //             B[j][ii*BLOCK_SIZE+i] = A[ii*BLOCK_SIZE+i][j];
        //     }
        // }
        // for(i = ii*BLOCK_SIZE; i < N; ++i){
        //     for(jj = 0; jj < M/BLOCK_SIZE; ++jj){
        //         for(j = 0; j < BLOCK_SIZE; ++j)
        //             B[jj*BLOCK_SIZE+j][i] = A[i][jj*BLOCK_SIZE+j];
        //     }
        //     for(j = jj*BLOCK_SIZE; j < M; ++j){
        //         B[j][i] = A[i][j];
        //     }
        // }
        for(ii = 0; ii <= N/BLOCK_SIZE; ++ii)
            for(jj = 0; jj <= M/BLOCK_SIZE; ++jj)
                for(i = 0; i < BLOCK_SIZE && ii*BLOCK_SIZE+i < N ; ++i)
                    for(j = 0; j < BLOCK_SIZE && jj*BLOCK_SIZE+j < M; ++j)
                        B[jj*BLOCK_SIZE+j][ii*BLOCK_SIZE+i] = A[ii*BLOCK_SIZE+i][jj*BLOCK_SIZE+j];
    }
}

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

// // STRIDE 8, 343, 4691, 2197
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
// void transpose_submit(int M, int N, int A[N][M], int B[M][N]){  
//     register int ii, jj, i, j, k, l, STRIDE = 8;
//     if(M == 64 && N == 64) STRIDE = 4;
//     for(ii = 0; ii < N/STRIDE; ++ii){
//         for(jj = 0; jj < M/STRIDE; ++jj){
//             for(k = 0; k < STRIDE; ++k)
//                 for(l = 0; l < STRIDE; ++l)
//                     B[jj*STRIDE+k][ii*STRIDE+l] = A[ii*STRIDE+l][jj*STRIDE+k];
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

// 287/300, 1619/1300, 2073/2000
// void transpose_submit(int M, int N, int A[N][M], int B[M][N]){  
//     register int ii, jj, i, j, c0, c1, c2, c3, c4, c5, c6, c7;
//     for(ii = 0; ii < N/8; ++ii){
//         for(jj = 0; jj < M/8; ++jj){
//             if(M == 64 && N == 64){
//                 for(i = 0; i < 4; ++i)
//                     for(j = 0; j < 4; ++j)
//                         B[jj*8+j][ii*8+i] = A[ii*8+i][jj*8+j];
//                 for(i = 4; i < 8; ++i)
//                     for(j = 0; j < 4; ++j)
//                         B[jj*8+j][ii*8+i] = A[ii*8+i][jj*8+j];
//                 for(i = 4; i < 8; ++i)
//                     for(j = 4; j < 8; ++j)
//                         B[jj*8+j][ii*8+i] = A[ii*8+i][jj*8+j];
//                 for(i = 0; i < 4; ++i)
//                     for(j = 4; j < 8; ++j)
//                         B[jj*8+j][ii*8+i] = A[ii*8+i][jj*8+j];
//             }
//             else{
//                 c0 = A[ii*8+0][jj*8+0];
//                 c1 = A[ii*8+0][jj*8+1];
//                 c2 = A[ii*8+0][jj*8+2];
//                 c3 = A[ii*8+0][jj*8+3];
//                 c4 = A[ii*8+0][jj*8+4];
//                 c5 = A[ii*8+0][jj*8+5];
//                 c6 = A[ii*8+0][jj*8+6];
//                 c7 = A[ii*8+0][jj*8+7];
//                 B[jj*8+0][ii*8+0] = c0;
//                 B[jj*8+1][ii*8+0] = c1;
//                 B[jj*8+2][ii*8+0] = c2;
//                 B[jj*8+3][ii*8+0] = c3;
//                 B[jj*8+4][ii*8+0] = c4;
//                 B[jj*8+5][ii*8+0] = c5;
//                 B[jj*8+6][ii*8+0] = c6;
//                 B[jj*8+7][ii*8+0] = c7;
//                 c0 = A[ii*8+1][jj*8+0];
//                 c1 = A[ii*8+1][jj*8+1];
//                 c2 = A[ii*8+1][jj*8+2];
//                 c3 = A[ii*8+1][jj*8+3];
//                 c4 = A[ii*8+1][jj*8+4];
//                 c5 = A[ii*8+1][jj*8+5];
//                 c6 = A[ii*8+1][jj*8+6];
//                 c7 = A[ii*8+1][jj*8+7];
//                 B[jj*8+0][ii*8+1] = c0;
//                 B[jj*8+1][ii*8+1] = c1;
//                 B[jj*8+2][ii*8+1] = c2;
//                 B[jj*8+3][ii*8+1] = c3;
//                 B[jj*8+4][ii*8+1] = c4;
//                 B[jj*8+5][ii*8+1] = c5;
//                 B[jj*8+6][ii*8+1] = c6;
//                 B[jj*8+7][ii*8+1] = c7;
//                 c0 = A[ii*8+2][jj*8+0];
//                 c1 = A[ii*8+2][jj*8+1];
//                 c2 = A[ii*8+2][jj*8+2];
//                 c3 = A[ii*8+2][jj*8+3];
//                 c4 = A[ii*8+2][jj*8+4];
//                 c5 = A[ii*8+2][jj*8+5];
//                 c6 = A[ii*8+2][jj*8+6];
//                 c7 = A[ii*8+2][jj*8+7];
//                 B[jj*8+0][ii*8+2] = c0;
//                 B[jj*8+1][ii*8+2] = c1;
//                 B[jj*8+2][ii*8+2] = c2;
//                 B[jj*8+3][ii*8+2] = c3;
//                 B[jj*8+4][ii*8+2] = c4;
//                 B[jj*8+5][ii*8+2] = c5;
//                 B[jj*8+6][ii*8+2] = c6;
//                 B[jj*8+7][ii*8+2] = c7;
//                 c0 = A[ii*8+3][jj*8+0];
//                 c1 = A[ii*8+3][jj*8+1];
//                 c2 = A[ii*8+3][jj*8+2];
//                 c3 = A[ii*8+3][jj*8+3];
//                 c4 = A[ii*8+3][jj*8+4];
//                 c5 = A[ii*8+3][jj*8+5];
//                 c6 = A[ii*8+3][jj*8+6];
//                 c7 = A[ii*8+3][jj*8+7];
//                 B[jj*8+0][ii*8+3] = c0;
//                 B[jj*8+1][ii*8+3] = c1;
//                 B[jj*8+2][ii*8+3] = c2;
//                 B[jj*8+3][ii*8+3] = c3;
//                 B[jj*8+4][ii*8+3] = c4;
//                 B[jj*8+5][ii*8+3] = c5;
//                 B[jj*8+6][ii*8+3] = c6;
//                 B[jj*8+7][ii*8+3] = c7;
//                 c0 = A[ii*8+4][jj*8+0];
//                 c1 = A[ii*8+4][jj*8+1];
//                 c2 = A[ii*8+4][jj*8+2];
//                 c3 = A[ii*8+4][jj*8+3];
//                 c4 = A[ii*8+4][jj*8+4];
//                 c5 = A[ii*8+4][jj*8+5];
//                 c6 = A[ii*8+4][jj*8+6];
//                 c7 = A[ii*8+4][jj*8+7];
//                 B[jj*8+0][ii*8+4] = c0;
//                 B[jj*8+1][ii*8+4] = c1;
//                 B[jj*8+2][ii*8+4] = c2;
//                 B[jj*8+3][ii*8+4] = c3;
//                 B[jj*8+4][ii*8+4] = c4;
//                 B[jj*8+5][ii*8+4] = c5;
//                 B[jj*8+6][ii*8+4] = c6;
//                 B[jj*8+7][ii*8+4] = c7;
//                 c0 = A[ii*8+5][jj*8+0];
//                 c1 = A[ii*8+5][jj*8+1];
//                 c2 = A[ii*8+5][jj*8+2];
//                 c3 = A[ii*8+5][jj*8+3];
//                 c4 = A[ii*8+5][jj*8+4];
//                 c5 = A[ii*8+5][jj*8+5];
//                 c6 = A[ii*8+5][jj*8+6];
//                 c7 = A[ii*8+5][jj*8+7];
//                 B[jj*8+0][ii*8+5] = c0;
//                 B[jj*8+1][ii*8+5] = c1;
//                 B[jj*8+2][ii*8+5] = c2;
//                 B[jj*8+3][ii*8+5] = c3;
//                 B[jj*8+4][ii*8+5] = c4;
//                 B[jj*8+5][ii*8+5] = c5;
//                 B[jj*8+6][ii*8+5] = c6;
//                 B[jj*8+7][ii*8+5] = c7;
//                 c0 = A[ii*8+6][jj*8+0];
//                 c1 = A[ii*8+6][jj*8+1];
//                 c2 = A[ii*8+6][jj*8+2];
//                 c3 = A[ii*8+6][jj*8+3];
//                 c4 = A[ii*8+6][jj*8+4];
//                 c5 = A[ii*8+6][jj*8+5];
//                 c6 = A[ii*8+6][jj*8+6];
//                 c7 = A[ii*8+6][jj*8+7];
//                 B[jj*8+0][ii*8+6] = c0;
//                 B[jj*8+1][ii*8+6] = c1;
//                 B[jj*8+2][ii*8+6] = c2;
//                 B[jj*8+3][ii*8+6] = c3;
//                 B[jj*8+4][ii*8+6] = c4;
//                 B[jj*8+5][ii*8+6] = c5;
//                 B[jj*8+6][ii*8+6] = c6;
//                 B[jj*8+7][ii*8+6] = c7;
//                 c0 = A[ii*8+7][jj*8+0];
//                 c1 = A[ii*8+7][jj*8+1];
//                 c2 = A[ii*8+7][jj*8+2];
//                 c3 = A[ii*8+7][jj*8+3];
//                 c4 = A[ii*8+7][jj*8+4];
//                 c5 = A[ii*8+7][jj*8+5];
//                 c6 = A[ii*8+7][jj*8+6];
//                 c7 = A[ii*8+7][jj*8+7];
//                 B[jj*8+0][ii*8+7] = c0;
//                 B[jj*8+1][ii*8+7] = c1;
//                 B[jj*8+2][ii*8+7] = c2;
//                 B[jj*8+3][ii*8+7] = c3;
//                 B[jj*8+4][ii*8+7] = c4;
//                 B[jj*8+5][ii*8+7] = c5;
//                 B[jj*8+6][ii*8+7] = c6;
//                 B[jj*8+7][ii*8+7] = c7;
//             }
//         }
//         for(j = jj*8; j < M; ++j){
//             c0 = A[ii*8+0][j];
//             c1 = A[ii*8+1][j];
//             c2 = A[ii*8+2][j];
//             c3 = A[ii*8+3][j];
//             c4 = A[ii*8+4][j];
//             c5 = A[ii*8+5][j];
//             c6 = A[ii*8+6][j];
//             c7 = A[ii*8+7][j];
//             B[j][ii*8+0] = c0;
//             B[j][ii*8+1] = c1;
//             B[j][ii*8+2] = c2;
//             B[j][ii*8+3] = c3;
//             B[j][ii*8+4] = c4;
//             B[j][ii*8+5] = c5;
//             B[j][ii*8+6] = c6;
//             B[j][ii*8+7] = c7;
//         }
//     }
//     for(i = ii*8; i < N; ++i){
//         for(jj = 0; jj < M/8; ++jj){
//             c0 = A[i][jj*8+0];
//             c1 = A[i][jj*8+1];
//             c2 = A[i][jj*8+2];
//             c3 = A[i][jj*8+3];
//             c4 = A[i][jj*8+4];
//             c5 = A[i][jj*8+5];
//             c6 = A[i][jj*8+6];
//             c7 = A[i][jj*8+7];
//             B[jj*8+0][i] = c0;
//             B[jj*8+1][i] = c1;
//             B[jj*8+2][i] = c2;
//             B[jj*8+3][i] = c3;
//             B[jj*8+4][i] = c4;
//             B[jj*8+5][i] = c5;
//             B[jj*8+6][i] = c6;
//             B[jj*8+7][i] = c7;
//         }
//         for(j = jj*8; j < M; ++j){
//             B[j][i] = A[i][j];
//         }
//     }
// }


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

