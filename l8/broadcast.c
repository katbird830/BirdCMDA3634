#include <stdio.h>
#include<stdlib.h>
#include <math.h>

#include "mpi.h"

void myMPI_Bcast(int *N, int root) {

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	//every rank other than rank 0 recieves
	if (rank != root){
		MPI_Status status;
		int tag = 1;
		int sourceRank = rank - 1; //recieves from previous rank
		if (rank == 0) sourceRank = size -1; //careful at rank 0

		MPI_Recv(N,	//pointer to int
			1,
			MPI_INT,
			sourceRank,
			tag,
			MPI_COMM_WORLD,
			&status);
	}

	int prev = root-1;
	if (root == 0) prev = size-1;
	//every rank other than previous send the data
	if (rank != prev) {

		int tag = 1;
		int destRank = rank + 1;
		if (rank == size-1) destRank = 0;

		MPI_Send(N,
			1,
			MPI_INT,
			destRank,
			tag,
			MPI_COMM_WORLD);
	}
}

void myMPI_Barrier() {

	int N;
	int rank;
        int size;
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

 
        if (rank == 0) N=1; //only rank 0 fills a value
        myMPI_Bcast(&N, 0);
  
        printf("Rank %d recieved the value N = %d\n",rank,N);
	if (rank == size-1) N=10; //only rank 0 fills a value
  
        myMPI_Bcast(&N, size - 1);
}

int main (int argc, char **argv) {

	MPI_Init(&argc, &argv);

	int rank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int N;

	if (rank == 0) N=199; //only rank 0 fills a value

	myMPI_Bcast(&N, 0);

	printf("Rank %d recieved the value N = %d\n",rank,N);

	if (rank == 0 == size-1) N=10; //only rank 0 fills a value
 
        myMPI_Bcast(&N, size - 1);
        printf("Rank %d recieved the value N = %d\n",rank,N);

	myMPI_Barrier();

	MPI_Finalize();
	return 0;
}
