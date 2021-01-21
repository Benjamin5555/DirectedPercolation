//Assumes char size of 8 bits	
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include<time.h>


int N;
int TIME_STEPS;


int writeDataToFile(char* filePathStr,bool dataSet[][N], int T )    {
    /*
     * Outputs the data to a file, since each item is fixed length we do not bother with csv
     * filepath: pointer to a filepath string to be written to 
     * dataSet: the data being written to file
     * T: Final timestep reached, may not be TIME_STEPS constant
     */ 
    FILE* filePath = fopen(filePathStr,"w");
    for (int t=0; t<T;t++)  {
        for (int i=1;i<N-1;i++)   { 
            if(dataSet[t][i])    {
                fwrite("1",sizeof(char),1,filePath);
            }
            else
            {
                fwrite("0",sizeof(char),1,filePath);
            }
        }
        fwrite("\n", sizeof(char), 1, filePath);//Newline for next data set
    }
    fclose(filePath);
    return 0;

}
int main(int argc, char* argv[])  {
    /*
     * Expects N,TimeSteps,p,r
     */ 
    //Define probabilities of survival (p) and transmission (r)
    //Rand function produces an int, so shall give as integers rather than converting to 
    N = atof(argv[1]);
    TIME_STEPS = atof(argv[2]);
    int p = atof(argv[3])*RAND_MAX ;
    int r = atof(argv[4])*RAND_MAX;
    char ExperimentName[30];
    snprintf(ExperimentName,sizeof(ExperimentName), "N%dT%dp%.3fr%.3f.exp", N,TIME_STEPS,p/(float)RAND_MAX,r/(float)RAND_MAX);
    printf("%s\n",ExperimentName); 
    int initialProbability = 0.1*RAND_MAX;
    
    printf("%d\n",N);
    printf("%d\n",TIME_STEPS);
    printf("%d\n",p);
    printf("%d\n",r);




    bool dataSet[TIME_STEPS][N]; 
    
    //srand(time(0)); //Fordebugging same behaviour would be useful

    for (int i=0;i<N;i++)   {
        dataSet[0][i] = rand()<initialProbability;  //Generate states based on above probability
    }
    int i = 0;
    int t = 0; //Want this outside of for loop scope in case of crash etc
    for (t;t<TIME_STEPS-1;t=t+1)    { //Main processing loop     
        
        //pbc so apply calcs for first state here first we only have uninitalised `junk' in the new
        //row of the array so far so can overwrite

        dataSet[t+1][N-1] = dataSet[t][0]&&rand()<r;
        dataSet[t+1][0]   = dataSet[t][0]&&rand()<p;
        dataSet[t+1][1]   = dataSet[t][0]&&rand()<r;

        for (i=1;i<N-1;i++)   { //Evolving the 'middle bit' (unaffected by pbd) of processing
            
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
    writeDataToFile(ExperimentName,dataSet,TIME_STEPS);

}


