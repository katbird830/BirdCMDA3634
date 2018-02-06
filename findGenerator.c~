#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void main() {

	int A, p, g, redFlag;

	printf("Enter a prime number: ");
	scanf("%d", &A);

	p = A-1;
	redFlag = 0;

	for (int i = 2; i < A; i++) {

		for (int j=1; j < p; j++) {
			//Currently causes an overflow since numbers grow super quickly in Z_n that's first generator is not 2
			int c = (int)pow(i, j);
			if (c % A == 1) {
				redFlag = 1;
				break;
			}
		}
		//program will always find the smallest generator in the set
		if (redFlag == 0) {
			g = i;
			break;
		}
	}
	//I did not make a provision for the input being a non prime number that will definitely make it mess up.
	printf("%d is a generator of Z_%d.\n",g,A);
}
