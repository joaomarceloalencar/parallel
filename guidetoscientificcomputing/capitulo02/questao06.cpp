#include <iostream>
#include <cassert>
#include <cmath>
#include <limits>

int main(int argc, char *argv[]) 
{
   double x_prev = -1;
   double x_next = 0;
   double fx = std::exp(0) + std::pow(0,3) - 5;
   //std::cout <<  std::numeric_limits<double>::epsilon() << "\n";
 		
   while (std::abs(x_next - x_prev) >= std::numeric_limits<double>::epsilon()) 
   {
      x_prev = x_next;
      x_next = x_prev - fx / (std::exp(x_prev) + 3 * std::pow(x_prev,2));
      fx = std::exp(x_next) + std::pow(x_next,3) - 5;
      std::cout << "x[i] = " << x_next << " : " << "f(x) = " << fx << "\n" ;
   }

   return 0;
}
