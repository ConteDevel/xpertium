#ifndef TRACER_HPP
#define TRACER_HPP

#include "rule.hpp"

#include <stack>

template <typename val_t>
class tracer_t {
    using rulev_t = rule_t<val_t>;
    std::stack<rulev_t> trace;
public:
};

#endif // TRACER_HPP
