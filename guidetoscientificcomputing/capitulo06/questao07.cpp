#include <iostream>
#include <cmath>

double pnorm(double *vtr, int size, double p = 2.0);

int main(int argc, char *argv[])
{
   double *vtr = new double[10];
  
   for (int i = 0; i < 10; i++)
   {
      vtr[i] = i;
   }

   std::cout<< pnorm(vtr, 10) << "\n";

   delete [] vtr;
   return 0;
}


double pnorm(double *vtr, int size, double p)
{
   double norm = 0.0;
   
   for (int i = 0; i < size; i++)
   {
      norm += std::pow(std::abs(vtr[i]), p);
   }
   norm = std::pow(norm, (1/p));
   return norm;
}
