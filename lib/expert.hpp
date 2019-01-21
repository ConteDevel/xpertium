#ifndef EXPERT_SYSTEM_H
#define EXPERT_SYSTEM_H

#include "dialog.hpp"
#include "kb.hpp"

#include <algorithm>
#include <vector>

namespace xpertium {

template <typename val_t>
class expert_t {
    kb_t<val_t> *m_kb;
    base_dialog_t<val_t> *m_dialog;
    vals_t<val_t> m_facts;
    std::vector<rule_t<val_t> *> m_unused_rules;
public:
    /**
     * @brief Constructor
     * @param kb Knowledge database
     * @param dialog Dialogue
     */
    expert_t(kb_t<val_t> *kb, base_dialog_t<val_t> *dialog) :
        m_kb{kb}, m_dialog{dialog} {}

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
     */
    void reset() {
        m_unused_rules.clear();
        auto rules = m_kb->rules();
        std::for_each(rules->begin(), rules->end(), [this](const auto &p) {
            m_unused_rules.push_back(p.get());
        });
        m_facts.clear();
    }

    bool run() {
        while (!m_unused_rules.empty()) {
            auto it = m_unused_rules.begin();
            auto old_size = m_unused_rules.size();
            for (; it != m_unused_rules.end(); ++it) {
                if ((*it)->is(m_facts)) {

                    if (!handle_rule(*it)) { return false; }

                    m_unused_rules.erase(it);
                }
            }
            if (old_size == m_unused_rules.size()) {
                m_dialog->print() << "Result : " << m_facts.back() << std::endl;

                break;
            }
        }

        return true;
    }

private:
    bool handle_rule(const rule_t<val_t> *rule) {
        val_t fact;

        if (rule->question()) {
            fact = m_dialog->ask(rule->question());
        } else if (rule->out()) {
            fact = *rule->out();
        } else {
            m_dialog->print() << "Rule `" << rule->id()
                              << "` doesn't consist question or output"
                              << std::endl;

            return false;
        }

        m_facts.push_back(fact);

        return true;
    }
};

}

#endif // EXPERT_SYSTEM_H
