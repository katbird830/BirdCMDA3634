#include<stdio.h>
#include<stdlib.h>


void main() {

	int A, B, gcd;

        printf("Enter the first number: ");
        scanf("%d", &A);
        printf("Enter the second number: ");
        scanf("%d", &B);
	//Noticible computation time when both numbers are more than 10^9
        for(int i = 1; i <= A && i<= B; i++)
        {
                if (A%i ==0 && B%i==0)
                {
                        gcd = i;
                }
        }

	if (gcd == 1) {
		printf("%d and %d are coprime.\n",A,B);
	}
	else {
		printf("%d and %d are not coprime.\n",A,B);
	}
}
