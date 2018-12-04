#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

template <typename val_t>
class ans_t {
public:
    const val_t id;
    const std::string title;

    ans_t(val_t &&id, std::string &&title): id{std::forward<val_t>(id)},
        title{std::forward<std::string>(title)} {}
};

template <typename val_t>
class quest_t {
public:
    const std::string question;
    const std::vector<ans_t<val_t>> answers;

    quest_t(std::string quest, std::initializer_list<ans_t<val_t>> answer_list)
        : question{std::forward<std::string>(quest)}, answers{answer_list} {}

    quest_t(std::string quest, std::vector<ans_t<val_t>> answers)
        : question{std::forward<std::string>(quest)}, answers{answers} {}

    bool suitable(const std::vector<val_t> &facts) const {
        for (auto ans : answers) {
            if (std::find(facts.begin(), facts.end(), ans.id) != facts.end()) {
                return true;
            }
        }
        return false;
    }
};

#endif // QUESTION_HPP
