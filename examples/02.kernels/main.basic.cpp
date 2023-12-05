#include <iostream>
#include <CL/sycl.hpp>

int main(int argc, char *argv[]) {
    constexpr unsigned N = 32;
    const sycl::range MatSize{N, N};

    sycl::buffer<double, 2> bufA{MatSize};
    sycl::buffer<double, 2> bufB{MatSize};
    sycl::buffer<double, 2> bufGold{MatSize};
    sycl::buffer<double, 2> bufC{MatSize};

    // Initialize input data
    {
        sycl::host_accessor h_a{bufA, sycl::read_write};
        sycl::host_accessor h_b{bufB, sycl::read_write};
        sycl::host_accessor h_g{bufGold, sycl::write_only};
        sycl::host_accessor h_c{bufC, sycl::write_only};

        for (unsigned j = 0; j < N; j++) {
            for (unsigned i = 0; i < N; i++) {
                h_a[j][i] = i;
                h_b[j][i] = j;
            }
        }

        for (unsigned j = 0; j < N; j++) {
            for (unsigned i = 0; i < N; i++) {
                double sum = 0;
                for (unsigned k = 0; k < N; k++) {
                    sum += h_a[j][k] * h_b[k][i];
                }
                h_g[j][i] = sum;
                h_c[j][i] = 0;
            }
        }
    }

    sycl::device d = sycl::device(sycl::default_selector_v);

    std::cout << "Device: " << std::endl;
    std::cout << "\tName: " << d.get_info<sycl::info::device::name>() << std::endl;
    std::cout << "\tMax Compute Units: " << d.get_info<sycl::info::device::max_compute_units>() << std::endl;

    sycl::queue q{d, sycl::property::queue::in_order()}; // In-order Queue

    auto event1 = q.submit([&](sycl::handler &h) {
        auto a = bufA.get_access<sycl::access::mode::read>(h);
        auto b = bufB.get_access<sycl::access::mode::read>(h);
        auto c = bufC.get_access<sycl::access::mode::write>(h);
        h.parallel_for(MatSize, [=](sycl::id<2> idx) {
            double sum = 0;
            for (unsigned k = 0; k < N; k++) {
                sum += a[idx.get(0)][k] * b[k][idx.get(1)];
            }
            c[idx] = sum;
        });
    });

    {
        sycl::host_accessor h_g{bufGold, sycl::read_only};
        sycl::host_accessor h_c{bufC, sycl::read_only};
        constexpr double maxErr = 1e-4;
        size_t errCnt = 0;
        for (unsigned j = 0; j < N; j++) {
            for (unsigned i = 0; i < N; i++) {
                auto dif = h_g[j][i] - h_c[j][i];
                dif = dif < 0 ? dif * -1.0 : dif;
                if (dif > maxErr) {
                    if(errCnt<25){
                        std::cout << "Mismatch at [j,i] of [" << j << ", " << i << "]: Gold: " << h_g[j][i] << ", C: "
                                  << h_c[j][i] << std::endl;
                    }
                    errCnt++;
                }
            }
        }

        std::cout << "Total Mismatches: "<< errCnt << std::endl;
    }

    return 0;
}