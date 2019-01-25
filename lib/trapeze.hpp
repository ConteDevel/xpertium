#ifndef TRAPEZE_HPP
#define TRAPEZE_HPP

namespace xpertium {

class trapeze_t {
public:
    double x1, x2, x3, x4;

    trapeze_t(double x1, double x2, double x3, double x4) :
        x1{x1}, x2{x2}, x3{x3}, x4{x4} {}
    trapeze_t(const trapeze_t &) = default;
    trapeze_t(trapeze_t &&) = default;

    trapeze_t &operator=(const trapeze_t &) = default;
    trapeze_t &operator=(trapeze_t &) = default;
    trapeze_t &operator=(trapeze_t &&) = default;

    double hit(double x) const {
        if (x >= x1 && x < x2) { return (x - x1) / (x2 - x1); }
        if (x >= x2 && x < x3) { return (x4 - x) / (x4 - x3); }
        if (x >= x3 && x < x4) { return 0.0; }

        return 0.0;
    }
};

}

#endif // TRAPEZE_HPP
