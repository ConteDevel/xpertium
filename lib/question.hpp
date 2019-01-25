#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace xpertium {

/**
 * This class is used to define answer for the question
 */
template <typename val_t>
class ans_t {
    val_t m_id;
    std::string m_title;
public:
    /**
     * @brief Constructor
     * @param id Answer ID (it must define a new fact in the system)
     * @param title Human readable title
     */
    ans_t(val_t id, const std::string &title): m_id{id},
        m_title{title} {}
    /**
     * @brief Copy constructor
     */
    ans_t(const ans_t<val_t> &) = default;

    /**
     * @brief Move constructor
     */
    ans_t(ans_t &&) = default;

    /**
     * @brief Copy assignment
     */
    ans_t<val_t> &operator=(const ans_t<val_t> &) = default;

    /**
     * @brief Assignment
     */
    ans_t<val_t> &operator=(ans_t<val_t> &) = default;

    /**
     * @brief Move assignment
     */
    ans_t<val_t> &operator=(ans_t &&) = default;

    /**
     * @brief Returns a title of the answer
     */
    const std::string &title() const { return m_title; }

    /**
     * @brief Returns an ID of the answer
     */
    val_t id() const { return m_id; }
};

template <typename val_t> using answers_t = std::vector<ans_t<val_t>>;

/**
 * This class represents a question in the knowledge database
 */
template <typename val_t>
class quest_t {
    std::string m_id;
    std::string m_question;
    answers_t<val_t> m_answers;
public:
    /**
     * @brief Constructor
     * @param id Question ID
     * @param quest Question
     * @param answers List of answers
     */
    quest_t(const std::string &id, const std::string &quest,
            answers_t<val_t> &&answers)
        : m_id{id}, m_question{quest}, m_answers{std::move(answers)} {}

    /**
     * @brief Copy constructor
     */
    quest_t(const quest_t<val_t> &) = default;

    /**
     * @brief Move constructor
     */
    quest_t(quest_t &&) = default;

    /**
     * @brief Copy assignment
     */
    quest_t<val_t> &operator=(const quest_t<val_t> &) = default;

    /**
     * @brief Assignment
     */
    quest_t<val_t> &operator=(quest_t<val_t> &) = default;

    /**
     * @brief Move assignment
     */
    quest_t<val_t> &operator=(quest_t &&) = default;

    /**
     * @brief Returns the question ID
     */
    const std::string &id() const { return m_id; }

    /**
     * @brief Returns a human readable question
     */
    const std::string &question() const { return m_question; }

    /**
     * @brief Returns a list of answers
     */
    const answers_t<val_t> &answers() const { return m_answers; }
};

}

#endif // QUESTION_HPP
