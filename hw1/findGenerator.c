#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void main() {

	int A, p, g, c, redFlag;

	printf("Enter a prime number: ");
	scanf("%d", &A);

	p = A-1;
	redFlag = 0;
	c = 0;

	for (int i = 2; i < A; i++) {
		redFlag = 0;
		for (int j=1; j < p; j++) {
			//Currently causes an overflow when first generator is
			//not 2 since numbers grow very quickly
			//When running debugger it indicates that the pow
			// function does not exist?
			c = pow(i, j);
			printf("%d is the base at power %d for %d.\n", c, j, A);
			printf("cmod A =  %d\n", c%A); 
			if (c % A == 1) {
				redFlag = 1;
				break;
			}
		}
		printf("RedFlag is %d.\n", redFlag);
		//program will always find the smallest generator in the set
		if (redFlag == 0) {
			g = i;
			break;
		}
	}
	//I did not make a provision for the input being a non 
	//prime number that will definitely make it break.
	printf("%d is a generator of Z_%d.\n",g,A);
}
