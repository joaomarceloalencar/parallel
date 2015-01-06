#include <iostream>

int main(int argc, char *argv[]) 
{
   int sum = 0;
   int value = 0;
   while (value != -1) 
   {
      sum += value;
      if (sum > 100) 
      {
          break;
      }
      std::cin >> value;
      if (value == -2) 
      {
          sum = 0;   
      }
   }
   std::cout << sum << "\n";
   return 0;
}
