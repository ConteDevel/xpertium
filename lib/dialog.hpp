#ifndef DIALOG_HPP
#define DIALOG_HPP

#include "question.hpp"

#include <iostream>

namespace xpertium {

template <typename val_t>
class base_dialog_t {
public:
    base_dialog_t() {}
    virtual ~base_dialog_t() {}

    virtual val_t ask(const quest_t<val_t> *quest) = 0;
    virtual std::ostream &print() = 0;
};

template <typename val_t>
class dialog_t : public base_dialog_t<val_t> {
public:
    /**
     * @brief Constructor
     */
    dialog_t() : base_dialog_t<val_t>() {}

    /**
     * @brief Copy constructor
     */
    dialog_t(const dialog_t<val_t> &) = default;

    /**
     * @brief Move constructor
     */
    dialog_t(dialog_t &&) = default;

    /**
     * @brief Copy assignment
     */
    dialog_t<val_t> &operator=(const dialog_t<val_t> &) = default;

    /**
     * @brief Assignment
     */
    dialog_t<val_t> &operator=(dialog_t<val_t> &) = default;

    /**
     * @brief Move assignment
     */
    dialog_t<val_t> &operator=(dialog_t &&) = default;

    virtual val_t ask(const quest_t<val_t> *quest) override {
        std::cout << "Q: " << quest->question() << std::endl;
        auto answers = quest->answers();
        for (size_t i = 0; i < answers.size(); ++i) {
            std::cout << (i + 1) << ") " << answers[i].title() << std::endl;
        }

        size_t ans_idx;
        do {
            std::cout << "Enter the answer number (`_` to abort): ";
            std::cin >> ans_idx;
            --ans_idx;
            if (ans_idx >= answers.size()) {
                std::cout << "Invalid number\n";
            }
        } while (ans_idx >= answers.size());

        return answers[ans_idx].id();
    }

    virtual std::ostream &print() override {
        return std::cout;
    }
};

}

#endif // DIALOG_HPP
