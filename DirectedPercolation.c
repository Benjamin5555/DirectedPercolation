//Assumes char size of 8 bits	
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>  /* SIZE_MAX */

#include "parameter.h"


int write_to_file_and_count(bool dataSet[N],int t,char* ExperimentName)    {
    /*
     * Outputs the data (a timestep worth of states) to a file, since each item is fixed length we do not bother with csv
     * Returns a count of number of active states remaining so the current run can end if it is zero
     * t : timestep represented, for file names
     * dataSet: the data (time step of states) being written to file
     *
     * 
     */
    int count = 0;
    char savePath[200];
    //t = t/STEPS_PER_SAVE; //Better ordering
    sprintf(savePath,"%s/t%d.exp",OUT_FILE_PATH,t);
    FILE* out = fopen(savePath,"w");
    for (int i = 0; i < N; i++) {
        if(dataSet[i])  {count++;}
        fprintf(out,"%d",dataSet[i]);
    }
    fclose(out);
    return count;
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

    char ExperimentName[40];
    //snprintf(ExperimentName,sizeof(ExperimentName), "N%dT%dp%fr%f", N,TIME_STEPS,p,r);
  
    srand48(42);//Debug so keep same seed each time
    //srand48(time(0));//Seed number gen to time

    


    //Make use of some counter %3 in place of the below
    bool dataStep[3][N];
    
    //bool  previousStep[N];
    //bool  pPropagationStep[N];
    //bool  pSpreadStep[N];


    //
    for (int i=0;i<N;i++)   {
        //previousStep[i] = drand48()<INIT_PROB;
        dataStep[0][i] = drand48()<INIT_PROB; 
    }
    int ap = 0;//By moving the 0 position around we can just
    for(int t=0;t<TIME_STEPS;t++)   {
        bool*  previousStep = dataStep[ap%3];
        bool*  pPropagationStep = dataStep[(ap+1)%3];
        bool*  pSpreadStep = dataStep[(ap+2)%3];

         
        ////////PROPAGATION STEP
        for(int i = 0; i<N ;i++) {
            //If already alive then try keeping alive for next step
            pPropagationStep[i] = previousStep[i]&&(drand48()<p);
        }
        

        ///////SPREAD STEP  
        //pbc left edge
        pSpreadStep[0] = pPropagationStep[0] || ( ( pPropagationStep[1]&&(drand48()<r) ) 
                                                        || (pPropagationStep[N-1]&&(drand48()<r) ));

        for (int i = 1; i<N-1;i++)  {
            //Spread step for 'bulk' of system
            //If already alive, just keep alive or if can  spread trial make alive from neighbour
            pSpreadStep[i] = pPropagationStep[i]||( ( pPropagationStep[i-1]&&(drand48()<r) )
                                                        || (pPropagationStep[i+1]&&(drand48()<r) ));
        }
        //pbc right edge
        pSpreadStep[N-1] = pPropagationStep[N-1] || ( ( pPropagationStep[N-2]&&(drand48()<r) )
                                                        || (pPropagationStep[0]&&(drand48()<r) ) );

            
   
        //Set old final data step to new  position
        //for(int i = 0; i <N; i++)   {
        //    previousStep[i] = pSpreadStep[i];
        //}

        
        if(t%STEPS_PER_SAVE==0) {
            int count = write_to_file_and_count(pSpreadStep,t,ExperimentName); 
            if(count == 0||count==N){
                //If active states are zero we have gone into laminar dominated, if all active 
                //states then turbulence dominated either case is useless for finding the critical point
                printf("Exiting current model as %d of %d states active, hence some behaviour dominates",count,N);
                break;            
            }
        }
        ap= (ap+2)%3;//Have to add 2 so to skip over the intermediate step


    }
}



