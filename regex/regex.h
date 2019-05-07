#ifndef REGEX_H
#define REGEX_H

#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <vector>

struct RegexTree {
  bool leaf;
  char value;
  typedef std::weak_ptr<RegexTree> Re;
  Re lhs, rhs;

  RegexTree(char value, Re lhs=Re(), Re rhs=Re())
  {
    this->leaf = false;
    this->value = value;
    this->lhs = lhs;
    this->rhs = rhs;
  }
};

typedef std::weak_ptr<RegexTree> Re;

struct Regex {
  std::map<char, std::shared_ptr<RegexTree> > symbols;
  std::vector<std::shared_ptr<RegexTree> > expressions;  // excludes symbols
  std::map<std::string, Re> named_expressions;
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
    symbol->leaf = true;
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

  Re set(const std::string& name, Re re)
  {
    this->named_expressions[name] = re;
    return re;
  }

  Re get(const std::string& name)
  {
    return this->named_expressions[name];
  }

  Re get(const std::string& name, Re re)
  {
    // get, set if not defined
    auto it = this->named_expressions.find(name);
    if (it == this->named_expressions.end()) {
      return this->set(name, re);
    }
    return it->second;
  }
};

#endif
