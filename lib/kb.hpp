#ifndef KB_HPP
#define KB_HPP

#include "question.hpp"
#include "rule.hpp"

#include <string>
#include <utility>

namespace xpertium {

template <typename val_t>
using quests_t = std::vector<std::unique_ptr<quest_t<val_t>>>;

template <typename val_t>
using rules_t = std::vector<std::unique_ptr<rule_t<val_t>>>;

template <typename val_t>
class kb_t {
    std::string m_name;
    quests_t<val_t> m_quests;
    rules_t<val_t> m_rules;
public:
    /**
     * @brief Constructor
     * @param name Name of the knowledge database
     */
    kb_t(const std::string &name) : m_name{name} {}
    const std::string &name() { return m_name; }
    quests_t<val_t> *questions() { return m_quests; }
    rules_t<val_t> *rules() { return m_rules; }

    void load(quests_t<val_t> &&quests) {
        m_quests = std::forward<quests_t<val_t>>(quests);
    }
};

}

#endif // KB_HPP
