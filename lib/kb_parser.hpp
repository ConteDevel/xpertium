#ifndef KB_PARSER_HPP
#define KB_PARSER_HPP

#include "kb.hpp"
#include "tinyxml2.h"

#include <string>
#include <vector>

namespace xpertium {

using namespace tinyxml2;
using sval_t = std::string;
using sans_t = ans_t<sval_t>;
using squest_t = quest_t<sval_t>;
using srule_t = rule_t<sval_t>;

namespace internal {

std::vector<sans_t> parse_answers(XMLElement *element) {
    std::vector<sans_t> as;
    auto e = element->FirstChildElement("answer");

    for(; e != nullptr; e = e->NextSiblingElement("answer")) {
        sans_t a(e->Attribute("id"), e->Attribute("title"));
        as.push_back(a);
    }

    return as;
}

quests_t<sval_t> parse_questions(XMLElement *element) {
    quests_t<sval_t> qs;
    auto e = element->FirstChildElement("question");

    for(; e != nullptr; e = e->NextSiblingElement("question")) {
        auto answers = parse_answers(e->FirstChildElement("answers"));
        auto q = std::make_unique<squest_t>(e->Attribute("q"),
                                            std::move(answers));
        qs.push_back(std::move(q));
    }

    return qs;
}

exp_t<sval_t> *parse_exp(XMLElement *element) {
    std::string type = element->Attribute("type");

    if (type.compare("fact") == 0) {
        return new fact_t<sval_t>(element->Attribute("value"));
    }

    return new exp_t<sval_t>();
}

std::vector<srule_t *> parse_rules(XMLElement *element) {
    std::vector<srule_t *> rules;
    auto e = element->FirstChildElement("rule");

    for(; e != nullptr; e = e->NextSiblingElement("rule")) {
        auto exp_node = e->FirstChildElement("exp");
        auto exp = exp_node ? parse_exp(exp_node) : new exp_t<sval_t>();
        std::string id = e->Attribute("id");
        std::string out = e->Attribute("out");
        auto rule = new srule_t(std::move(id), std::move(exp), std::move(out));
        rules.push_back(rule);
    }

    return rules;
}

}

bool load_kb(const std::string &filename, kb_t<std::string> **kb) {
    XMLDocument doc;
    XMLError error = doc.LoadFile(filename.c_str());
    if (error != XML_SUCCESS) return false;
    auto root_node = doc.RootElement();
    *kb = new kb_t<std::string>(root_node->Attribute("name"));
    auto quests_node = root_node->FirstChildElement("questions");
    auto quests = internal::parse_questions(quests_node);
    auto rules_node = root_node->FirstChildElement("rules");
    auto rules = internal::parse_rules(rules_node);
    (*kb)->load(std::move(quests));

    return true;
}

}

#endif // KB_PARSER_HPP
