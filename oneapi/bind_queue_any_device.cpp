#include <CL/sycl.hpp>
#include <iostream>

using namespace cl::sycl;

int main () {
    // Create queue on whatever default device SYCL
    // implementation chooses. Implicit use of the default_selector()
    // Bind to default device.
    // queue myQueue;
    // Bind to host device
    queue myQueue(host_selector{});

    std::cout << "Selected device: " << myQueue.get_device().get_info<info::device::name>() << "\n";

    return 0;
}