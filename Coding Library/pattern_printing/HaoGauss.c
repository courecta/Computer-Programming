#include "myge.h"
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

// 輔助函數 - 計算最大公約數（Greatest Common Divisor, GCD）
int64_t gcd(int64_t a, int64_t b) {
    return b == 0 ? a : gcd(b, a % b);
}

// 輔助函數 - 計算最小公倍數（Least Common Multiple, LCM）
int64_t lcm(int64_t a, int64_t b) {
    return (a / gcd(a, b)) * b;
}



// 輔助函數 - 檢查是否有解和是否為多重解
int32_t check_solution(int32_t n, int32_t extended_matrix[n][n+1]) {
    int32_t rank = 0;
    bool has_zero_row = false;
    for (int32_t i = 0; i < n; ++i) {
        bool non_zero_row = false;
        for (int32_t j = 0; j < n; ++j) {
            if (extended_matrix[i][j] != 0) {
                non_zero_row = true;
                break;
            }
        }
        if (non_zero_row) {
            rank++;
        } else {
            if (extended_matrix[i][n] != 0) {
                return 0; // 無解
            }
            has_zero_row = true;
        }
    }
    return (rank == n) ? 1 : (has_zero_row ? 2 : 1);
}

int32_t gaussian_elimination(int32_t n, int32_t *pA, int32_t *py, int32_t **px) {
    if (n <= 0 || pA == NULL || py == NULL || px == NULL) {
        return -1; // 檢查輸入有效性
    }

    *px = (int32_t *)malloc(n * sizeof(int32_t));
    if (*px == NULL) {
        return -1; // 記憶體分配失敗
    }

    int64_t extended_matrix[n][n + 1];  // 使用 int64_t 以避免溢出
    for (int32_t i = 0; i < n; ++i) {
        for (int32_t j = 0; j < n; ++j) {
            extended_matrix[i][j] = *((pA + i * n) + j);
        }
        extended_matrix[i][n] = py[i];
    }

    for (int32_t k = 0; k < n; ++k) {
    // 檢查主元是否為零
    if (extended_matrix[k][k] == 0) {
        // 需要進行行交換
        int32_t swap_row = -1;
        for (int32_t i = k + 1; i < n; ++i) {
            if (extended_matrix[i][k] != 0) {
                swap_row = i;
                break;
            }
        }
        if (swap_row == -1) {
            // 沒有解或者有無限多解
            free(*px);
            return 0; // 沒有解
        }
        // 進行行交換
        for (int32_t j = 0; j <= n; ++j) {
            int32_t temp = extended_matrix[k][j];
            extended_matrix[k][j] = extended_matrix[swap_row][j];
            extended_matrix[swap_row][j] = temp;
        }
    }
    // 高斯消去操作
        for (int32_t i = k + 1; i < n; ++i) {
            int64_t factor_k = lcm(extended_matrix[k][k], extended_matrix[i][k]) / extended_matrix[k][k];
            int64_t factor_i = lcm(extended_matrix[k][k], extended_matrix[i][k]) / extended_matrix[i][k];

            for (int32_t j = k; j <= n; ++j) {
                extended_matrix[i][j] *= factor_i;
                extended_matrix[i][j] -= extended_matrix[k][j] * factor_k;
            }

            // 對當前行進行約分
            int64_t row_gcd = extended_matrix[i][k];
            for (int32_t j = k + 1; j <= n; ++j) {
                row_gcd = gcd(row_gcd, extended_matrix[i][j]);
            }
            if (row_gcd != 0) {
                for (int32_t j = k; j <= n; ++j) {
                    extended_matrix[i][j] /= row_gcd;
                }
            }
        }
    }

    // 進行回代求解
    for (int32_t i = n - 1; i >= 0; --i) {
        (*px)[i] = extended_matrix[i][n];
        for (int32_t j = i + 1; j < n; ++j) {
            (*px)[i] -= extended_matrix[i][j] * (*px)[j];
        }
        if (extended_matrix[i][i] == 0) {
            if ((*px)[i] == 0) {
                free(*px);
                return 2; // 系統有無限多解
            } else {
                free(*px);
                return 0; // 系統無解
            }
        }
        (*px)[i] /= extended_matrix[i][i];
    }

    return 1; // 成功求解
}