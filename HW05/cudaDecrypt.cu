#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "cuda.h"
#include "functions.c"

__device__ unsigned int dProdMod(unsigned int a, unsigned int b, unsigned int p) {
	unsigned int za = a;
	unsigned int ab = 0;

	while (b>0) {
		if (b%2 == 1) ab = (ab+za)%p;
		za = (2*za)%p;
		b/=2;
	}
	return ab;
}

__device__ unsigned int dExpMod(unsigned int a, unsigned int b, unsigned int p) {
	unsigned int z = a;
	unsigned int aExpb = 1;

	while (b>0) {
		if (b%2 == 1) {
			aExpb = dProdMod(aExpb, z, p);
		}
		z = dProdMod(z, z, p);
		b /= 2;
	}
	return aExpb;
}

__global__ void kernalFindKey(unsigned int p, unsigned int g, unsigned int h, unsigned int *x) {

	unsigned int threadid = (unsigned int) threadIdx.x;
	unsigned int blockid = (unsigned int) blockIdx.x;
	unsigned int Nblock = (unsigned int) blockDim.x;

	unsigned int id = threadid + blockid*Nblock + 1;

	if (dExpMod(g, id, p) == h) {
		*x = id;
	}
}

int main (int argc, char **argv) {

  /* Part 2. Start this program by first copying the contents of the main function from 
     your completed decrypt.c main function. */
	  //declare storage for an ElGamal cryptosytem
  unsigned int n, p, g, h, x;
  unsigned int Nints;

  //get the secret key from the user
  printf("Enter the secret key (0 if unknown): "); fflush(stdout);
  char stat = scanf("%u",&x);

  printf("Reading file.\n");

  /* Q3 Complete this function. Read in the public key data from public_key.txt
    and the cyphertexts from messages.txt. */
	FILE *f;
	f = fopen("public_key.txt", "r");
	fscanf(f, "%u\n%u\n%u\n%u", &n, &p, &g, &h);
	fclose(f);

	FILE *fr;
	fr = fopen("message.txt", "r");
	fscanf(fr, "%u\n", &Nints);

	//allocating memory for the (m, a)
	unsigned int *b = (unsigned int *) malloc(Nints*sizeof(unsigned int));
	unsigned int *Zmessage = (unsigned int *) malloc(Nints*sizeof(unsigned int));

	//filling b and Zmessage from txt file
	for (unsigned int i = 0; i<Nints; i++) {
		fscanf(fr, "(%u,%u)\n", &Zmessage[i], &b[i]);
	}
	fclose(fr);

	//Q4
	unsigned int *d_x;

	cudaMalloc(&d_x,1*sizeof(unsigned int));

	int Nthreads = 32;
	int Nblocks = (p+Nthreads-1)/Nthreads;
	
	double startTime = clock();
  // find the secret key
	if (x==0 || modExp(g,x,p)!=h) {
    	printf("Finding the secret key...\n");
    	double startTime = clock();
    	kernalFindKey <<<Nblocks ,Nthreads >>>(p, g, h, d_x);
	}
    double endTime = clock();

    double totalTime = (endTime-startTime)/CLOCKS_PER_SEC;
    double work = (double) p;
    double throughput = work/totalTime;

    printf("Searching all keys took %g seconds, throughput was %g values tested per second.\n", totalTime, throughput);
  
	cudaMemcpy(&x, d_x,1*sizeof(unsigned int), cudaMemcpyDeviceToHost);

	unsigned int charsPerInt = (n-1)/8;
	unsigned int Nchars = Nints*charsPerInt;

	int bufferSize = 1024;
	unsigned char *message1 = (unsigned char *) malloc(bufferSize*sizeof(unsigned char));

	ElGamalDecrypt(Zmessage, b, Nints, p, x);
	
	convertZToString(Zmessage, Nints, message1, Nchars);

	printf("Decrypted Message = \"%s\"\n", message1);
	printf("\n");

	cudaFree(d_x);

	free(b);
	free(Zmessage);
  
  return 0;
}
