#ifndef TRACER_HPP
#define TRACER_HPP

#include "rule.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace xpertium {

template <typename val_t>
class base_tracer_t {
public:
    base_tracer_t() {}
    virtual ~base_tracer_t() {}
    virtual void push_fact(val_t fact) = 0;
    virtual void push_rule(const rule_t<val_t> *rule, val_t out) = 0;
    virtual void print() = 0;
};

template <typename val_t>
class tracer_t : public base_tracer_t<val_t> {
    std::vector<std::string> m_trace;
public:
    virtual void push_fact(val_t fact) override {
        std::string str = "+ fact: <" + to_string(fact) + ">";
        m_trace.push_back(std::move(str));
    }

    virtual void push_rule(const rule_t<val_t> *rule, val_t out) override {
        std::string str = "+ ";
        if (rule->target()) { str += "tget: "; }
        else { str += "rule: "; }
        str += "<" + rule->id() + "> -> <" + to_string(out) + ">";
        m_trace.push_back(std::move(str));
    }

    virtual void print() override {
        std::cout << "Trace: " << std::endl;
        for (auto it = m_trace.begin(); it != m_trace.end(); ++it) {
            std::cout << (*it) << std::endl;
        }
    }
private:
    auto to_string(val_t val) {
        if constexpr (std::is_same<val_t, std::string>::value) {
            return static_cast<std::string>(val);
        } else {
            return std::to_string(val);
        }
    }
};

}

#endif // TRACER_HPP
