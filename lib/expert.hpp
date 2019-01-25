#ifndef EXPERT_SYSTEM_H
#define EXPERT_SYSTEM_H

#include "dialog.hpp"
#include "kb.hpp"
#include "tracer.hpp"

#include <algorithm>
#include <iterator> // for back_inserter
#include <vector>

namespace xpertium {

template <typename val_t>
class expert_t {
    const kb_t<val_t> *m_kb;
    const base_dialog_t<val_t> &m_dialog;
    base_tracer_t<val_t> &m_tracer;
    vals_t<val_t> m_facts;
    std::vector<rule_t<val_t> *> m_cur_rules;
public:
    /**
     * @brief Constructor
     * @param kb Knowledge database
     * @param dialog Dialogue
     * @param tracer Stack tracer
     */
    expert_t(const kb_t<val_t> *kb, const base_dialog_t<val_t> &dialog,
             base_tracer_t<val_t> &tracer) :
        m_kb{kb}, m_dialog{dialog}, m_tracer{tracer} {}

    /**
     * @brief Copy constructor
     */
    expert_t(const expert_t<val_t> &) = default;

    /**
     * @brief Move constructor
     */
    expert_t(expert_t &&) = default;

    /**
     * @brief Move assignment
     */
    expert_t<val_t> &operator=(expert_t &&) = default;

    /**
     * @brief Resets all known facts
     * @param init Initial facts
     */
    void reset(const std::vector<val_t> *init = nullptr) {
        m_cur_rules.clear();
        auto rules = m_kb->rules();
        std::for_each(rules->begin(), rules->end(), [this] (const auto &p) {
            m_cur_rules.push_back(p.get());
        });
        m_facts.clear();
        m_tracer.clear();
        if (init) {
            for (auto it = init->begin(); it != init->end(); ++it) {
                m_facts.push_back(*it);
                m_tracer.push_fact(*it);
            }
        }
    }

    /**
     * @brief Launch the expert system with the reverse output
     * @param target_fact The target fact (`nullptr` to run for any target)
     * @return True if the target was achieved
     */
    bool reverse(const val_t target_fact) {
        bool result = reverse_impl(target_fact);
        if (result) {
            m_dialog.print() << "Target is reachable!" << std::endl;
        } else {
            m_dialog.print() << "Target isn't reachable!" << std::endl;
        }

        return result;
    }

    /**
     * @brief Launch the expert system with the direct output
     * @param target_fact The target fact (`nullptr` to run for any target)
     * @return True if the target was achieved
     */
    bool direct(const val_t *target_fact = nullptr) {
        while (!m_cur_rules.empty()) {
            auto it = m_cur_rules.begin();
            auto old_size = m_cur_rules.size();
            bool is_target = false;
            for (; it < m_cur_rules.end(); ++it) {
                is_target = false;
                if ((*it)->is(m_facts) > 0) {
                    if (!handle_rule(*it)) { return false; }
                    is_target = (*it)->target();

                    m_cur_rules.erase(it);
                }
                if (is_target) { break; }
            }
            if (is_target && !target_fact) {
                m_dialog.print() << "Result: " << m_facts.back()
                                 << std::endl;
                return true;
            } else if (*target_fact == m_facts.back()) {
                m_dialog.print() << "Target was found!" << std::endl;
                return true;
            }
            if (old_size == m_cur_rules.size()) {
                if (target_fact) {
                    m_dialog.print() << "Target wasn't reached" << std::endl;
                } else {
                    m_dialog.print() << "No reachable targets" << std::endl;
                }
                return false;
            }
        }

        return true;
    }

private:
    /**
     * @brief Handles the rule to get its output and update fact database
     * @param rule Rule
     * @return False if the rule is invalid
     */
    bool handle_rule(const rule_t<val_t> *rule) {
        val_t fact;

        if (rule->question()) {
            fact = m_dialog.ask(rule->question());
        } else if (rule->out()) {
            fact = *rule->out();
        } else {
            m_dialog.print() << "Rule `" << rule->id()
                              << "` doesn't consist question or output"
                              << std::endl;

            return false;
        }

        m_tracer.push_rule(rule, fact);
        m_tracer.push_fact(fact);

        m_facts.push_back(fact);

        return true;
    }

    bool reverse_impl(const val_t tgt_fact) {
        for (auto i = m_cur_rules.begin(); i < m_cur_rules.end(); ++i) {
            auto rule = *i;
            if ((*i)->is_possible_out(tgt_fact) && check_output(*i, tgt_fact)) {
                m_cur_rules.erase(i);
                return prove_rule(rule, tgt_fact);
            }
        }

        return false;
    }

    /**
     * @brief Check if activation conditions of the rule are true
     * @param rule Rule
     * @param target_fact Target fact (it will be accepted as true fact if the
     *                    conditions are true)
     * @return Check result
     */
    bool check_rule(const rule_t<val_t> *rule, val_t target_fact) {
        if (rule->is(m_facts) > 0) {
            m_facts.push_back(target_fact);
            m_tracer.push_fact(target_fact);

            return true;
        }

        return false;
    }

    /**
     * @brief Try to prove that the target is a fact
     * @param rule Rule that contains a required output
     * @param target_fact Target (potential fact)
     * @return True if the target was proved
     */
    bool prove_rule(const rule_t<val_t> *rule, val_t target_fact) {
        bool approved;
        do {
            approved = false;
            auto uks = rule->unknowns(m_facts);

            if (uks.empty()) { return check_rule(rule, target_fact); }

            for (auto u = uks.begin(); u != uks.end(); ++u) {
                if (reverse_impl(*u)) {
                    approved = true;

                    break;
                }
            }
        } while (approved);

        return false;
    }

    /**
     * @brief Check if the rule will produce a required output fact
     * @param rule Rule
     * @param target_fact Target fact
     * @return Check result
     */
    bool check_output(const rule_t<val_t> *rule, val_t target_fact) {
        val_t fact;

        if (rule->question()) {
            fact = m_dialog.ask(rule->question());
        } else if (rule->out()) {
            fact = *rule->out();
        } else {
            m_dialog.print() << "Rule `" << rule->id()
                             << "` doesn't consist question or output"
                             << std::endl;

            return false;
        }

        m_tracer.push_rule(rule, fact);

        return fact == target_fact;
    }
};

}

#endif // EXPERT_SYSTEM_H
