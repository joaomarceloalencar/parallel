#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>

int main( int argc, char *argv[] )
{
   if ( argc != 2 ) 
   {
      std::cout << "usage: " << argv[0] << " N " << "\n";
      return 1;
   } 
    
   int N = atoi( argv[1] );
   assert( N > 1 );
   
   double h = N / 100.0;
   double y = 1;
   
   std::ofstream write_output("xy.dat");
   assert(write_output.is_open()); 
   write_output.setf(std::ios::scientific);
   write_output.setf(std::ios::showpos);
   write_output.precision(4);

   for ( double i = 0; i < N; i += h )
   {
      y += h * (-i);   
      write_output << i << " " << y << "\n";
   }


   write_output << "\n";
   write_output.flush();
   write_output.close();

   return 0;
}
