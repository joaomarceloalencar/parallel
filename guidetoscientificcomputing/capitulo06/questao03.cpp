#include <iostream>

void swapPointers(double *ptr1, double *ptr2);
void swapReferences(double &ref1, double &ref2);

int main(int argc, char *argv[])
{
   double a, b;
   a = 1.0;
   b = 2.0; 

   std::cout<< a << " " << b << "\n";
   swapPointers(&a,&b);
   std::cout<< a << " " << b << "\n";
   swapReferences(a,b);
   std::cout<< a << " " << b << "\n";

   return 0;
}

void swapPointers(double *ptr1, double *ptr2)
{
   double temp;
   temp = *ptr1;
   *ptr1 = *ptr2;
   *ptr2 = temp;

   return;
}

void swapReferences(double &ref1, double &ref2)
{
   double temp;
   temp = ref1;
   ref1 = ref2;
   ref2 = temp;

   return;
}

