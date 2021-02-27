#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <unistd.h>


#define NumberOfSites 1000
#define MaxIter 10000

#define SIGMA 1.0
#define threshold 0.0

int main()
{
	int Lattice[NumberOfSites], LatticeNew[NumberOfSites];
	double zeta[NumberOfSites];
	int i, j, k;
	double chi;
	double prob, turbulentfraction;

	double normconst = 0.0;
	double distances[NumberOfSites/2+1];

	char filename_A[FILENAME_MAX+1];
	FILE *fff;

	// Initialise the random number generator
	srand48((unsigned) time(NULL) * getpid());

	// Initialise the distances
	distances[0] = 0.0;
	for(j=1;j<NumberOfSites/2+1;j++)
		distances[j] = 1.0/pow((double)j,1.0+SIGMA);

	// Calculate the normalisation constant
	for(j=1;j<NumberOfSites;j++)
		normconst += distances[abs(-j - NumberOfSites*(int)(2*(-j)/NumberOfSites))];

	for(i=0;i<NumberOfSites;i++){
		Lattice[i] = 1;
		LatticeNew[i] = 0;
	}

	fff = fopen("turbfraction.txt", "w");

	for(k=0;k<MaxIter;k++){

		//Random numbers for spreading
		for(i=0;i<NumberOfSites;i++)
			zeta[i] = (double)(Lattice[i])*drand48();

		for(i=0;i<NumberOfSites;i++){

			chi = 0.0;
			for(j=0;j<NumberOfSites;j++)
				chi += zeta[j]*distances[abs(i-j - NumberOfSites*(int)(2*(i-j)/NumberOfSites))];
			chi = chi/normconst;
			chi += zeta[i];

			if(chi>threshold)
				LatticeNew[i] = 1;
			else
				LatticeNew[i] = 0;
		}

		//Calculating turbulent fraction and housekeeping
		turbulentfraction = 0.0;
		for(i=0;i<NumberOfSites;i++){
			Lattice[i] = LatticeNew[i];
			turbulentfraction += (double)Lattice[i]/NumberOfSites;
		}

		if(turbulentfraction==0.0)
			break;

        fprintf(fff,"%d %20.18f\n", k, turbulentfraction);
	}

	fclose(fff);

	return 0;
}
