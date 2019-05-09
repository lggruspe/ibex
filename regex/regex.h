#pragma once
#include <memory>
#include <string>

namespace regex {

    class RegexTree {
        typedef std::shared_ptr<RegexTree> Re;
    public:
        std::string value;
        Re lhs, rhs;
        RegexTree(const std::string& symbol="", Re lhs=nullptr, Re rhs=nullptr)
        {
            this->value = symbol;
            this->lhs = lhs;
            this->rhs = rhs;
        }
    };

    typedef std::shared_ptr<RegexTree> Re;

    Re alternate(Re a, Re b)
    {
        return std::make_shared<RegexTree>("|", a, b);
    }

    Re concatenate(Re a, Re b)
    {
        return std::make_shared<RegexTree>("+", a, b);
    }

    Re closure(Re a)
    {
        return std::make_shared<RegexTree>("*", a);
    }

    Re symbol(const std::string& s)
    {
        return std::make_shared<RegexTree>(s);
    }

    std::string tostring(std::weak_ptr<RegexTree> a)
    {
        if (a.expired()) {
            return "";
        }
        auto sp = a.lock();
        if (sp->lhs == nullptr) {
            return sp->value;
        }
        if (sp->rhs == nullptr) {
            return "(" + sp->value + ", " + tostring(sp->lhs) + ")";
        }
        return "(" + sp->value + ", " + tostring(sp->lhs) + ", " + tostring(sp->rhs) + ")";
    }
}
