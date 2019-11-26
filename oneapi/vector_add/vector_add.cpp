#include <CL/sycl.hpp>
#include <iostream>
#include <array>
#define SIZE 1024

using namespace cl::sycl;

int main() {
    std::array<int, SIZE> a, b, c;
    for (int i = 0; i < SIZE; ++i) {
        a[i] = i;
        b[i] = -i;
        c[i] = i;
    }
    {
        range<1> a_size{SIZE};
        auto platforms = platform::get_platforms();
        for (auto &platform : platforms) {
            std::cout << "Plataform: "
               << platform.get_info<info::platform::name>() << std::endl;
            auto devices = platform.get_devices();
            for (auto &device : devices) {
                std::cout << " Device: "
                   << device.get_info<info::device::name>()
                   << std::endl;
            }
        }
        queue d_queue;
        buffer<int, 1> a_device(a.data() , a_size);
        buffer<int, 1> b_device(a.data() , a_size);
        buffer<int, 1> c_device(a.data() , a_size);

        d_queue.submit([&](handler &cgh) {
            auto c_res = c_device.get_access<access::mode::write>(cgh);
            auto a_in = c_device.get_access<access::mode::read>(cgh);
            auto b_in = c_device.get_access<access::mode::read>(cgh);
            cgh.parallel_for<class ex1>(a_size, [=](id<1> idx) {
                c_res[idx] = a_in[idx] + b_in[idx];
                });
        });
    }
}