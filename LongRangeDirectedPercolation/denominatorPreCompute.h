#include <math.h>
#include "parameter.h"


double preComp[N];


int ijTo1D(int i,int j) {
    return abs(i-j-N*((int)2*(i-j)/N));
}

float get_denominator(int i, int j) {
    /*
     *
     */ 
    return preComp[ijTo1D(i,j)];
}



int precompute_denom()   {
    /*
     *
     */ 
    int i = 0;
    for (i;i <= N/2; i++) {
        preComp[i] = pow((double)i,(double)SIGMA);
    }
    for (i; i <N; i++)  {
        preComp[i] = pow((double)(N-i),(double)SIGMA);
    }

}


double precompute_normalisation() {
    double c=0;
    for (int j = 1; j < N; j++) {

        c = c  + pow((double)j,-1*(double)SIGMA);
    }
    return c;

}

