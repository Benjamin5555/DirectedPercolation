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
    /*
     * Returns element of the precomputed matrix as though it was a 2D array, basically a simpler 
     * access method 
     */ 
    return preComp[flat_index_convert(i,j)];
    
}


int precompute_denom()   {
    /*
     * Generate all possible nearest neigbour distances (raised to power of sigma) for access later 
     * via the get_denominator function.
     *
     * Stores into a 1D array in a manner that can be accessed as though in 2D array using the 
     * get_denominator function, this is to save memory as the 2D form would be symmetric about the 
     * diagonal
     *
     * Generates using that anything a distance of N/2 apart is closer by going 'through the 
     * periodic boundary'
     */ 
    for (int i = 0; i<N; i++)   {
        int j = 0;
        while(j<=i) {
            int ij = abs(i-j);
            if(ij<= N/2)    {
                preComp[flat_index_convert(i,j)]= pow(ij,SIGMA);
            }
            else    {
                preComp[flat_index_convert(i,j)]=pow(N-i+j,SIGMA);
            }
            
            j++;

        }
    }

    printf("Precompute complete\n");
}



double precompute_normalisation() {
    double c;
    for (int j = 1; j < N; j++) {
        //printf("comp %f\n",pow(j,-1*SIGMA));

        c = c  + pow(j,-1*SIGMA);
    }
    return 1/c;

}

