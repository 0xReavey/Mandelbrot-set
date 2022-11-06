#include "mandelbrot.hpp"
using ld = double long;
int inSet(std::complex<long double> c, int iterations) {
    std::complex<long double> z{0.0, 0.};
    for (int i = 0; i < iterations; i++) {
        z = z * z + c;
        if (std::norm(z) > 50) return i;
    }
    return 0;
}
