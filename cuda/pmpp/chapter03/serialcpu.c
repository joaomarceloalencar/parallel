#include <stdio.h>
#include <stdlib.h>

void vecAdd(float *h_A, float *h_B, float* h_C, int n)
{
   int i;
   for (i = 0; i < n; i++)
      h_C[i] = h_A[i] + h_B[i];
 
}

int main(int argc, char *argv[])
{
   int n = atoi(argv[1]);
   printf("Size: %d\n", n);

   float *h_A = (float *) malloc(n * sizeof(float));
   float *h_B = (float *) malloc(n * sizeof(float));
   float *h_C = (float *) malloc(n * sizeof(float));

   int i;
   for (i = 0; i < n; i++)
   {
      h_A[i] = 1;
      h_B[i] = 1;
   }

   vecAdd(h_A, h_B, h_C, n);  
 
   printf("%.2f\n", h_C[0]);

   free(h_A);
   free(h_B);
   free(h_C);
   
   return 0;
}
