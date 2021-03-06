//Assumes char size of 8 bits	
#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>  /* SIZE_MAX */
#include <unistd.h>
#include "parameter.h"
#include "denominatorPreCompute.h"
#include <omp.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define ExperimentName STR(N),STR(TIME_STEPS),STR(SIGMA),STR(p)




int write_to_file_and_count(bool dataSet[N],int t)    {
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
    
    sprintf(savePath,"%s/t%d.exp",OUT_FILE_PATH,t);
    printf("C:%s\n",savePath);

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



/*
int write_to_file_and_count(bool dataSet[N],int t)    {
    *
     * Outputs the data (a timestep worth of states) to a file, since each item is fixed length we do not bother with csv
     * Returns a count of number of active states remaining so the current run can end if it is zero
     * t : timestep represented, for file names
     * dataSet: the data (time step of states) being written to file
     *
     * 
     *
    int count = 0;
    char savePath[200];
    
    sprintf(savePath,"%s/t%d.exp",OUT_FILE_PATH,t);
    FILE* out = fopen(savePath,"w");
    if(out ==NULL)  {
        printf("OUTPUT TO FILE ERROR\n");
        return 0;
    }
    else
    {
        fflush(out);
        for (int i = 0; i < N; i++) {
            if(dataSet[i]) 
                {count++;}
        }
        fprintf(out,"%d\n",count);
        
    }
    fclose(out);
    return count;
}
*/

int gen_state_probabilities(double probabilities_array[], bool states_array[]){

    /*
     *  Generates an array of random probabilities relating to each state in the array, 0 if state 
     *  is passive
     */

    for (int i = 0 ; i<N; i++)   {
        if(states_array[i])    {
            probabilities_array[i] = drand48();

        }
        else
        {
            probabilities_array[i] = 0;
        }
    }
}


int main(int argc, char* argv[])  {
    /*
     * Expects N,TimeSteps,p,r
     */ 


  
    srand48(getpid()^time(0));//Seed number gen to time and pid
    
    //Generate all possible nearest neigbour distances (raised to power of sigma) for access later 
    //via the get_denominator function
    precompute_denom();
    double normalisation = precompute_normalisation();

    //Store current [0] and next [1] steps of the system 
    bool dataStep[2][N];
   
    
        //initalisation of the system
        for (int i=0;i<N;i++)   {
            dataStep[0][i] =1;
        }

            

        int ap = 0;//By moving the 0 position around we don't have to rewrite array from prev to next

        double stateProbabilities[N];

        for(int t=0;t<TIME_STEPS;t++)   {
            bool*  previousStep = dataStep[ap%2];
            bool*  finalState = dataStep[(ap+1)%2];
            
            gen_state_probabilities(stateProbabilities, previousStep);
        
          #pragma omp parallel default(none) shared(ap,t,normalisation,dataStep,previousStep,finalState,stateProbabilities)
          {
    



            #pragma omp for 
            for (int i = 0; i < N; i++) {


                //Active states have some sigma probability of  survival irregardless of other states
                double ownContribution = stateProbabilities[i];       
                
                //Sum up effects of interaction with all states across the system (other than itself) 
               //which act to increase the probability of survival into the next time step 
                double otherContributions = 0;
                for (int j = 0; j < N; j++) {
                    if(i!=j)   {
                        // Don't want to include the current particle in the calculation will diverge/give error
                        
                        otherContributions = otherContributions+(stateProbabilities[j])/(get_denominator(i,j));
                    }
                }
                
                finalState[i] = ((ownContribution+(1/normalisation)*otherContributions))>p; //Check meets threshold probability

             
            }
          }
            //for (int i =0; i <N;i++)  {
            //    printf("%d,",finalState[i]);
            //}
            //printf("\n\n");

            if(t%STEPS_PER_SAVE==0) {
                int count = write_to_file_and_count(finalState,t); 
                //printf("%d,%d\n",t,count); 
                if(count == 0){
                    //If active states are zero we have gone into laminar dominated, if all active 
                    //states then turbulence dominated either case is useless for finding the critical point
                    printf("Exiting current model as %d of %d states active, hence some behaviour dominates",count,N);
                    break;            
                }
                
            }
            ap= (ap+1)%2;//Have to add 2 so to skip over the intermediate step

        }
    
}



