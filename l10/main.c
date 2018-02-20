#include "mympi.h"

int main (int argc, char **argv) {

	MPI_Init(&argc, &argv);
	
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	int N;

	if (rank == size-1) N=10;

	MPI_Bcast(&N,			//pointer to data
			1,				//count (number of entries)
			MPI_INT,		//data type
			0,				//root process
			MPI_COMM_WORLD);

	printf("Rank %d recieved the value N %d\n",rank,N);

	MPI_Barrier(MPI_COMM_WORLD);

	float val = 1.0;
	float sum;

	//MPI's reduce function
	MPI_Reduce(&val,		//send buffer
				&sum,		//recieve buffer
				1			//count (number of entries
				MPI_FLOAT,	//data type
				MPI_SUM, 	//operation there are a bunch others
				0,			//root process (the one that has final answer)
				MPI_COMM_WORLD);

	//this performs the reduction so all ranks have the final vale
	MPI_Allreduce(&val,
				&sum,
				1,
				MPI_FLOAT,
				MPI_SUMM,
				MPI_COMM_WORLD);


	for (intr=0;r<size,r++) {
		if (r==rank) {
			printf("Rank %d has value %f after the reduction.\n", rank, sum);
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}

	val = (float) rank;
	float *gatheredVal;
	
	//only rank 0 needs the storage
	if (rank==0) gatheredVal = (float *) malloc(size*sizeof(float));

	MPI_Gather(&val,		//send buffer
				1,			//send count
				MPI_FLOAT,	//send type
				gatherVal,	//recv buffer
				1,			//recv count
				MPI_FLOAT,	//recv type
				0,			//root process
				MPI_COMM_WORLD);

	if (rank ==0) {
		for (int r =0; r<size; r++) {
			printf("gatheredVal[%d] = %f \n",r,gatheredVal[r]);
			gatheredVal[r] *= 2; //change the data
		}
	}

	//scatter the data back (this is the reverse of gather
	MPI_Scatter(gatheredVal,	//send buffer
				1,				//send count
				MPI_FLOAT,		//send type
				&val,			//recv buffer
				1,				///recv count
				MPI_FLOAT,		//recv type
				0,				//root rank
				MPI_COMM_WORLD);

	printf("Rank %d has value %f after the scatter. \n", rank, val);


	MPI_Finalize();
	return 0;
}	
