#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "mpi.h"
#include "functions.h"

int main (int argc, char **argv) {

  MPI_Init(&argc,&argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  //seed value for the randomizer 
  //double seed = clock()+rank; //this will make your program run differently everytime
  double seed = rank; //uncomment this and your program will behave the same everytime it's run

  srand(seed);

  //begin with rank 0 getting user's input
  unsigned int n = 8;

  /* Q3.1 Make rank 0 setup the ELGamal system and
    broadcast the public key information */
	if (rank == 0) {
		//printf("Enter a number of bits: "); fflush(stdout);
		//char status = scanf("%u",&n);

  //make sure the input makes sense
		if ((n<3)||(n>31)) {//Updated bounds. 2 is no good, 31 is actually ok
			printf("Unsupported bit size.\n");
    		return 0;   
		}
		printf("\n");
	}

  //declare storage for an ElGamal cryptosytem
		unsigned int p, g, h, x;

  //setup an ElGamal cryptosystem
	if (rank ==0) {
		setupElGamal(n,&p,&g,&h,&x);
	}
	//MPI_Bcast
	MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&g, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&h, 1, MPI_INT, 0, MPI_COMM_WORLD);


  //Suppose we don't know the secret key. Use all the ranks to try and find it in parallel
  if (rank==0)
    printf("Using %d processes to find the secret key...\n", size);

  /*Q3.2 We want to loop through values i=0 .. p-2
     determine start and end values so this loop is 
     distributed amounst the MPI ranks  */
  unsigned int N = p-1; //total loop size
  unsigned int start, end;
  unsigned int chunk = N/size;	
	unsigned int rem = N%size;
	if (rank < rem) {
  		start = rank*chunk + rank; 
  		end = start + chunk + 1;
	}
	else {
		start = rank*chunk + rem;
		end = start + chunk;
	}

  //loop through the values from 'start' to 'end'
	double timeStart = MPI_Wtime();
	unsigned int counter = 0;
	unsigned int Ninterval = 50;
	unsigned int redFlag = 0;

	for (unsigned int i=start;i<end;i++) {
		counter++;

    	if (modExp(g,i+1,p)==h) {
      		printf("Secret key found! x = %u \n", i+1);
			redFlag = 1;
		}
		//attempting do extra credit portion
		//if (counter%Ninterval == 0 && redFlag == 1) {
		//	break;
		//}
	}

	double timeEnd = MPI_Wtime();
	double timeTotal = timeEnd-timeStart;

	printf("The time elapsed was %f seconds.\n", timeTotal);
	//printf("The program went through %u iterations.\n", counter);
	printf("The throughput was %f iteration/time.\n", (double)counter/timeTotal);

	MPI_Finalize();

	return 0;
}
