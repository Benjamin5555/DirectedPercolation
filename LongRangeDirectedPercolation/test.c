#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <unistd.h>


#define NumberOfSites 71
#define SIGMA 10.0


int main()
{
	int i, j;
	double normconst;
	double distances[NumberOfSites/2+1];

	// Initialise the distances
	distances[0] = 0.0;
	for(j=1;j<NumberOfSites/2+1;j++)
		distances[j] = 1.0/pow((double)j,1.0+SIGMA);

	// Calculate the normalisation constant
	normconst = 0.0;
	for(j=1;j<NumberOfSites;j++)
		normconst += distances[abs(-j - NumberOfSites*(int)(2*(-j)/NumberOfSites))];

			
	// Check distances
	for(i=0;i<NumberOfSites;i++)
		for(j=0;j<NumberOfSites;j++){
				printf("%d %d %d\n",i,j, abs(i-j - NumberOfSites*(int)(2*(i-j)/NumberOfSites)));
		}


	return 0;
}
