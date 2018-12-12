#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <algorithm>
#include <memory>
#include <vector>

namespace xpertium {

template <typename val_t> using  vals_t = std::vector<val_t>;

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
    fact_t(val_t &&value) : m_value{std::forward<val_t>(value)} {}

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
    not_t(exp_t<val_t> *exp) : m_exp(exp) {}

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
};

template <typename val_t> using  exps_t = std::vector<exp_t<val_t>>;

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
    and_t(exps_t<val_t> &&exps) : exp_t<val_t>(),
        m_exps{std::forward<exps_t<val_t>>(exps)} {}

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
        for (auto &exp : m_exps) {
            if (!exp.is(kb)) { return false; }
        }
        return true;
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
    or_t(exps_t<val_t> &&exps):
        and_t<val_t>{std::forward<exps_t<val_t>>(exps)} {}

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
        for (auto &exp : this->m_exps) {
            if (exp.is(kb)) { return true; }
        }
        return false;
    }
};

template <typename val_t>
/**
 * @brief Creates a new fact
 * @param v Fact value
 * @return New fact
 */
fact_t<val_t> _fact(val_t &&v) {
    return std::move(fact_t<val_t>(std::forward<val_t>(v)));
}

template <typename val_t>
/**
 * @brief Creates a new negation
 * @param exp Nested logical expression
 * @return New negation
 */
not_t<val_t> _not(exp_t<val_t> &&exp) {
    return std::move(not_t<val_t>(std::forward<exp_t<val_t>>(exp)));
}

template <typename val_t>
/**
 * @brief Creates a new conjunction
 * @param exps Nested logical expression
 * @return New conjunction
 */
and_t<val_t> _and(exps_t<val_t> &&exps) {
    return std::move(and_t<val_t>(std::forward<exps_t<val_t>>(exps)));
}

template <typename val_t>
/**
 * @brief Creates a new disjunction
 * @param exps Nested logical expression
 * @return New disjunction
 */
or_t<val_t> _or(exps_t<val_t> &&exps) {
    return std::move(or_t<val_t>(std::forward<exps_t<val_t>>(exps)));
}

}

#endif // EXPRESSION_HPP
