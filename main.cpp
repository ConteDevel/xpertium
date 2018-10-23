#include "expert.hpp"

#include <string>
#include <iostream>

using namespace std;

using val_t = std::string;

int main() {
    std::vector<quest_t<val_t>> quests;
    std::vector<rule_t<val_t> *> rules;
    expert_t<val_t> ex(std::move(quests), std::move(rules));
    ex.reset();
    return 0;
}
