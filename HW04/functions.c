#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "functions.h"

//compute a*b mod p safely
unsigned int modprod(unsigned int a, unsigned int b, unsigned int p) {
  unsigned int za = a;
  unsigned int ab = 0;

  while (b > 0) {
    if (b%2 == 1) ab = (ab +  za) % p;
    za = (2 * za) % p;
    b /= 2;
  }
  return ab;
}

//compute a^b mod p safely
unsigned int modExp(unsigned int a, unsigned int b, unsigned int p) {
  unsigned int z = a;
  unsigned int aExpb = 1;

  while (b > 0) {
    if (b%2 == 1) aExpb = modprod(aExpb, z, p);
    z = modprod(z, z, p);
    b /= 2;
  }
  return aExpb;
}

//returns either 0 or 1 randomly
unsigned int randomBit() {
  return rand()%2;
}

//returns a random integer which is between 2^{n-1} and 2^{n}
unsigned int randXbitInt(unsigned int n) {
  unsigned int r = 1;
  for (unsigned int i=0; i<n-1; i++) {
    r = r*2 + randomBit();
  }
  return r;
}

//tests for primality and return 1 if N is probably prime and 0 if N is composite
unsigned int isProbablyPrime(unsigned int N) {

  if (N%2==0) return 0; //not interested in even numbers (including 2)

  unsigned int NsmallPrimes = 168;
  unsigned int smallPrimeList[168] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 
                                37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 
                                79, 83, 89, 97, 101, 103, 107, 109, 113, 
                                127, 131, 137, 139, 149, 151, 157, 163, 
                                167, 173, 179, 181, 191, 193, 197, 199, 
                                211, 223, 227, 229, 233, 239, 241, 251, 
                                257, 263, 269, 271, 277, 281, 283, 293, 
                                307, 311, 313, 317, 331, 337, 347, 349, 
                                353, 359, 367, 373, 379, 383, 389, 397, 
                                401, 409, 419, 421, 431, 433, 439, 443, 
                                449, 457, 461, 463, 467, 479, 487, 491, 
                                499, 503, 509, 521, 523, 541, 547, 557, 
                                563, 569, 571, 577, 587, 593, 599, 601, 
                                607, 613, 617, 619, 631, 641, 643, 647, 
                                653, 659, 661, 673, 677, 683, 691, 701, 
                                709, 719, 727, 733, 739, 743, 751, 757, 
                                761, 769, 773, 787, 797, 809, 811, 821, 
                                823, 827, 829, 839, 853, 857, 859, 863, 
                                877, 881, 883, 887, 907, 911, 919, 929, 
                                937, 941, 947, 953, 967, 971, 977, 983, 
                                991, 997};

  //before using a probablistic primality check, check directly using the small primes list
  for (unsigned int n=1;n<NsmallPrimes;n++) {
    if (N==smallPrimeList[n])   return 1; //true
    if (N%smallPrimeList[n]==0) return 0; //false
  }

  //if we're testing a large number switch to Miller-Rabin primality test
  unsigned int r = 0;
  unsigned int d = N-1;
  while (d%2 == 0) {
    d /= 2;
    r += 1;
  }

  for (unsigned int n=0;n<NsmallPrimes;n++) {
    unsigned int k = smallPrimeList[n];
    unsigned int x = modExp(k,d,N);

    if ((x==1) || (x==N-1)) continue;

    for (unsigned int i=1;i<r-1;i++) {
      x = modprod(x,x,N);
      if (x == 1) return 0; //false
      if (x == N-1) break;
    }
    // see whether we left the loop becasue x==N-1
    if (x == N-1) continue; 

    return 0; //false
  }
  return 1; //true
}

//Finds a generator of Z_p using the assumption that p=2*q+1
unsigned int findGenerator(unsigned int p) {
  unsigned int g;
  unsigned int q = (p-1)/2;

  do {
    //make a random number 1<= g < p
    g = randXbitInt(32)%p; //could also have passed n to findGenerator
  } while (g==0 || (modExp(g,q,p)==1) || (modExp(g,2,p)==1));
  
  return g;
}

void setupElGamal(unsigned int n, unsigned int *p, unsigned int *g, 
                                  unsigned int *h, unsigned int *x) {

  /* Use isProbablyPrime and randomXbitInt to find a new random n-bit prime number 
     which satisfies p=2*q+1 where q is also prime */
  unsigned int q;
  do {
    *p = randXbitInt(n);
    q = (*p-1)/2;
  } while (!isProbablyPrime(*p) || !isProbablyPrime(q));


  /* Use the fact that p=2*q+1 to quickly find a generator */
  *g = findGenerator(*p);
  
  //pick a secret key,  x
  *x = randXbitInt(n)%(*p);

  //compute h
  *h = modExp(*g,*x,*p);
  
  printf("ElGamal Setup successful.\n");
  printf("p = %u. \n", *p);  
  printf("g = %u is a generator of Z_%u \n", *g, *p);  
  printf("Secret key: x = %u \n", *x);
  printf("h = g^x = %u\n", *h);
  printf("\n");
}

void ElGamalEncrypt(unsigned int *m, unsigned int *a, unsigned int Nints, 
                    unsigned int p, unsigned int g, unsigned int h) {

  /* Q2.1 Parallelize this function with OpenMP   */
	#pragma omp parallel for
  for (unsigned int i=0; i<Nints;i++) {
    //pick y in Z_p randomly
    unsigned int y;
    do {
      y = randXbitInt(32)%p;
    } while (y==0); //dont allow y=0

    //compute a = g^y
    a[i] = modExp(g,y,p);

    //compute s = h^y
    unsigned int s = modExp(h,y,p);

    //encrypt m by multiplying with s
    m[i] = modprod(m[i],s,p);  
  }
}

void ElGamalDecrypt(unsigned int *m, unsigned int *a, unsigned int Nints,
                    unsigned int p, unsigned int x) {

  /* Q2.1 Parallelize this function with OpenMP   */
	#pragma omp parallel for
  for (unsigned int i=0; i<Nints;i++) {
    //compute s = a^x
    unsigned int s = modExp(a[i],x,p);

    //compute s^{-1} = s^{p-2}
    unsigned int invS = modExp(s,p-2,p);
    
    //decrypt message by multplying by invS
    m[i] = modprod(m[i],invS,p);
  }
}

//Pad the end of string so its length is divisible by Nchars
// Assume there is enough allocated storage for the padded string 
void padString(unsigned char* string, unsigned int charsPerInt) {

  /* Q1.2 Complete this function   */
	int length = strlen(string);
	int remainder = length%charsPerInt;
	if (remainder != 0) {
		int padLen = charsPerInt - remainder;
		char pad = ' ';
		for (int i=0; i<padLen; i++) {
			string[length+i] = pad;
		}
		string[length+padLen+1] = '\0';
	}
}


void convertStringToZ(unsigned char *string, unsigned int Nchars,
                      unsigned int  *Z,      unsigned int Nints) {
	//printf("Nints/Nchars: %u.\n", (Nints/Nchars));
  /* Q1.3 Complete this function   */
  /* Q2.2 Parallelize this function with OpenMP   */
	int chars = (Nchars/Nints);
	printf("chars is %d\n", chars);
	#pragma omp parallel for
	for (int i=0; i<=Nints; i++) {
		if ((Nchars/Nints) == 1) {
			//grabs each character of string and casts to int
			unsigned int a = (unsigned int) string[i];
			//sets ith entry to casted char
			Z[i] = a;
			//troubleshooting
			printf("entry in Z[]: %u \n", Z[i]);
			printf("(Nchars/Nints) is %u \n", (Nchars/Nints));
		}
		else if ((Nchars/Nints) == 2) {
			//grabs first two characters of string and shifts the first
			unsigned int a = (unsigned int) string[2*i];
			unsigned int b = (unsigned int) string[2*i+1];
			printf("i: %d\n", i);
			printf("Nints: %d\n", Nints);
			Z[i] = (a*256) + b;
			printf("entry in Z: %u \n", Z[i]);
			printf("(Nchars/Nints) is %u \n", (Nchars/Nints));
		}
		else {
			Z[i] = ((unsigned int) string[3*i] << 16) + ((unsigned int) string[3*i+1]*256)+((unsigned int) string[3*i+2]);
			printf("entry in Z: %u \n", Z[i]);
		}
	}

}


void convertZToString(unsigned int  *Z,      unsigned int Nints,
                      unsigned char *string, unsigned int Nchars) {

  /* Q1.4 Complete this function   */
  /* Q2.2 Parallelize this function with OpenMP   */
	#pragma omp parallel for
	for (int i=0; i<Nints; i++) {
		if (Nchars/Nints == 1) {
			unsigned char a = (unsigned char) Z[i]%256;
			string[i] = a;
			printf("a is %c \n", a);
			printf("string is: %c \n", string[i]);
		}
		else if (Nchars/Nints == 2) {
			//take mode to grab last character added
			unsigned int b = Z[i]%256;
			//cast and shift right
			unsigned char a = (unsigned char) ((Z[i]-b)/256);
			unsigned char bLetter = (unsigned char) b;

			string[2*i] = a;
			string[2*i+1] = bLetter;
			printf("Z[%d]: %d\n", i, Z[i]);
			printf("a:%d, b:%d, string[%d] is: %d \n", a, bLetter, i, string[i]);
		}
		else {
			unsigned int c = Z[i]%256;
			unsigned int b = ((Z[i]-c)/256)%256;
			unsigned int a = ((Z[i]-b-c)>>16);

			string[3*i] = (unsigned char)a;
			string[3*i+1] = (unsigned char)b;
			string[3*i+2] = (unsigned char)c;
		}
	}
}

