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
/*
    if (i>j)    {
        return preComp[ijTo1D(i,j)];
    }
    else {
        return preComp[ijTo1D(j,i)];
    }
    */
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
        //printf("comp %f\n",pow(j,-1*SIGMA));

        c = c  + pow((double)j,-1*(double)SIGMA);
    }
    //printf("%f,%f)\n\n\n",c,1/(c));
    return c;

}

