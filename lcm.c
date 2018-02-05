#include<stdio.h>
#include<stdlib.h>



int gcd(int a, int b) {

	int d;

        for(int i = 1; i <= a  && i<= b; i++)
        {
                if (a%i ==0 && b%i==0)
                {
                        d = i;
                }
        }
	return d;
}


void main() {

	int A, B, C, lcm;
	//Does not work when numbers are both in hundred millions
        printf("Enter the first number: ");
        scanf("%d", &A);
        printf("Enter the second number: ");
        scanf("%d", &B);

	C = gcd(A, B);
	
	lcm = (A*B)/C;
	printf("The least common multiple of %d and %d is %d.\n",A,B,lcm);
}
