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

quests_t<sval_t> *parse_questions(XMLElement *element) {
    auto qs = new quests_t<sval_t>();

    if (!element) { return qs; }

    auto e = element->FirstChildElement("question");

    for(; e != nullptr; e = e->NextSiblingElement("question")) {
        auto answers = parse_answers(e->FirstChildElement("answers"));
        auto q = std::make_unique<squest_t>(
                    e->Attribute("id"),
                    e->Attribute("q"),
                    std::move(answers));
        qs->push_back(std::move(q));
    }

    return qs;
}

exp_t<sval_t> *parse_exp(XMLElement *element) {
    std::string type = element->Attribute("type");

    if (type.compare("fact") == 0) {
        return _fact<sval_t>(element->Attribute("value"));
    }
    if (type.compare("not") == 0) {
        auto nested_exp = parse_exp(element->FirstChildElement("exp"));
        return _not<sval_t>(nested_exp);
    }

    auto exps = exps_t<sval_t>();
    auto e = element->FirstChildElement("exp");
    for(; e != nullptr; e = e->NextSiblingElement("exp")) {
        auto nested_exp = std::unique_ptr<exp_t<sval_t>>(parse_exp(e));
        exps.push_back(std::move(nested_exp));
    }

    return type.compare("and") == 0 ?
                _and<sval_t>(std::move(exps)) : _or<sval_t>(std::move(exps));
}

rules_t<sval_t> *parse_rules(XMLElement *element) {
    auto rules = new rules_t<sval_t>();
    auto e = element->FirstChildElement("rule");

    for(; e != nullptr; e = e->NextSiblingElement("rule")) {
        auto exp_node = e->FirstChildElement("exp");
        auto exp = exp_node ? parse_exp(exp_node) : new exp_t<sval_t>();
        std::string id = e->Attribute("id");
        auto q_id_ptr = e->Attribute("quest_id");
        std::string *q_id = q_id_ptr ? new std::string(q_id_ptr) : nullptr;
        auto out_ptr = e->Attribute("out");
        std::string *out = out_ptr ? new std::string(out_ptr) : nullptr;
        auto rule = std::make_unique<srule_t>(std::move(id), exp, q_id, out);
        rules->push_back(std::move(rule));
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
    (*kb)->load(quests, rules);

    return true;
}

}

#endif // KB_PARSER_HPP
