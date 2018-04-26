#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "functions.h"

int main (int argc, char **argv) {

	//seed value for the randomizer 
  //double seed = clock(); //this will make your program run differently everytime
  double seed = 0; //uncomment this and your program will behave the same everytime it's run

  srand(seed);

  int bufferSize = 1024;
  unsigned char *message = (unsigned char *) malloc(bufferSize*sizeof(unsigned char));

  printf("Enter a message to encrypt: ");
  int stat = scanf (" %[^\n]%*c", message); //reads in a full line from terminal, including spaces

  //declare storage for an ElGamal cryptosytem
  unsigned int n, p, g, h;

  printf("Reading file.\n");

  /* Q2 Complete this function. Read in the public key data from public_key.txt,
    convert the string to elements of Z_p, encrypt them, and write the cyphertexts to 
    message.txt */
	//unsigned int *stor = (unsigned in *)malloc(sizeof(unsigned int));
	FILE *fr;
	fr = fopen("public_key.txt", "r");
	fscanf(fr, "%u\n%u\n%u\n%u", &n, &p, &g, &h);
	fclose(fr);

	printf("n= %u\n", n);
	printf("p= %u\n", p);
	printf("g= %u\n", g);
	printf("h= %u\n", h);

	unsigned int charsPerInt = (n-1)/8;

	padString(message, charsPerInt);

	unsigned int Nints = strlen(message)/charsPerInt;
	
	unsigned int Nchars = strlen(message);
	
	unsigned int *a = (unsigned int *) malloc(Nints*sizeof(unsigned int));
	unsigned int *Zmess = (unsigned int *) malloc(Nints*sizeof(unsigned int));

	padString(message, charsPerInt);

	convertStringToZ(message, Nchars, Zmess, Nints);

	ElGamalEncrypt(Zmess, a, Nints, p, g, h);

	FILE *f;
	f = fopen("message.txt", "w");
	fprintf(f, "%u\n", Nints);
	for (unsigned int i = 0; i<Nints; i++) {
		fprintf(f, "%u %u\n", Zmess[i], a[i]);
	}
	fclose(f);
	
	free(a);
	free(Zmess);
	free(message);
  return 0;
}
