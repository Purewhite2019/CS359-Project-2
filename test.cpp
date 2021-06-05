#include <iostream>
using namespace std;

int A[16][16], B[16][16];

#define BLOCK_SIZE 23
// 287/300, 1427/1300, 2073/2000    1427 = 3(init) + 8 * 38(ii==jj) + 56 * 2*10(ii!=jj)
void transpose_submit(int M, int N, int A[][16], int B[][16]){
    register int ii, jj, i, j, c0, c1, c2, c3, c4, c5, c6, c7;
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


int main(){
    for(int i = 0; i < 16; ++i)
        for(int j = 0; j < 16; ++j)
            A[i][j] = 100*i+j;
    for(int i = 0; i < 16; ++i){
        for(int j = 0; j < 16; ++j)
            cout << A[i][j] << ' ';
        cout << endl;
    }
    cout << "---------------" << endl;
    transpose_submit(16, 16, A, B);
    for(int i = 0; i < 16; ++i){
        for(int j = 0; j < 16; ++j)
            cout << B[i][j] << ' ';
        cout << endl;
    }
    return 0;
}