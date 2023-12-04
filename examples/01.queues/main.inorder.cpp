#include <iostream>
#include <CL/sycl.hpp>

int main(int argc, char *argv[]) {
    constexpr const size_t N = 1024*64;
    const sycl::range VecSize{N};

    sycl::buffer<double> bufA{VecSize};
    sycl::buffer<double> bufB{VecSize};
    sycl::buffer<double> bufC{VecSize};

    // Initialize input data
    {
        sycl::host_accessor h_a{bufA, sycl::write_only};
        sycl::host_accessor h_b{bufB, sycl::write_only};

        for (int i = 0; i < N; i++) {
            h_a[i] = sin(i);
            h_b[i] = cos(i);
        }
    }

    sycl::device d = sycl::device(sycl::default_selector_v);

    std::cout << "Device: " << std::endl;
    std::cout << "\tName: " <<d.get_info<sycl::info::device::name>() << std::endl;
    std::cout << "\tMax Compute Units: " <<d.get_info<sycl::info::device::max_compute_units>() << std::endl;

    sycl::queue q{d, sycl::property::queue::in_order()}; // In-order Queue

    auto event1 = q.submit([&](sycl::handler &h) {
        auto a = bufA.get_access<sycl::access::mode::read_write>(h);
        h.parallel_for(VecSize, [=](sycl::id<1> i) {
            a[i] = a[i] * a[i];
        });
    });

    auto event2 = q.submit([&](sycl::handler &h) {
        auto b = bufB.get_access<sycl::access::mode::read_write>(h);
        h.parallel_for(VecSize, [=](sycl::id<1> i) {
            b[i] = b[i] * b[i];
        });
    });

    auto event3 = q.submit([&](sycl::handler &h) {
        const auto read_t = sycl::access::mode::read;
        const auto write_t = sycl::access::mode::write;

        auto a = bufA.get_access<read_t>(h);
        auto b = bufB.get_access<read_t>(h);
        auto c = bufC.get_access<write_t>(h);

        h.parallel_for(VecSize, [=](sycl::id<1> i) {
            c[i] = a[i] + b[i];
        });
    });

    {
        sycl::host_accessor h_c{bufC, sycl::read_only};

        double sum = 0.0f;
        for (int i = 0; i < N; i++) {
            sum += h_c[i];
        }
        std::cout << "Sum is : " << sum << std::endl;
    }

    return 0;
}