#ifndef TERM_HPP
#define TERM_HPP

#include "trapeze.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace xpertium {

class phase_t: public trapeze_t {
    std::string m_name;
public:
    phase_t(const std::string &name, double x1 = 0, double x2 = 0,
            double x3 = 0, double x4 = 0) :
        trapeze_t(x1, x2, x3, x4), m_name{name} {}
    phase_t(const phase_t &) = default;
    phase_t(phase_t &&) = default;

    phase_t &operator=(const phase_t &) = default;
    phase_t &operator=(phase_t &) = default;
    phase_t &operator=(phase_t &&) = default;

    const std::string &name() { return m_name; }
};

/**
 * This class represents a separate term and helps to phase a value
 */
template <typename val_t>
class term_t {
    std::vector<phase_t> m_phases;
    val_t m_name;
public:
    term_t(val_t name) : m_name{name} {}
    term_t(const term_t<val_t> &) = default;
    term_t(term_t &&) = default;

    term_t<val_t> &operator=(const term_t<val_t> &) = default;
    term_t<val_t> &operator=(term_t<val_t> &) = default;
    term_t<val_t> &operator=(term_t &&) = default;

    phase_t *find_phase(const std::string &name) {
        auto it = std::find_if(m_phases.begin(), m_phases.end(),
                               [&name](auto obj) {
            return obj.name().compare(name) == 0;
        });
        if (it == m_phases.end()) { return nullptr; }
        return *it;
    }

    /**
     * @brief Returns the probability of belonging to the term [0, 1]
     * @param phase Phase name
     * @param value Input value
     * @return Probability of belonging
     */
    double hit(const std::string &phase, double value) const {
        auto ph = find_phase(phase);
        if (ph) { return ph->hit(value); }
        return -1;
    }

    /**
     * @brief Returns a term name
     */
    val_t name() const { return m_name; }
};

}

#endif // TERM_HPP
