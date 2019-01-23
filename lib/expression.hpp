#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "utils.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace xpertium {

template <typename val_t> using vals_t = std::vector<val_t>;

/**
 * This class must be a parent of all expression classes
 */
template <typename val_t>
class exp_t {
public:
    /**
     * @brief Constructor
     */
    exp_t() {}

    /**
     * @brief Deletes copy constructor
     */
    exp_t(const exp_t<val_t> &) = delete;

    /**
     * @brief Move constructor
     */
    exp_t(exp_t &&) = default;

    /**
     * @brief Virtual destructor
     */
    virtual ~exp_t() {}

    /**
     * @brief Deletes a copy assignment
     */
    exp_t<val_t> &operator=(const exp_t<val_t> &) = delete;

    /**
     * @brief Deletes an assignment
     */
    exp_t<val_t> &operator=(exp_t<val_t> &) = delete;

    /**
     * @brief Move assignment
     */
    exp_t<val_t> &operator=(exp_t &&) = default;

    /**
     * @brief Checks if the current logical expression is true
     * @param kb Knowledge database
     * @return Check result
     */
    virtual bool is(const vals_t<val_t> &kb) const { return true; }

    /**
     * @brief Returns a required fact
     * @param kb Knowledge database
     * @return Required facts
     */
    virtual unknowns_t<val_t> unknowns(const vals_t<val_t> &kb) const {
        return unknowns_t<val_t>();
    }
};

/**
 * This class represents a fact. The fact is always true
 */
template <typename val_t>
class fact_t: public exp_t<val_t> {
    val_t m_value;
public:
    /**
     * @brief Constructor
     * @param value Fact value
     */
    fact_t(val_t &&value) : m_value{std::move(value)} {}

    /**
     * @brief Move constructor
     */
    fact_t(fact_t &&) = default;

    /**
     * Move assignment
     */
    fact_t<val_t> &operator=(fact_t &&) = default;

    /**
     * @inherits
     */
    virtual bool is(const vals_t<val_t> &kb) const override {
        return std::find(kb.begin(), kb.end(), m_value) != kb.end();
    }

    /**
     * @inherits
     */
    virtual unknowns_t<val_t> unknowns(const vals_t<val_t> &kb) const override {
        if (std::find(kb.begin(), kb.end(), m_value) != kb.end()) {
            return exp_t<val_t>::unknowns(kb);
        }
        return unknowns_t<val_t>({unknown_t(true, m_value)});
    }
};

/**
 * This class represents the logical negation
 */
template <typename val_t>
class not_t: public exp_t<val_t> {
    std::unique_ptr<exp_t<val_t>> m_exp;
public:
    /**
     * @brief Constructor
     * @param exp Nested logical expression
     */
    not_t(exp_t<val_t> *exp) : m_exp{exp} {}

    /**
     * @brief Move constructor
     */
    not_t(not_t &&other) : m_exp{std::move(other.m_exp)} {}

    /**
     * @brief Move assignment
     */
    not_t<val_t> &operator=(not_t &&other) {
        m_exp = std::move(other.m_exp);
        return *this;
    }

    /**
     * @inherits
     */
    virtual bool is(const vals_t<val_t> &kb) const override {
        return !m_exp->is(kb);
    }

    /**
     * @inherits
     */
    virtual unknowns_t<val_t> unknowns(const vals_t<val_t> &kb) const override {
        auto uks = m_exp->unknowns(kb);
        for (auto it = uks.begin(); it != uks.end(); ++it) {
            it->state != it->state;
        }
        return uks;
    }
};

template <typename val_t>
using exps_t = std::vector<std::unique_ptr<exp_t<val_t>>>;

/**
 * This class represents the logical conjunction
 */
template <typename val_t>
class and_t: public exp_t<val_t> {
protected:
    exps_t<val_t> m_exps;
public:
    /**
     * @brief Constructor
     * @param exps Nested logical expression
     */
    and_t(exps_t<val_t> &&exps) : exp_t<val_t>(), m_exps{std::move(exps)} {}

    /**
     * Move constructor
     */
    and_t(and_t &&) = default;

    /**
     * Move assignment
     */
    and_t<val_t> &operator=(and_t &&) = default;

    /**
     * @inherits
     */
    virtual bool is(const vals_t<val_t> &kb) const override {
        for (const auto &exp : m_exps) {
            if (!exp->is(kb)) { return false; }
        }
        return true;
    }

    /**
     * @inherits
     */
    virtual unknowns_t<val_t> unknowns(const vals_t<val_t> &kb) const override {
        if(m_exps.empty()) { return exp_t<val_t>::unknowns(kb); }

        auto uks = (*m_exps.begin())->unknowns(kb);
        for (auto it = m_exps.begin() + 1; it < m_exps.end(); ++it) {
            auto tmp = (*it)->unknowns(kb);
            if (tmp.size() > uks.size()) {
                uks = std::move(tmp);
            }
        }

        return uks;
    }
};

/**
 * This class represents the logical disjunction
 */
template <typename val_t>
class or_t: public and_t<val_t> {
public:
    /**
     * @brief Constructor
     * @param exps Nested logical expression
     */
    or_t(exps_t<val_t> &&exps) : and_t<val_t>{std::move(exps)} {}

    /**
     * Move constructor
     */
    or_t(or_t &&) = default;

    /**
     * Move assignment
     */
    or_t<val_t> &operator=(or_t &&) = default;

    /**
     * @inherits
     */
    virtual bool is(const vals_t<val_t> &kb) const override {
        for (const auto &exp : this->m_exps) {
            if (exp->is(kb)) { return true; }
        }
        return false;
    }

    /**
     * @inherits
     */
    virtual unknowns_t<val_t> unknowns(const vals_t<val_t> &kb) const override {
        if(this->m_exps.empty()) { return exp_t<val_t>::unknowns(kb); }

        auto uks = unknowns_t<val_t>();
        for (auto it = this->m_exps.begin(); it < this->m_exps.end(); ++it) {
            auto tmp = (*it)->unknowns(kb);
            // If a reachable branch was found the knowledge database has enough
            // facts to the expression was true
            if (tmp.empty()) { return unknowns_t<val_t>(); }
            // Plex branches into a single vector
            plex(uks, std::move(tmp));
        }

        return uks;
    }
private:
    void plex(unknowns_t<val_t> &uks, const unknowns_t<val_t> &&branch) const {
        for (auto i = branch.begin(); i != branch.end(); ++i) {
            auto j = std::find_if(uks.begin(), uks.end(), [i](auto obj) {
                return obj.value == i->value;
            });
            if (j == uks.end()) {
                uks.push_back(*i);
            } else if (!j->state && i->state) {
                j->state = true;
            }
        }
    }
};

template <typename val_t>
/**
 * @brief Creates a new fact
 * @param v Fact value
 * @return New fact
 */
fact_t<val_t> *_fact(val_t &&v) {
    return new fact_t<val_t>(std::move(v));
}

template <typename val_t>
/**
 * @brief Creates a new negation
 * @param exp Pointer of a nested logical expression
 * @return New negation
 */
not_t<val_t> *_not(exp_t<val_t> *exp) {
    return new not_t<val_t>(exp);
}

template <typename val_t>
/**
 * @brief Creates a new conjunction
 * @param exps Nested logical expression
 * @return New conjunction
 */
and_t<val_t> *_and(exps_t<val_t> &&exps) {
    return new and_t<val_t>(std::move(exps));
}

template <typename val_t>
/**
 * @brief Creates a new disjunction
 * @param exps Nested logical expression
 * @return New disjunction
 */
or_t<val_t> *_or(exps_t<val_t> &&exps) {
    return new or_t<val_t>(std::move(exps));
}

}

#endif // EXPRESSION_HPP
