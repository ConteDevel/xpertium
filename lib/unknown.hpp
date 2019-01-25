#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <vector>

namespace xpertium {

template <typename val_t>
struct unknown_t {
    bool state;
    val_t value;

    unknown_t(bool state, val_t value) : state{state}, value{value} {}
};

template <typename val_t> using unknowns_t = std::vector<unknown_t<val_t>>;

}

#endif // UTILS_HPP
