#include<stdio.h>
#include<stdlib.h>
#include<math.h>


int N;
void main() {

	printf("Enter an upper bount:");
	scanf("%d",&N);

	//make storage for flags
	int *isPrime = (int*) malloc(N*sizeof(int));

	//initialize everything to true
	for (int n=0; n<N;n++) isPrime[n] = 1;

	int sqrtN = (int) sqrt(N);

	for (int i=2; i<sqrtN; i++)
	{
		if(isPrime[i]){//if i is prime
			for (int j=i*i; j<N; j+=i) {
				isPrime[j]=0;//set j to not prime
			}
		}
	}
	//count the number of primes we found
	int count = 0;
	for(int n=0; n<N; n++) {
		if (isPrime[n]) {
			count++;
		}
	}

	//make a list of them
	int *primes = (int*) malloc(count*sizeof(int));
	//loop once more to 
	count = 0;
	for (int n=0; n<N; n++) {
		if (isPrime[n]) {
			primes[count++]=n;
		}
	}
	
	//print out what found
	for (int n=0; n<count; n++) printf("The %d-th prime is %d\n",n,primes[n]);
	
//clean up
free(isPrime);
free(primes);
	
}
