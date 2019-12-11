#include <CL/sycl.hpp>
#include <CL/sycl/intel/fpga_extensions.hpp> // fpga_selector
#include <iostream>
#include <string>

using namespace cl::sycl;

void output_dev_info(const device& dev, std::string selector_name) {
    std::cout << selector_name << ": Selected device: " << dev.get_info<info::device::name>() << "\n";  
    std::cout << "                -> Device vendor: " << dev.get_info<info::device::vendor>() << "\n";  
}

int main() {
    output_dev_info(device(default_selector{}), "default_selector");
    output_dev_info(device(host_selector{}), "host_selector");
    output_dev_info(device(cpu_selector{}), "cpu_selector");
    //output_dev_info(device(gpu_selector{}), "gpu_selector");
    output_dev_info(device(accelerator_selector{}), "accelerator_selector");
    //output_dev_info(device(intel::fpga_selector{}), "fpga_selector");
   
    return 0;
}



