#include <math.h>
#include "parameter.h"


double preComp[(int)N/2+1];


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
    for (i;i <= (int)N/2+1; i++) {
        preComp[i] = pow((double)i,(double)SIGMA+1.0);
    }
}

double precompute_normalisation() {
    /*
     *  Precomputes normalisation of the 'other contributions' requires denominators have already 
     *  been precomputed 
     */ 
    double c=0;
    for (int j = 1; j < N; j++) {
        c = c+1.0/get_denominator(0,j);
        //c = c  + pow((double)j,-1*(double)SIGMA);
    }
    return c;

}

