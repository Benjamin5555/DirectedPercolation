#define N 1024
#define TIME_STEPS 100
//Assumes char size of 8 bits	
#include <stdlib.h>
#include <stdbool.h>
#include<time.h>

int main(void)  {
    //Define probabilities of survival (p) and transmission (r)
    //Rand function produces an int, so shall give as integers rather than converting to 
    //flractions each step
    int p = 0.75*RAND_MAX ;
    int r = 0.5*RAND_MAX;
    int initialProbability = 0.5*RAND_MAX;


    bool dataSet[TIME_STEPS][N]; 
    
    //srand(time(0)); //Fordebugging same behaviour would be useful

    for (int i=0;i<N;i++)   {
        dataSet[0][i] = rand()<initialProbability;  //Generate states based on above probability
    }

    int i = 0;
    for (int t=0;t<TIME_STEPS-1;t=t+1)    { //Main processing loop     
        
        //pbc so apply calcs for first state here first we only have uninitalised `junk' in the new
        //row of the array so far so can overwrite

        dataSet[t+1][N-1] = dataSet[t][0]&&rand()<r;
        dataSet[t+1][0]   = dataSet[t][0]&&rand()<p;
        dataSet[t+1][1]   = dataSet[t][0]&&rand()<r;

        for (int i=1;i<N-1;i++)   { //Evolving the 'middle bit' (unaffected by pbd) of processing
            
            //Apply or operation so to 'protect' active state already set from it being set (and 
            //surviving) previous time step.
            dataSet[t+1][i-1] = dataSet[t+1][i-1] || (dataSet[t][i]&&(rand()<r));
            dataSet[t+1][i]   = dataSet[t+1][i]   || (dataSet[t][i]&&(rand()<p));
            //Will not have been set by now so no or operation required
            dataSet[t+1][i+1] = dataSet[t][i]&&(rand()<r); 
        }
        dataSet[t+1][N-2]  = dataSet[t+1][N-2] || (dataSet[t][i]&&(rand()<r));
        dataSet[t+1][N-1]  = dataSet[t+1][0]   || (dataSet[t][i]&&(rand()<p));
        //Will not have been set by now so no or operation required
        dataSet[t+1][0] = dataSet[t+1][0] || (dataSet[t][i]&&(rand()<r)); 

        
    }

}
