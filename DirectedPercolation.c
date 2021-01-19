#define N 1024
//Assumes char size of 8 bits	
#include <stdlib.h>
int main(void)  {

    unsigned char** dataSet; //Defines pointer to the whole produced data set array
    dataSet = (unsigned char**)malloc((int)N/sizeof(unsigned char*));//Asigns memory for the data array
    int flag = 0; //Time Flag
    int i = 0;
    while(flag==0)  {
        for (int j=0;j<N;j=j+4)   {
            unsigned char* currentStep = dataSet[i];
            unsigned char* nextStep = dataSet[i++];

            for (int k = j; k<j+sizeof(char)*8;k++) {
                //Go through current byte, changing values based on the rules given 
                //How to read at edge of a byte?? Might be more work this way the need be  
            }
        }
    }
}


int readBit(unsigned char byte, unsigned int k)  {
/*
 *  Provides an ability to read a specific bit in a byte of data given the byte and the bits position in that byte 
 */ 
    return byte & (1<<k);
}

void setBit(unsigned char* byte, unsigned int k, unsigned int value) {
    *byte = *byte ^ ((!value+1)^*byte)&&(1<<k);
    return; 
}
