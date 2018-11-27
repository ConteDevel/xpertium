#include "kb.hpp"
#include "expert.hpp"

#include <string>
#include <iostream>

using namespace std;

using val_t = std::string;

int main() {
    kb_t kb;
    kb.create();
    expert_t<val_t> ex(kb.questions(), kb.rules());
    ex.reset();
    return 0;
}
