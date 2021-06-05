#include <iostream>
using namespace std;

int A[32][32], B[32][32];

#define BLOCK_SIZE 23
// 287/300, 1427/1300, 2073/2000    1427 = 3(init) + 8 * 38(ii==jj) + 56 * 2*10(ii!=jj)
void transpose_submit(int M, int N, int A[][32], int B[][32]){
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
                else{
                    c0 = A[ii*8+0][jj*8+0];
                    c1 = A[ii*8+0][jj*8+1];
                    c2 = A[ii*8+0][jj*8+2];
                    c3 = A[ii*8+0][jj*8+3];
                    c4 = A[ii*8+0][jj*8+4];
                    c5 = A[ii*8+0][jj*8+5];
                    c6 = A[ii*8+0][jj*8+6];
                    c7 = A[ii*8+0][jj*8+7];
                    B[jj*8+0][ii*8+0] = c0;
                    B[jj*8+1][ii*8+0] = c1;
                    B[jj*8+2][ii*8+0] = c2;
                    B[jj*8+3][ii*8+0] = c3;
                    B[jj*8+4][ii*8+0] = c4;
                    B[jj*8+5][ii*8+0] = c5;
                    B[jj*8+6][ii*8+0] = c6;
                    B[jj*8+7][ii*8+0] = c7;
                    c0 = A[ii*8+1][jj*8+0];
                    c1 = A[ii*8+1][jj*8+1];
                    c2 = A[ii*8+1][jj*8+2];
                    c3 = A[ii*8+1][jj*8+3];
                    c4 = A[ii*8+1][jj*8+4];
                    c5 = A[ii*8+1][jj*8+5];
                    c6 = A[ii*8+1][jj*8+6];
                    c7 = A[ii*8+1][jj*8+7];
                    B[jj*8+0][ii*8+1] = c0;
                    B[jj*8+1][ii*8+1] = c1;
                    B[jj*8+2][ii*8+1] = c2;
                    B[jj*8+3][ii*8+1] = c3;
                    B[jj*8+4][ii*8+1] = c4;
                    B[jj*8+5][ii*8+1] = c5;
                    B[jj*8+6][ii*8+1] = c6;
                    B[jj*8+7][ii*8+1] = c7;
                    c0 = A[ii*8+2][jj*8+0];
                    c1 = A[ii*8+2][jj*8+1];
                    c2 = A[ii*8+2][jj*8+2];
                    c3 = A[ii*8+2][jj*8+3];
                    c4 = A[ii*8+2][jj*8+4];
                    c5 = A[ii*8+2][jj*8+5];
                    c6 = A[ii*8+2][jj*8+6];
                    c7 = A[ii*8+2][jj*8+7];
                    B[jj*8+0][ii*8+2] = c0;
                    B[jj*8+1][ii*8+2] = c1;
                    B[jj*8+2][ii*8+2] = c2;
                    B[jj*8+3][ii*8+2] = c3;
                    B[jj*8+4][ii*8+2] = c4;
                    B[jj*8+5][ii*8+2] = c5;
                    B[jj*8+6][ii*8+2] = c6;
                    B[jj*8+7][ii*8+2] = c7;
                    c0 = A[ii*8+3][jj*8+0];
                    c1 = A[ii*8+3][jj*8+1];
                    c2 = A[ii*8+3][jj*8+2];
                    c3 = A[ii*8+3][jj*8+3];
                    c4 = A[ii*8+3][jj*8+4];
                    c5 = A[ii*8+3][jj*8+5];
                    c6 = A[ii*8+3][jj*8+6];
                    c7 = A[ii*8+3][jj*8+7];
                    B[jj*8+0][ii*8+3] = c0;
                    B[jj*8+1][ii*8+3] = c1;
                    B[jj*8+2][ii*8+3] = c2;
                    B[jj*8+3][ii*8+3] = c3;
                    B[jj*8+4][ii*8+3] = c4;
                    B[jj*8+5][ii*8+3] = c5;
                    B[jj*8+6][ii*8+3] = c6;
                    B[jj*8+7][ii*8+3] = c7;
                    c0 = A[ii*8+4][jj*8+0];
                    c1 = A[ii*8+4][jj*8+1];
                    c2 = A[ii*8+4][jj*8+2];
                    c3 = A[ii*8+4][jj*8+3];
                    c4 = A[ii*8+4][jj*8+4];
                    c5 = A[ii*8+4][jj*8+5];
                    c6 = A[ii*8+4][jj*8+6];
                    c7 = A[ii*8+4][jj*8+7];
                    B[jj*8+0][ii*8+4] = c0;
                    B[jj*8+1][ii*8+4] = c1;
                    B[jj*8+2][ii*8+4] = c2;
                    B[jj*8+3][ii*8+4] = c3;
                    B[jj*8+4][ii*8+4] = c4;
                    B[jj*8+5][ii*8+4] = c5;
                    B[jj*8+6][ii*8+4] = c6;
                    B[jj*8+7][ii*8+4] = c7;
                    c0 = A[ii*8+5][jj*8+0];
                    c1 = A[ii*8+5][jj*8+1];
                    c2 = A[ii*8+5][jj*8+2];
                    c3 = A[ii*8+5][jj*8+3];
                    c4 = A[ii*8+5][jj*8+4];
                    c5 = A[ii*8+5][jj*8+5];
                    c6 = A[ii*8+5][jj*8+6];
                    c7 = A[ii*8+5][jj*8+7];
                    B[jj*8+0][ii*8+5] = c0;
                    B[jj*8+1][ii*8+5] = c1;
                    B[jj*8+2][ii*8+5] = c2;
                    B[jj*8+3][ii*8+5] = c3;
                    B[jj*8+4][ii*8+5] = c4;
                    B[jj*8+5][ii*8+5] = c5;
                    B[jj*8+6][ii*8+5] = c6;
                    B[jj*8+7][ii*8+5] = c7;
                    c0 = A[ii*8+6][jj*8+0];
                    c1 = A[ii*8+6][jj*8+1];
                    c2 = A[ii*8+6][jj*8+2];
                    c3 = A[ii*8+6][jj*8+3];
                    c4 = A[ii*8+6][jj*8+4];
                    c5 = A[ii*8+6][jj*8+5];
                    c6 = A[ii*8+6][jj*8+6];
                    c7 = A[ii*8+6][jj*8+7];
                    B[jj*8+0][ii*8+6] = c0;
                    B[jj*8+1][ii*8+6] = c1;
                    B[jj*8+2][ii*8+6] = c2;
                    B[jj*8+3][ii*8+6] = c3;
                    B[jj*8+4][ii*8+6] = c4;
                    B[jj*8+5][ii*8+6] = c5;
                    B[jj*8+6][ii*8+6] = c6;
                    B[jj*8+7][ii*8+6] = c7;
                    c0 = A[ii*8+7][jj*8+0];
                    c1 = A[ii*8+7][jj*8+1];
                    c2 = A[ii*8+7][jj*8+2];
                    c3 = A[ii*8+7][jj*8+3];
                    c4 = A[ii*8+7][jj*8+4];
                    c5 = A[ii*8+7][jj*8+5];
                    c6 = A[ii*8+7][jj*8+6];
                    c7 = A[ii*8+7][jj*8+7];
                    B[jj*8+0][ii*8+7] = c0;
                    B[jj*8+1][ii*8+7] = c1;
                    B[jj*8+2][ii*8+7] = c2;
                    B[jj*8+3][ii*8+7] = c3;
                    B[jj*8+4][ii*8+7] = c4;
                    B[jj*8+5][ii*8+7] = c5;
                    B[jj*8+6][ii*8+7] = c6;
                    B[jj*8+7][ii*8+7] = c7;
                }
            }
        }
    }

}


int main(){
    for(int i = 0; i < 32; ++i)
        for(int j = 0; j < 32; ++j)
            A[i][j] = 100*i+j;
    for(int i = 0; i < 32; ++i){
        for(int j = 0; j < 32; ++j)
            cout << A[i][j] << ' ';
        cout << endl;
    }
    cout << "---------------" << endl;
    transpose_submit(32, 32, A, B);
    for(int i = 0; i < 32; ++i){
        for(int j = 0; j < 32; ++j)
            cout << B[i][j] << ' ';
        cout << endl;
    }
    return 0;
}