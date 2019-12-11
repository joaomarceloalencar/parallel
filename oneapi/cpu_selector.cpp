#include <CL/sycl.hpp>
#include <iostream>

using namespace cl::sycl;

int main () {
    // Create queue on whatever default device SYCL
    // CPU Selector
    queue myQueue(cpu_selector{});

    std::cout << "Selected device: " << myQueue.get_device().get_info<info::device::name>() << "\n";
    std::cout << "   -> Device vendor: " << myQueue.get_device().get_info<info::device::vendor>() << "\n"; 
    return 0;
}