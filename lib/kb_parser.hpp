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

std::vector<squest_t> parse_questions(XMLElement *element) {
    std::vector<squest_t> qs;
    auto e = element->FirstChildElement("question");

    for(; e != nullptr; e = e->NextSiblingElement("question")) {
        auto answers = parse_answers(e->FirstChildElement("answers"));
        squest_t q(e->Attribute("q"), answers);
        qs.push_back(q);
    }

    return qs;
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
    (*kb)->load(quests);

    return true;
}

}

#endif // KB_PARSER_HPP
