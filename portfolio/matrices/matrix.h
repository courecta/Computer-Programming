#pragma once

#include <stdio.h>

// const macros
const unsigned int m = 256;
const unsigned int n = 256;
const unsigned int p = 256;

// menu functions
void add();
void multiply();
void gauss();
void determinant();
void inverse();
void eigen();
void orthogonal();

// Operation functions
void populateMatrix(int m, int n, int matrix[m][n]);
void addition(int m, int n, int a[m][n], int b[m][n]);
void multiplication(int m, int n, int p, int a[m][n], int b[m][n]);
int gaussian_elimination(int n, int *pA, int *py, int **px);
int findDeterminant(int size, int matrix[n][n]);
void invert(int size, int matrix[n][n]);