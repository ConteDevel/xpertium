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
    std::unique_ptr<exp_t<val_t>> m_exp;
    std::unique_ptr<quest_t<val_t>> m_quest;
    std::unique_ptr<val_t> m_out;
public:
    /**
     * @brief Constructor
     * @param id Rule ID
     * @param exp Pointer to an activating logical expression
     * @param quest Question pointer
     * @param out Rule output
     */
    rule_t(const std::string &id, exp_t<val_t> *exp, quest_t<val_t> *quest,
           val_t *out) : m_id{id}, m_exp{exp}, m_quest{quest}, m_out{out} {}

    /**
     * @brief Constructor
     * @param id Rule ID
     * @param exp Pointer to an activating logical expression
     * @param q_id Question pointer
     */
    rule_t(const std::string &id, exp_t<val_t> *exp, quest_t<val_t> *quest) :
        rule_t{id, exp, quest, nullptr} {}

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
    bool is(const vals_t<val_t> &kb) const { return m_exp->is(kb); }

    /**
     * @brief Returns a rule ID
     */
    const std::string &id() { return m_id; }

    /**
     * @brief Returns the linked question
     */
    const quest_t<val_t> *quest() { return m_quest.get(); }

    /**
     * @brief Returns a rule output (can be `nullptr`)
     */
    const val_t *out() { return m_out.get(); }
};

}

#endif // RULE_T_HPP
