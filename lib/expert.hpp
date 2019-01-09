#ifndef EXPERT_SYSTEM_H
#define EXPERT_SYSTEM_H

#include "kb.hpp"

#include <algorithm>
#include <vector>

namespace xpertium {

template <typename val_t>
class expert_t {
    kb_t<val_t> &m_kb;
    vals_t<val_t> m_facts;
    std::vector<rule_t<val_t> *> m_unused_rules;
public:
    /**
     * @brief Constructor
     * @param kb Knowledge database
     */
    expert_t(kb_t<val_t> &kb) : m_kb{kb} {}

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
        auto rules = m_kb.rules();
        std::for_each(rules->begin(), rules->end(), [this](const auto &p) {
            m_unused_rules.push_back(p.get());
        });
        m_facts.clear();
    }

    void run() {
        while (!m_unused_rules.empty()) {
            auto it = m_unused_rules.begin();
            for (; it != m_unused_rules.end(); ++it) {
                if ((*it)->is(m_facts)) {
                    handle_rule(*it);
                    m_unused_rules.erase(it);
                }
            }
        }
    }

private:
    void handle_rule(const rule_t<val_t> *rule) {

    }
};

}

#endif // EXPERT_SYSTEM_H
