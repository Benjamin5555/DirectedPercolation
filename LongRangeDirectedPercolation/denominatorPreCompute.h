#include <math.h>
#include "parameter.h"


    
float preComp[(unsigned int)N*(N+1)/2];

int flat_index_convert(int i,int j) {
    /*
     * Converts indexes imaginging the precompute matrix as a symetric through the diagonal 2D 
     * matrix into a 1D matrix that only needs store the lower triangle
     */ 
    if(i>=j) {
        return ((i+1)*i)/2+j;
    }
    else    {
        return flat_index_convert(j,i);
    }
    
}


float get_denominator(int i, int j) {
    
    return preComp[flat_index_convert(i,j)];
    
}


int precompute_denom(float sigma)   {
    for (int i = 0; i<N; i++)   {
        int j = 0;
        while(j<=i) {
            int ij = abs(i-j);
            if(ij<= N/2)    {
                preComp[flat_index_convert(i,j)]= pow(ij,sigma);
            }
            else    {
                preComp[flat_index_convert(i,j)]=pow(N-i+j,sigma);
            }
            
            j++;

        }
    }
    printf("Precompute complete");
}


