#include <stdio.h>
#include <stdlib.h>

#include "cuda.h"

//This is my DEVICE functionfun to the hoasttion is visible
// __global__ means this 
__global__ void kernelHelloWorld() {

	int thread = threadIdx.x;	//local thread number in a block
	int block = blockIdx.x;		//block number

	printf("Hello Worldfrom thread %d of block %d!\n", thread, block);

}

int main(int argc, char** argv) {

	int Nblocks = 10; //number of blocks
	int Nthreads = 3; //number of threads per block

	// run the function'kernalHelloWorld' on the DEVICE
	kernelHelloWorld <<< Nblocks, Nthreads >>> ();

	//wait for the DEVICE function to complete before moving on
	cudaDeviceSynchronize();
}
