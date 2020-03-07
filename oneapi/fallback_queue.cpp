#include <CL/sycl.hpp>
#include <iostream>
#define N 1024
#define M 32

using namespace cl::sycl;
int main () {
   queue gpuQueue(gpu_selector{});
   queue hostQueue(host_selector{});

   buffer<int, 2> buf(range<2>(N, N));
   gpuQueue.submit([&] (handler &cgh) {
            auto bufacc = buf.get_access<access::mode::read_write>(cgh);
            cgh.parallel_for<class ndim>(nd_range<2>(range<2>(N, N), range<2>(M, M)), [=](nd_item<2> i) {
                      id<2> ind = i.get_global_id();
                      bufacc[ind[0]][ind[1]] = ind[0] + ind[1];
                  });
         }, hostQueue);
   auto bufacc1 = buf.get_access<access::mode::read>();
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         if (bufacc1[i][j] != i+j) {
            std::cout<< "Wrong result\n";
         }
      }
   }
   std::cout <<"Correct results\n";
   return 0;
}
