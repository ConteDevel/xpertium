#ifndef RULE_T_HPP
#define RULE_T_HPP

#include "expression.hpp"
#include "question.hpp"

#include <memory>
#include <string>
#include <vector>

namespace xpertium {

/**
 * This class represents a production-rule in the system
 */
template<class val_t>
class rule_t {
    std::string m_id;
    exp_t<val_t> m_exp;
    std::string m_quest_id;
public:
    /**
     * @brief Constructor
     * @param id Rule ID
     * @param exp Activating logical expression
     * @param quest_id Question ID
     */
    rule_t(std::string &&id, exp_t<val_t> &&exp, std::string &&quest_id) :
        m_id{std::forward<std::string>(id)},
        m_exp{std::forward<exp_t<val_t>>(exp)},
        m_quest_id{std::forward<std::string>(quest_id)} {}

    /**
     * @brief Deletes a copy constructor
     */
    rule_t(const rule_t<val_t> &&) = delete;

    /**
     * @brief Move constructor
     */
    rule_t(rule_t &&) = default;

    /**
     * @brief Deletes a copy assignment
     */
    rule_t<val_t> &operator=(const rule_t<val_t> &) = delete;

    /**
     * @brief Deletes an assignment
     */
    rule_t<val_t> &operator=(rule_t<val_t> &) = delete;

    /**
     * @brief Move assignment
     */
    rule_t<val_t> &operator=(rule_t<val_t> &&) = default;

    /**
     * @brief Checks if an activating logical expression is true
     * @param kb Knowledge database
     * @return Check result
     */
    bool is(const vals_t<val_t> &kb) const { return m_exp.is(kb); }
};

}

#endif // RULE_T_HPP
