#include <stdio.h>
#include <omp.h>

int main() {
	printf("Olá Mundo:");
	#pragma omp parallel
	   printf(" %d", omp_get_thread_num());
	printf("\n");
	return 0;
}