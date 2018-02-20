#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void main() {

	int n, redFlag;

	printf("Enter a number: ");
	scanf("%d", &n);
	//Since I only count up to half the number this cuts down on computation
	//Program begins to have noticible computation time in the upper 900 millions
	for (int i = 2; i <= n/2; i++) {
		if (n%i == 0) {
			redFlag = 1;
		}
	}
	if (redFlag == 1) {
		printf("%d is not a prime number.\n", n);
	}
	else {
		printf("%d is a prime number.\n", n);
	}
}
