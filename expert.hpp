#ifndef EXPERT_SYSTEM_H
#define EXPERT_SYSTEM_H

#include "question.hpp"
#include "rule.hpp"

#include <limits>
#include <vector>

template <typename val_t>
class expert_t {
    using questv_t = quest_t<val_t>;
    using rulev_t = rule_t<val_t>;

    std::vector<questv_t> m_quests;
    std::vector<rulev_t *> m_rules;

    std::vector<val_t> m_facts;
    std::vector<questv_t *> m_act_quests;
    std::vector<rulev_t *> m_act_rules;
public:
    expert_t(std::vector<questv_t> &&quests, std::vector<rulev_t *> &&rules)
        : m_quests{std::forward<std::vector<questv_t>>(quests)},
          m_rules{std::forward<std::vector<rulev_t *>>(rules)} {}

    virtual ~expert_t() {
        for (auto it = m_rules.begin() ; it != m_rules.end(); ++it) {
             delete (*it);
        }
        m_rules.clear();
    }

    void reset() {
        m_facts.clear();
        m_act_quests.clear();
        m_act_rules.clear();
        // Copy questions
        for (auto i = m_quests.begin(); i != m_quests.end(); ++i) {
            m_act_quests.push_back(&*i);
        }
        // Copy rules
        for (auto i = m_rules.begin(); i != m_rules.end(); ++i) {
            // Copy all but trivial rules
            if ((*i)->is(m_facts)) m_facts.push_back((*i)->out());
            else m_act_rules.push_back(*i);
        }
    }

    bool next(const quest_t<val_t> **q) {
        auto i = m_act_rules.begin();
        int min = std::numeric_limits<int>::max();
        rulev_t *candidate = nullptr;
        // Search a rule-candidate to prepare the next question
        while (i != m_act_rules.end()) {
            if ((*i)->is(m_facts)) i = m_act_rules.erase(i);
            else {
                int unknown = (*i)->unknown(m_facts);
                if (unknown < min) {
                    min = unknown;
                    candidate = *i;
                }
                ++i;
            }
        }
        // Find a question to get a new actual fact
        *q = find_quest(std::move(candidate->unknown_facts(m_facts)));

        return *q == nullptr;
    }
private:
    questv_t *find_quest(const std::vector<val_t> && facts) {
        auto quest = m_act_quests.begin();
        for (; quest != m_act_quests.end(); ++quest) {
            if (quest->suitable(facts)) break;
        }
        if (quest == m_act_quests.end()) return nullptr;

        return *quest;
    }
};

#endif // EXPERT_SYSTEM_H
