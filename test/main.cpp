#include "kb_parser.hpp"
#include "expert.hpp"

#include <string>
#include <iostream>

using namespace std;
using namespace xpertium;

using sval_t = std::string;

int main() {
    std::setlocale(LC_ALL, "en_US.UTF-8");
    kb_t<sval_t> *kb;

    if (!xpertium::load_kb("kb.xml", &kb)) {
        std::cout << "Can't load knowledge database.\n";
    }

    std::cout << "KB name: " << kb->name() << std::endl;

    expert_t<std::string> exp(*kb);
    exp.reset();
    exp.run();

    delete kb;

    return 0;
}
