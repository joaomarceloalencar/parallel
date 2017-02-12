#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
int main(int argc, char **argv) 
{
	unsigned long int n;
	double r, pi = 1;
	int i, j;
 
        if (argc != 2) {
		printf("Usage: <calc_pi> <number of iterations>\n");
		exit (1);
	}
 
	n = strtol(argv[1], NULL, 10);
	if (!n) {
		printf("ERROR: Invalid number of iterations!\n");
		exit (2);
	}
        # pragma omp parallel for private(j,r) schedule(dynamic) reduction(*:pi) 
	for (i = 0; i < n; i++) {
 
		r = 2;
		for (j = i; j ; j--)
			r = 2 + sqrt(r);
		r = sqrt(r);
 
		pi *= (r / 2);
	}  
 
	pi = 2 / pi;
 
	printf("Aproximated value of PI = %1.16f\n", pi);
 
	return 0;
}
