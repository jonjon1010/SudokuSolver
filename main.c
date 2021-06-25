#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "solveSK.h"

#define minValue 97
#define MAXFNAMESIZE 50

char **loadBoard(char *filePath, int *dim);
char base;
void printBoard(char **board, int dim, int rows, int cols);

void testSolveSK() {
    int dim, rows, cols;
    char *filePath = (char*)malloc(MAXFNAMESIZE*sizeof(char));
    printf("Please Enter File Name:");
    scanf("%s", filePath);
    char ** board = loadBoard(filePath, &dim);
    if(dim == 6) {
      rows = 2;
      cols = 3;
    }
    else {
        rows = sqrt(dim);
        cols = sqrt(dim);
    }
    printBoard(board, dim, rows, cols);
    printf("--------------------------------Solving Sudoku--------------------------------\n");
    solveSK(board, dim);
    printBoard(board, dim, rows, cols);
    printf("------------------------------------------------------------------------------");
}

void printBoard(char **board, int dim, int rows, int cols) {
    for(int i = 0; i < dim; i++) {
        int start = 1;
        int begin = 1;
        if(i % rows == 0 && i != 0) {
            for(int x = 0; x < dim / cols; x++ ) {
                if(start) {
                    start = 0;
                }
                else {
                    printf("+");
                }
                for(int y = 0; y < rows * 2 + 1; y++) {
                    if(x == 0 && y == 0)
                        printf("  ");
                    printf("-");
                }
            }
            printf("\n");
        }
        for(int j = 0; j < dim ; j++) {
            if(begin) {
                printf(" ");
                begin = 0;
            }
            if(j % cols == 0 && j != 0) {
                printf("| ");
            }
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

char **loadBoard(char *filePath, int *dim) {
    FILE *loc;
    char line[300];
    loc = fopen (filePath, "r");
    if(loc == NULL) {
        printf("File %s can not be found\n", filePath);
        exit(EXIT_FAILURE);
    }
    if(fgets(line, sizeof(line), loc) != NULL) {
        *dim = atoi(line);
    }
    printf("Input board:: \n");
    printf("n = %d\n", *dim);
    if(*dim <= 9) {
        base = '1';
    }
    else {
        base = 'a';
    }
    int squareN = *dim * *dim;
    char ** board = (char**)malloc(sizeof(char*) * squareN);
    for(int i = 0; i < squareN; i++){
        board[i] = (char*)malloc(sizeof(char)* squareN);
    }
    int i = 0;
    while(fgets(line, sizeof(line), loc) != NULL) {
        if(i == *dim) {
            printf("Too many rows in the table.");
            exit(EXIT_FAILURE);
        }
        int j = 0;
        char *token = strtok(line," \n");
        if(token != NULL) {
            if(strcmp(token,"-") == 0) {
                board[i][j++] = '-';
            }
            else{
                board[i][j++] = token[0];
            }
        }
        while(token != NULL) {
            token = strtok(NULL, " \n");
            if(token != NULL) {
                if(strcmp(token,"-") == 0) {
                    if(j == *dim) {
                        printf("Too many columns in table row %d", i);
                        exit(EXIT_FAILURE);
                    }
                    board[i][j++] = '-';
                }
                else if((int)token[0] >= 32) {
                    if(j == *dim) {
                        printf("Too many columns in table row %d", i);
                        exit(EXIT_FAILURE);
                    }
                    board[i][j++] = token[0];
                }
            }
        }
        if(j < *dim) {
            printf("Too few columns in table row %d", i);
            exit(EXIT_FAILURE);
        }
        i++;
    }
    fclose(loc);
    for(int x = 0; x < *dim; x++){
        for(int y = 0; y < *dim; y++) {
            char c = board[x][y];
            if((c != '1') && (c != '2') && (c != '3') && (c != '4') && (c != '5') && (c != '6') && (c != '7') && (c != '8') && (c != '9') && (c != '-') && ((int)c < 97|| (int)c >= 97 + *dim)) {
                printf("Invalid character %c on position (%d, %d)", c, x, y);
                exit(EXIT_FAILURE);
            }
        }
    }
    return board;
}

int main() {
    testSolveSK();
    return 0;
}