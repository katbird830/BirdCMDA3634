#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "functions.h"

int main (int argc, char **argv) {

	//seed value for the randomizer 
  double seed;
  
 // seed = clock(); //this will make your program run differently everytime
  seed = 0; //uncomment this and you program will behave the same everytime it's run
  
  srand48(seed);

	//testing modProd function
	//unsigned int c = modprod(13, 74, 111);
	//printf("The result is %u.\n", c);

	//begin by getting user's input
	unsigned int n;

	printf("Enter a number of bits: ");
	scanf("%u",&n);

	//make sure the input makes sense
	if ((n<2)||(n>30)) {
		printf("Unsupported bit size.\n");
		return 0;  	
	}

	unsigned int p = randXbitInt(n);

  /* Q2.2: Use isProbablyPrime and randomXbitInt to find a random n-bit prime number */
	
	while (isProbablyPrime(p) == 0) {
		//printf("p =  %u isn't prime.\n", p);
		p = randXbitInt(n);
	}
	
	printf("p = %u is probably prime.\n",p);

  /* Q3.2: Use isProbablyPrime and randomXbitInt to find a new random n-bit prime number 
     which satisfies p=2*q+1 where q is also prime */
	unsigned int p1 = randXbitInt(n);
	unsigned int q = (p1-1)/2;

	while (isProbablyPrime(q) == 0 || isProbablyPrime(p1) == 0) {

		p1 = randXbitInt(n);
		q = (p1-1)/2;
	}

	printf("p = %u is probably prime and equals 2*q + 1. q = %u and is also probably prime.\n", p1, q);  

	/* Q3.3: use the fact that p=2*q+1 to quickly find a generator */
	unsigned int g = findGenerator(p1);

	printf("g = %u is a generator of Z_%u \n", g, p1);  

	/*Extra credit stuff*/
	unsigned int x = (rand()%p);
	unsigned int h = modExp(g, x, p);

	unsigned int x1, g1;

	//has a lot of computation time when a number of bits input is < 21
	//was faster when I used pow() function instead of modExp()
	for (int i = 0; i < p; i++) {

		g1 = modExp(g, i, p);

		if (h == g1) {
			x1 = i;
			break;
		}
	}
	//This seems to work although for the higher bit inputs x = 2 consistant
	printf("A possible secret key x for h = %u and g = %u is x = %u.\n", h, g, x1);

	return 0;
}
