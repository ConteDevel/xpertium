#ifndef KB_HPP
#define KB_HPP

#include "question.hpp"
#include "rule.hpp"

class kb_t {
    using val_t = std::string;
    std::vector<quest_t<val_t>> m_quests;
    std::vector<rule_t<val_t> *> m_rules;
public:
    kb_t();
    ~kb_t();
    std::vector<quest_t<val_t>> *questions();
    std::vector<rule_t<val_t> *> *rules();
    void create();
private:
    void create_quest();
    void create_rules();
};

#endif // KB_HPP
