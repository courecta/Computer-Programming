#include "mystatistics.h"

int32_t statistics( int32_t *pData , int32_t size, double *pMean, double *pVariance , double *pStd ){
    
    if (!pData || !pMean || !pVariance || !pStd || sizeof(*pData) != sizeof(int32_t) || size <= 0){
        return -1;
    }

    double sum = 0.0;
    for (size_t i = 0 ; i < size ; ++i){
        sum += pData[i];
    }

    *pMean = sum / size;

    double var = 0.0;
    for (size_t i = 0 ; i < size ; ++i){
        var += pow(pData[i] - *pMean, 2);
    }

    *pVariance = var / size;

    *pStd = sqrt(*pVariance);

    return 0;
}