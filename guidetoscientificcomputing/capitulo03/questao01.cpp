#include <iostream>
#include <fstream>
#include <cassert>

int main( int argc, char *argv[] )
{
   double x[4] = {0.0, 1.0, 1.0, 0.0};
   double y[4] = {0.0, 0.0, 1.0, 1.0};
   
   std::ofstream write_output("x_and_y.dat");
   assert(write_output.is_open()); 

   write_output.setf(std::ios::scientific);
   write_output.setf(std::ios::showpos);
   write_output.precision(10);

   for ( int i = 0; i < 4; i++ ) 
   {
      write_output << x[i] << " ";
   }

   write_output << "\n";
   write_output.flush();

   for ( int i = 0; i < 4; i++ ) 
   {
      write_output << y[i] << " ";
   }

   write_output << "\n";
   write_output.flush();


   return 0;
}
