#include <iostream>
#include <CL/sycl.hpp>

int main(int argc, char *argv[]) try {
    constexpr const size_t N = 1024 * 1024 * 256; //2GB
    const sycl::range VecSize{N};

    sycl::buffer<double> bufA{VecSize};
    sycl::buffer<double> bufB{VecSize};
    sycl::buffer<double> bufC{VecSize};

    // Initialize input data
    {
        sycl::host_accessor h_a{bufA, sycl::write_only};
        sycl::host_accessor h_b{bufB, sycl::write_only};

        for (int i = 0; i < N; i++) {
            h_a[i] = sin(i) * sin(i);
            h_b[i] = cos(i) * cos(i);
        }
    }

    sycl::device d = sycl::device(sycl::default_selector_v);

    std::cout << "Device: " << std::endl;
    std::cout << "\tName: " << d.get_info<sycl::info::device::name>() << std::endl;
    std::cout << "\tMax Compute Units: " << d.get_info<sycl::info::device::max_compute_units>() << std::endl;
    sycl::queue myQueue{
            d,
            [](sycl::exception_list elist) {
                for (auto &e: elist) {
                    try { std::rethrow_exception(e); }
                    catch (sycl::exception &e) {
                        std::cerr << "Async Handler: " << e.what() << std::endl;
                    }
                }
                std::terminate();
            },
            sycl::property::queue::in_order()
    };

    auto cg = [&](sycl::handler &h) {
        const auto read_t = sycl::access::mode::read;
        const auto write_t = sycl::access::mode::write;

        auto a = bufA.get_access<read_t>(h);
        auto b = bufB.get_access<read_t>(h);
        auto c = bufC.get_access<write_t>(h);

        h.parallel_for(VecSize, [=](sycl::id<1> i) {
            c[i] = a[i] + b[i];
        });
    };

    myQueue.submit(cg);

    {
        sycl::host_accessor h_c{bufC, sycl::read_only};

        double sum = 0.0f;
        for (int i = 0; i < N; i++) {
            sum += h_c[i];
        }
        std::cout << "Sum is : " << sum << std::endl;
    }

    return 0;
} catch (sycl::exception &e) {
    std::cerr << "SYCL Exception: " << e.what() << std::endl;
    std::terminate();
} catch (std::exception &e) {
    std::cerr << "STD Exception: " << e.what() << std::endl;
    std::terminate();
} catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
    std::terminate();
}