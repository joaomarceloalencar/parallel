#include <iostream>
#include <cmath>

double mean(double vtr[], int size);
double standardDeviation(double vtr[], int size);


int main(int argc, char *argv[])
{
   double *vtr = new double[10];
  
   for (int i = 0; i < 10; i++)
   {
      vtr[i] = i;
   }

   std::cout<< mean(vtr, 10) << " " << standardDeviation(vtr, 10) << "\n";

   delete [] vtr;
   return 0;
}


double mean(double vtr[], int size)
{
   double sum = 0.0;
   double m = 0.0;

   for (int i = 0; i < size; i++) 
   {
      sum +=vtr[i];
   }
   
   m = sum / size;
   return m;
}

double standardDeviation(double vtr[], int size)
{
   double m = mean(vtr, size);
   double term = 0.0;

   for (int i = 0; i < size; i++) 
   {
      term += (vtr[i] - m) * (vtr[i] - m);
   }

   double sigma = std::sqrt(term / (size - 1));

   return sigma;
}



