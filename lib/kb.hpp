#ifndef KB_HPP
#define KB_HPP

#include "question.hpp"
#include "rule.hpp"

#include <string>
#include <utility>

template <typename val_t>
class kb_t {
    using quests_t = std::vector<quest_t<val_t>>;
    using rules_t = std::vector<rule_t<val_t> *>;

    const std::string m_name;
    quests_t m_quests;
    rules_t m_rules;
public:
    kb_t(std::string &&name) : m_name{std::forward<std::string>(name)} {}
    virtual ~kb_t() {
        for (auto it = m_rules.begin() ; it != m_rules.end(); ++it) {
             delete (*it);
        }
        m_rules.clear();
    }
    const std::string &name() { return m_name; }
    quests_t *questions() { return m_quests; }
    rules_t *rules() { return m_rules; }

    void load(quests_t quests) {
        m_quests = std::forward<quests_t>(quests);
    }
};

#endif // KB_HPP
