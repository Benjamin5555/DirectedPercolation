//Assumes char size of 8 bits	
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>  /* SIZE_MAX */
#include <unistd.h>

#include "parameterLRDP.h"


#define prob_surv  0.75

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
    if(out ==NULL)  {
        printf("OUTPUT TO FILE ERROR");
        return 0;
    }
    else
    {
        fflush(out);
        for (int i = 0; i < N; i++) {
            if(dataSet[i]) 
                {count++;}
            fprintf(out,"%d",dataSet[i]);
        }
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

    char ExperimentName[40];
  
    //srand48(42);//Debug so keep same seed each time
    srand48(getpid()^time(0));//Seed number gen to time

    


    //Make use of some counter %3 in place of the below
    bool dataStep[2][N];
    

    for (int i=0;i<N;i++)   {
        dataStep[0][i] = drand48()<INIT_PROB; 
        //dataStep[0][i] = 1;//drand48()<INIT_PROB; 
    }
    int ap = 0;//By moving the 0 position around we can just
    for(int t=0;t<TIME_STEPS;t++)   {
        bool*  previousStep = dataStep[ap%2];
        bool*  finalState = dataStep[(ap+1)%2];


        for (int i = 0; i < N; i++) {
            double chi = previousStep[i] * prob_surv;       
            printf("chi=%f\n", chi);
            double psi = 0;
            for (int j = 0; j < N; j++) {
                psi = psi+(previousStep[j]*prob_surv)/(denoms[i][j]);

            }
            finalState[i] = (chi+psi)>drand48();

        }
       
        
        if(t%STEPS_PER_SAVE==0) {
            int count = write_to_file_and_count(finalState,t,ExperimentName); 
            /*
            if(count == 0){
                //If active states are zero we have gone into laminar dominated, if all active 
                //states then turbulence dominated either case is useless for finding the critical point
                printf("Exiting current model as %d of %d states active, hence some behaviour dominates",count,N);
                break;            
            }
            */
        }
        ap= (ap+1)%2;//Have to add 2 so to skip over the intermediate step


    }
}



