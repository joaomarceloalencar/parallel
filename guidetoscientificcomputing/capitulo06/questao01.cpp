#include <iostream>

void printValue(int *ptr);

int main(int argc, char *argv[])
{
   int n = 5;

   printValue(&n);

   return 0;
}

void printValue(int *ptr) 
{
   std::cout<< *ptr << "\n";
   
   return;
}
