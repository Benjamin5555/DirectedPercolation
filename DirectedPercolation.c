//Assumes char size of 8 bits	
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include<time.h>
#include <stdint.h>  /* SIZE_MAX */

int N;
int TIME_STEPS;
int STEPS_PER_SAVE;
char* OUT_FILE_PATH="ExperimentalOutput";

int write_to_file(bool dataSet[N],int t,char* ExperimentName)    {
    /*
     * Outputs the data (a timestep worth of states) to a file, since each item is fixed length we do not bother with csv
     * t : timestep represented, for file names
     * dataSet: the data (time step of states) being written to file
     */

    char savePath[200];
    sprintf(savePath,"%s/%s.t%d.exp",OUT_FILE_PATH,ExperimentName,t);
    FILE* out = fopen(savePath,"w");
    for (int i = 0; i < N; i++) {
        fprintf(out,"%d",dataSet[i]);
    }
    fclose(out);

}


int init_to_zero(bool array[],int arrayLen) {
    /*
     *  Simply goes through and sets each element of the array to zero
     */
    for (int i = 0 ; i<arrayLen; i++)   {
        array[i] = 0;
    }
}


int main(int argc, char* argv[])  {
    /*
     * Expects N,TimeSteps,p,r
     */ 
    //Define probabilities of survival (p) and transmission (r)
    //Rand function produces an int, so shall give as integers rather than converting to 
    N = 100000;
    TIME_STEPS = 200000;//atof(argv[2]);
    double p = 0.75 ;
    double r = 0.2625;
    char ExperimentName[40];
    snprintf(ExperimentName,sizeof(ExperimentName), "N%dT%dp%fr%f", N,TIME_STEPS,p,r);
    printf("%s\n",ExperimentName); 
    int initialProbability = 0.5;
  
    srand48(42);//Debug so keep same seed each time
    //srand48(time(0));//Seed number gen to time

    STEPS_PER_SAVE = TIME_STEPS/100;




    fclose(fopen(ExperimentName,"w"));
    //Make use of some counter %3 in place of the below
    bool previousStep[N];
    bool pPropagationStep[N];
    bool pSpreadStep[N];
   
    //
    for (int i=0;i<N;i++)   {
        previousStep[i] = drand48()<initialProbability; 
    }
    
    
    for(int t=0;t<TIME_STEPS;t++)   {
        init_to_zero(pPropagationStep,N);
        
        ////////PROPAGATION STEP
        for(int i = 0; i<N;i++) {
            pPropagationStep[i] = previousStep[i]&&(drand48()<p);
        }


        ///////SPREAD STEP  
        //pbc left edge
        pSpreadStep[0] = pPropagationStep[1]&&(drand48()<r) || pPropagationStep[N-1]&&(drand48()<r);

        for (int i = 1; i<N-1;i++)  {
            //Spread step for 'bulk' of system
            pSpreadStep[i] = pPropagationStep[i-1]&&(drand48()<r) 
                                || pPropagationStep[i+1]&&(drand48()<r);
        }
        //pbc right edge
        pSpreadStep[N-1] = pPropagationStep[N-2]&&(drand48()<r) 
                                || pPropagationStep[0]&&(drand48()<r);

            
   
        //Set old data step to new  position
        for(int i = 0; i <N; i++)   {
            previousStep[i] = pSpreadStep[i];
        }

        
        if(t%STEPS_PER_SAVE==0) {
                write_to_file(pSpreadStep,t,ExperimentName);
        }   


    }
}



