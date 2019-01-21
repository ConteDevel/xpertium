#include "dialog.hpp"
#include "kb_parser.hpp"
#include "expert.hpp"
#include "tracer.hpp"

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
        return 1;
    }

    std::cout << "KB name: " << kb->name() << std::endl;

    std::unique_ptr<dialog_t<sval_t>> dialog(new dialog_t<sval_t>());
    std::unique_ptr<tracer_t<sval_t>> tracer(new tracer_t<sval_t>());
    expert_t<std::string> exp(kb, dialog.get(), tracer.get());
    exp.reset();
    exp.run();
    char ch;
    std::cout << "Show trace (Y/N): ";
    std::cin >> ch;

    if (ch == 'Y' || ch == 'y') {
        tracer->print();
    }

    delete kb;

    return 0;
}
