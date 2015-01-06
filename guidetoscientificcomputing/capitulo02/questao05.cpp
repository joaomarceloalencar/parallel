#include <iostream>
#include <cassert>

int main(int argc, char *argv[]) 
{
   int A[2][2] = {{4, 10},
                  {1, 1}};   
   double factor = 1.0 / (A[0][0] * A[1][1] - A[0][1] * A[1][0]);
   double Ai[2][2] = {{factor * A[1][1], -factor * A[0][1]},
                   {-factor * A[1][0], factor * A[0][0]}};
   std::cout << Ai[0][0] << ":" << Ai[0][1] << ":" << Ai[1][0] << ":" << Ai[1][1] << "\n";

   assert(A[0][0] * A[1][1] - A[0][1] * A[1][0]);

   return 0;
}
