#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INTSIZE 4

void createBin (int n, int tam) {
   
	int numberOfInts = (tam * 1024 * 1024) / INTSIZE, number;
	char arq[50];
		
	int i,j;
	srand(time(0));
	
	for ( i = 0; i < n; i++ ) {
		
	  sprintf(arq,"arquivo%d.bin",i+1);
	  FILE *fp = fopen(arq, "w+");
	  
	  for (j=0;j<numberOfInts;j++){
		number = rand() % 1000;
		fwrite( &number, sizeof(number), 1, fp); 
	  }
	fclose(fp);
	}
}
