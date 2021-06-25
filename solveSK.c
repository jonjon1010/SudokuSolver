#include <stdio.h>
#include <stdlib.h>
#include "solveSK.h"
#include "solver.h"
#include "math.h"
#include <time.h>

int encode(int rows, int cols, int dim, int size) {
    int ans = size * size * rows + size * cols + (dim - 97) + 1;
    return ans;
}

void Cells(veci* temp, int size, int x, solver* s){
    int y = size / x;
    for(int a = 0; a < size / y; a++){
        for(int b = 0; b < size / x; b++) {
            for(int n = 97; n < 97 + size; n++) {
                veci_new(temp);
                veci_resize(temp, 0);

                for(int j = 0; j < x; j++) {
                    for(int i = 0; i < y; i++) {
                        int l = encode(y * a + i, x * b + j, n, size);
                        veci_push(temp, toLit(l));
                    }
                }
                lit* begin = veci_begin(temp);
                solver_addclause(s, begin, begin + veci_size(temp));
                veci_delete(temp);
            }
        }
    }
}

void numExist(veci* temp, int size, solver* s, char** board) {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            char z = board[i][j];
            if(z == '-') {
                continue;
            }
            int a = (int)z;
            int b = encode(i, j, a, size);
            veci_new(temp);
            veci_resize(temp, 0);
            veci_push(temp, toLit(b));
            lit* begin = veci_begin(temp);
            solver_addclause(s, begin, begin + veci_size(temp));
            veci_delete(temp);
        }
    }
}

void singleLine(veci* temp, int size, solver* s) {
    int i, j, n, inv;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            for(n = 97; n < 97 + size; n++) {
                for(inv = n + 1; inv < 97 + size; inv++) {
                    veci_new(temp);
                    veci_resize(temp, 0);
                    veci_push(temp, lit_neg(toLit(encode(i, j, n, size))));
                    veci_push(temp, lit_neg(toLit(encode(i, j, inv, size))));
                    lit* begin = veci_begin(temp);
                    solver_addclause(s, begin, begin + veci_size(temp));
                    veci_delete(temp);
                }
            }
        }
    }
}

void Cols(veci* temp, int size, solver* s)
{
    for(int j = 0; j < size; j++) {
        for(int k = 97; k < 97 + size; k++) {
            veci_new(temp);
            veci_resize(temp, 0);
            for(int i = 0; i < size; i++) {
                int l = encode(i, j, k, size);
                veci_push(temp, toLit(l));
            }
            lit* begin = veci_begin(temp);
            solver_addclause(s, begin, begin + veci_size(temp));
            veci_delete(temp);
        }
    }
}

void Rows(veci* temp, int size, solver* s) {
    for(int i = 0; i < size; i++) {
        for(int k = 97; k < 97 + size; k++) {
            veci_new(temp);
            veci_resize(temp, 0);
            for(int j = 0; j < size; j++) {
                int l = encode(i, j, k, size);
                veci_push(temp, toLit(l));
            }
            lit* begin = veci_begin(temp);
            solver_addclause(s, begin, begin + veci_size(temp));
            veci_delete(temp);
        }
    }
}

void decode(int a, int size, int* i, int* j, int* n) {
    int temp;
    temp = (a - 1) % size ;
    *n = temp + 97;
    *j = (a - (temp + 1)) / size % size;
    *i = (a - (temp + 1) - *j * size) / (size * size);
}

void solveSK(char ** board, int dim) {
    solver* s = solver_new();
    lbool   st;
    veci lits;
    veci_new(&lits);
    lit* begin;
    int clk = clock(), r, c, i, j, k, n;
    if(dim == 6) {
        n = 3;
    }
    else {
        n = sqrt(dim);
    }
    Cells(&lits, dim, n, s);
    numExist(&lits, dim, s, board);
    singleLine(&lits, dim, s);
    Cols(&lits, dim, s);
    Rows(&lits, dim, s);
    st = solver_solve(s, 0, 0);
    if(st != l_True) {
        printf("Couldn't solve the sudoku puzzle");
        exit(EXIT_FAILURE);
    }
    if(st == l_True) {
        int k;
        for(k = 0; k < s -> model.size; k++) {
            int i, j, n;
            decode(k, dim, &i, &j, &n);
            if(s -> model.ptr[k] == l_True) {
                board[i][j] = n;
            }
        }
    }
}