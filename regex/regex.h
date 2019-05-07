#ifndef REGEX_H
#define REGEX_H

#include <iostream>
#include <memory>
#include <map>
#include <vector>

struct RegexTree {
  char value;
  typedef std::weak_ptr<RegexTree> Re;
  Re lhs, rhs;

  RegexTree(char value, Re lhs=Re(), Re rhs=Re())
  {
    this->value = value;
    this->lhs = lhs;
    this->rhs = rhs;
  }
};

typedef std::weak_ptr<RegexTree> Re;

struct Regex {
  std::map<char, std::shared_ptr<RegexTree> > symbols;
  std::vector<std::shared_ptr<RegexTree> > expressions;  // excludes symbols
  Re empty;

  Regex()
  {
    this->empty = symbol('\0');
  }

  Re symbol(char a)
  {
    // symbols can only be defined once
    auto it = this->symbols.find(a);
    if (it != this->symbols.end()) {
      return it->second;
    }

    std::shared_ptr<RegexTree> symbol = std::make_shared<RegexTree>(a);
    this->symbols[a] = symbol;
    return symbol;    // do shared_ptrs get freed when goign out of scope?
  }

  Re expression(char value, Re A, Re B)
  {
    std::shared_ptr<RegexTree> C;
    C = std::make_shared<RegexTree>(value, A, B);
    this->expressions.push_back(C);
    return C;
  }

  Re alternate(Re A, Re B)
  {
    return this->expression('|', A, B);
  }

  Re concatenate(Re A, Re B)
  {
    return this->expression('+', A, B);
  }

  Re closure(Re A)
  {
    return this->expression('*', A, Re());
  }

  Re optional(Re A)
  {
    return this->alternate(this->empty, A);
  }
};

#endif
