#include "kb_parser.hpp"
#include "expert.hpp"

#include <string>
#include <iostream>

using namespace std;

using val_t = std::string;

int main() {
    std::setlocale(LC_ALL, "en_US.UTF-8");
    kb_t<std::string> *kb;

    if (!xpertium::load_kb("kb.xml", &kb)) {
        std::cout << "Can't load knowledge database.\n";
    }

    std::cout << "KB name: " << kb->name() << std::endl;

    delete kb;

    return 0;
}
