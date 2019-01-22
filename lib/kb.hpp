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
    std::unique_ptr<quests_t<val_t>> m_quests;
    std::unique_ptr<rules_t<val_t>> m_rules;
public:
    /**
     * @brief Constructor
     * @param name Name of the knowledge database
     */
    kb_t(const std::string &name) : m_name{name} {}

    /**
     * @brief Deletes a copy constructor
     */
    kb_t(const kb_t<val_t> &) = delete;

    /**
     * @brief Move constructor
     */
    kb_t(kb_t &&) = default;

    /**
     * @brief Deletes a copy assignment
     */
    kb_t<val_t> &operator=(const kb_t<val_t> &) = delete;

    /**
     * @brief Deletes an assignment
     */
    kb_t<val_t> &operator=(kb_t<val_t> &) = delete;

    /**
     * @brief Move assignment
     */
    kb_t<val_t> &operator=(kb_t &&) = default;

    /**
     * @brief Returns a name of the knowledge database
     */
    const std::string &name() const { return m_name; }

    /**
     * @brief Returns a question by ID
     * @param id Question ID
     * @return Pointer of question or `nullptr`
     */
    const quest_t<val_t> *question(const std::string *id) const {
        for (auto it = m_quests->begin(); it != m_quests->end(); ++it) {
            if (it->id == (*id)) { return *it; }
        }
        return nullptr;
    }

    /**
     * @brief Returns questions
     */
    const quests_t<val_t> *questions() const { return m_quests.get(); }

    /**
     * @brief Returns rules
     */
    const rules_t<val_t> *rules() const { return m_rules.get(); }

    /**
     * @brief Loads a production model
     * @param quests Questions
     * @param rules Production rules
     */
    void load(quests_t<val_t> *quests, rules_t<val_t> *rules) {
        m_quests = std::unique_ptr<quests_t<val_t>>(quests);
        m_rules = std::unique_ptr<rules_t<val_t>>(rules);
    }
};

}

#endif // KB_HPP
