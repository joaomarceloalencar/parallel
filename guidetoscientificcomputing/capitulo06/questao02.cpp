#include <iostream>

void addInt(int *ptr);

int main(int argc, char *argv[])
{
   int n = 5;

   addInt(&n);
   std::cout<< n << "\n";

   return 0;
}

void addInt(int *ptr) 
{
   (*ptr)++;
   return;
}
