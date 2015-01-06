#include <iostream>

int main( int argc, char **argv ) 
{
   int i = 5;
   int* p_j;
  
   p_j = &i;

   *p_j = *p_j * 5;

   int* p_k = new int;

   *p_k = i;

   std::cout << *p_k << '\n';

   return 0;
}
