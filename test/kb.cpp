#include "kb.hpp"

kb_t::kb_t() {

}

kb_t::~kb_t() {
    for (auto it = m_rules.begin() ; it != m_rules.end(); ++it) {
         delete (*it);
    }
    m_rules.clear();
}

std::vector<quest_t<kb_t::val_t> > *kb_t::questions() {
    return &m_quests;
}

std::vector<rule_t<kb_t::val_t> *> *kb_t::rules() {
    return &m_rules;
}

void kb_t::create() {
    create_quest();
    create_rules();
}

void kb_t::create_quest() {
    m_quests.push_back(quest_t<val_t>("What is the color of the mineral?", {
        ans_t<val_t>("BlackColor", "Black"),
        ans_t<val_t>("BlueColor", "Blue"),
        ans_t<val_t>("BrownColor", "Brown"),
        ans_t<val_t>("GreenColor", "Green"),
        ans_t<val_t>("LightBlueColor", "Light blue"),
        ans_t<val_t>("OrangeColor", "Orange"),
        ans_t<val_t>("PinkColor", "Pink"),
        ans_t<val_t>("RedColor", "Red"),
        ans_t<val_t>("TransparentColor", "Transparent"),
        ans_t<val_t>("VioletColor", "Violet"),
        ans_t<val_t>("WhiteColor", "White"),
        ans_t<val_t>("YellowColor", "Yellow")
    }));
    m_quests.push_back(quest_t<val_t>("Has it a glitter?", {
        ans_t<val_t>("Glitter", "Yes"),
        ans_t<val_t>("_", "No")
    }));
}

void kb_t::create_rules() {
}
