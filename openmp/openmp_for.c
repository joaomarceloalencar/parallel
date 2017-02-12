#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{
	int N = 1000;
	int i, tid, j;
	int num = 0;

	# pragma omp parallel for private(j) reduction(+:num)
	for (i = 0; i < N; i++) {
		tid = omp_get_thread_num();
	        // printf("Thread: %d, i: %d\n", tid, i, j);	
                
		for (j = 0; j < N ; j++) {
	 	   // printf("Thread: %d, i: %d, j: %d, num: %d\n", tid, i, j, num);	
                   // # pragma omp critical
		   num += 1;
		}
	}
	printf("%d\n", num); 
	return 0;
}

