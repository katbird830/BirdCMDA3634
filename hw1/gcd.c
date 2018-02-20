#include<stdio.h>
#include<stdlib.h>



void main() {

	int A, B, gcd;
	
	printf("Enter the first number: ");
	scanf("%d", &A);
	printf("Enter the second number: ");
	scanf("%d", &B);
	//Again there is noticible computation lag when numbers are both in the hundred millions
	for(int i = 1; i <= A && i<= B; i++)
	{
		if (A%i ==0 && B%i==0)
		{
			gcd = i;
		}
	}

	printf("The greatest common divisor of %d and %d is %d\n.", A,B, gcd);
}
