#ifndef RULE_T_HPP
#define RULE_T_HPP

#include "expression.hpp"

#include <memory>
#include <vector>

template<class val_t>
class rule_t {
    unsigned m_id;
    std::unique_ptr<exp_t<val_t>> m_exp;
    val_t m_out;
public:
    using vals_t = std::vector<val_t>;

    rule_t(unsigned id, exp_t<val_t> *exp, val_t &&out)
        : m_id{id}, m_exp{exp}, m_out{std::forward<val_t>(out)} {}

    val_t out() const { return m_out; }
    bool is(const vals_t &kb) const { return m_exp->is(kb); }
    int unknown(const vals_t &kb) const {
        return m_exp->unknown(kb);
    }
    vals_t unknown_facts(const vals_t &kb) const {
        return m_exp->unknown_facts(kb);
    }
};

#endif // RULE_T_HPP
