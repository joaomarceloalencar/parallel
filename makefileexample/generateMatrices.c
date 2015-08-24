#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "matrix.h"

int main(int argc, char *argv[]) 
{
   if (argc != 7) 
   {
      printf("usage: generateMatrices filenameA rowsA columnsA filenameB rowsB columnsB\n");
      return -1;
   }
   
   int rowsA = atoi(argv[2]);
   int columnsA = atoi(argv[3]);

   int rowsB = atoi(argv[5]);
   int columnsB = atoi(argv[6]);              

   if (columnsA != rowsB) 
   {
      printf("Matrices dimensions don't match!\n");
      return -2;
   }
   
   createRandomMatrixFile(argv[1], rowsA, columnsA);
   createRandomMatrixFile(argv[4], rowsB, columnsB);

   return 0;
}
