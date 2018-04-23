#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "functions.h"


int main (int argc, char **argv) {

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
	
  // find the secret key
  if (x==0 || modExp(g,x,p)!=h) {
    printf("Finding the secret key...\n");
    double startTime = clock();
    for (unsigned int i=0;i<p-1;i++) {
      if (modExp(g,i+1,p)==h) {
        printf("Secret key found! x = %u \n", i+1);
        x=i+1;
      } 
    }
    double endTime = clock();

    double totalTime = (endTime-startTime)/CLOCKS_PER_SEC;
    double work = (double) p;
    double throughput = work/totalTime;

    printf("Searching all keys took %g seconds, throughput was %g values tested per second.\n", totalTime, throughput);
  }

  /* Q3 After finding the secret key, decrypt the message */
	unsigned int charsPerInt = (n-1)/8;
	unsigned int Nchars = Nints*charsPerInt;

	int bufferSize = 1024;
	unsigned char *message1 = (unsigned char *) malloc(bufferSize*sizeof(unsigned char));

	ElGamalDecrypt(Zmessage, b, Nints, p, x);
	
	convertZToString(Zmessage, Nints, message1, Nchars);

	printf("Decrypted Message = \"%s\"\n", message1);
	printf("\n");

	free(b);
	free(Zmessage);
	return 0;
}
