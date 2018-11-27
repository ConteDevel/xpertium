#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <vector>

template <typename val_t>
class exp_t {
public:
    using vals_t = std::vector<val_t>;

    exp_t() {}
    virtual ~exp_t() {}

    virtual bool is(const vals_t &kb) const { return true; }
    virtual int unknown(const vals_t &kb) const { return 0; }
    virtual vals_t unknown_facts(const vals_t &kb) const {
        return vals_t();
    }
};

template <typename val_t>
class fact_t: public exp_t<val_t> {
public:
    using vals_t = std::vector<val_t>;

    const val_t m_value;

    fact_t(val_t &&value): m_value{std::forward<val_t>(value)} {}

    virtual bool is(const vals_t &kb) const override {
        return std::find(kb.begin(), kb.end(), m_value) != kb.end();
    }

    virtual int unknown(const vals_t &kb) const override {
        return is(kb) ? 0 : 1;
    }

    virtual vals_t unknown_facts(const vals_t &kb) const override {
        auto facts = exp_t<val_t>::unknown_facts(kb);
        if (!is(kb)) facts.push_back(m_value);
        return facts;
    }
};

template <typename val_t>
class not_t: public exp_t<val_t> {
    std::unique_ptr<exp_t<val_t>> m_exp;
public:
    using vals_t = std::vector<val_t>;

    not_t(exp_t<val_t> *exp)
        : exp_t<val_t>(), m_exp(exp) {}

    virtual bool is(const vals_t &kb) const override {
        return !m_exp->is(kb);
    }

    virtual int unknown(const vals_t &kb) const {
        return m_exp->unknown(kb);
    }

    virtual vals_t unknown_facts(const vals_t &kb) const override {
        return m_exp->unknown_facts(kb);
    }
};

template <typename val_t>
class and_t: public exp_t<val_t> {
protected:
    std::vector<exp_t<val_t> *> m_exps;
public:
    using vals_t = std::vector<val_t>;

    and_t(std::initializer_list<exp_t<val_t> *> exps)
        : exp_t<val_t>(), m_exps{exps} {}
    virtual ~and_t() {
        for (auto it = m_exps.begin() ; it != m_exps.end(); ++it) {
             delete (*it);
        }
        m_exps.clear();
    }

    virtual bool is(const vals_t &kb) const override {
        for (auto &exp : m_exps) {
            if (!exp->is(kb)) { return false; }
        }
        return true;
    }

    virtual int unknown(const vals_t &kb) const {
        int count = 0;

        for (auto exp : m_exps) count += exp->unknown(kb);

        return count;
    }

    virtual vals_t unknown_facts(const vals_t &kb) const override {
        auto facts = exp_t<val_t>::unknown_facts(kb);
        for (auto exp : m_exps) {
            for (auto fact : exp->unknown_facts(kb)) facts.push_back(fact);
        }
        return facts;
    }
};

template <typename val_t>
class or_t: public and_t<val_t> {
public:
    using vals_t = std::vector<val_t>;

    or_t(std::initializer_list<exp_t<val_t> *> exps): and_t<val_t>(exps) {}

    virtual bool is(const vals_t &kb) const override {
        for (auto &exp : this->m_exps) {
            if (exp->is(kb)) { return true; }
        }
        return false;
    }
};

template <typename val_t>
fact_t<val_t> *_fact(val_t &&v) {
    return new fact_t<val_t>(std::forward<val_t>(v));
}

template <typename val_t>
not_t<val_t> *_not(exp_t<val_t> *exp) {
    return new not_t<val_t>(exp);
}

template <typename val_t, typename ... args_t>
and_t<val_t> *_and(args_t *... exps) {
    return new and_t<val_t>({exps...});
}

template <typename val_t, typename ... args_t>
or_t<val_t> *_or(args_t *... exps) {
    return new or_t<val_t>({exps...});
}

#endif // EXPRESSION_HPP
