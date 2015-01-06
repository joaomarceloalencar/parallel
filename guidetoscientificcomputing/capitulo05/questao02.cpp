#include <iostream>

int main( int argc, char **argv ) 
{
   int i = 5;
   int j = 8;

   int* p_i;
   int* p_j;

   p_i = &i;
   p_j = &j;

   int* temp = new int;
   *temp = *p_i;
   *p_i = *p_j;
   *p_j = *temp;

   std::cout << i << " " << j << '\n';
   
   delete temp;
   return 0;
}
