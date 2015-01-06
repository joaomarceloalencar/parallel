#include <iostream>

int main( int argc, char **argv ) 
{
   int* p_v1;
   int* p_v2;
   int scalar = 0;

   for ( int k = 0 ; k < 1000000000; k++)
   {
      p_v1 = new int[3];
      p_v2 = new int[3];

      p_v1[0] = p_v1[1] = p_v1[2] = 1;
      p_v2[0] = p_v2[1] = p_v2[2] = 2;

      for ( int i = 0; i < 3; i++ ) 
      {
         scalar += p_v1[i] * p_v2[i];
      }

      // std::cout << scalar << '\n';

      delete[] p_v1;
      delete[] p_v2;
   }
   return 0;
}
