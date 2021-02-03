//Assumes char size of 8 bits	
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include<time.h>
#include <stdint.h>  /* SIZE_MAX */

int N;
int TIME_STEPS;
int STEPS_PER_SAVE;
char* OUT_FILE_PATH="RawExperimentalOutput";

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


int init_to_zero(bool array[]) {
    /*
     *  Simply goes through and sets each element of the array to zero
     */
    for (int i = 0 ; i<N; i++)   {
        array[i] = 0;
    }
}


int main(int argc, char* argv[])  {
    /*
     * Expects N,TimeSteps,p,r
     */ 
    //Define probabilities of survival (p) and transmission (r)
    //Rand function produces an int, so shall give as integers rather than converting to 
    N          = 1000;
    TIME_STEPS = 200001;//atof(argv[2]);
    double p = 1 ;
    double r = 0.2625;
    char ExperimentName[40];
    snprintf(ExperimentName,sizeof(ExperimentName), "N%dT%dp%fr%f", N,TIME_STEPS,p,r);
    printf("%s\n",ExperimentName); 
    double initialProbability = 0.5;
  
    srand48(42);//Debug so keep same seed each time
    //srand48(time(0));//Seed number gen to time

    STEPS_PER_SAVE = TIME_STEPS/100;




    fclose(fopen(ExperimentName,"w"));
    //Make use of some counter %3 in place of the below
    bool dataStep[3][N];
    
    bool  previousStep[N];
    bool  pPropagationStep[N];
    bool  pSpreadStep[N];


    //
    for (int i=0;i<N;i++)   {
        previousStep[i] = drand48()<initialProbability;
        //dataStep[0][i] = drand48()<initialProbability; 
    }
    //int ap = 0;//By moving the 0 position around we can just
    for(int t=0;t<TIME_STEPS;t++)   {
        //bool*  previousStep = dataStep[ap%3];
        //bool*  pPropagationStep = dataStep[(ap+1)%3];
        //bool*  pSpreadStep = dataStep[(ap+2)%3];

         
        ////////PROPAGATION STEP
        for(int i = 0; i<N ;i++) {
            pPropagationStep[i] = previousStep[i]&&(drand48()<p);
        }
        

        ///////SPREAD STEP  
        //pbc left edge
        pSpreadStep[0] = pPropagationStep[0] ||(pPropagationStep[1]&&(drand48()<r) || pPropagationStep[N-1]&&(drand48()<r));

        for (int i = 1; i<N-1;i++)  {
            //Spread step for 'bulk' of system
            pSpreadStep[i] = pPropagationStep[i]||(pPropagationStep[i-1]&&(drand48()<r) 
                                || pPropagationStep[i+1]&&(drand48()<r));
        }
        //pbc right edge
        pSpreadStep[N-1] = pPropagationStep[N-1] ||(pPropagationStep[N-2]&&(drand48()<r) 
                                || pPropagationStep[0]&&(drand48()<r));

            
   
        //Set old final data step to new  position
        for(int i = 0; i <N; i++)   {
            previousStep[i] = pSpreadStep[i];
        }

        
        if(t%STEPS_PER_SAVE==0) {
                write_to_file(pSpreadStep,t,ExperimentName);
        }
       // ap= (ap+2)%3;//Have to add 2 so to skip over the intermediate step


    }
}



