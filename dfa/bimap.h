#ifndef BIMAP_H
#define BIMAP_H

#include <algorithm>
#include <map>
#include <set>

struct Bimap {
  std::map<int, std::set<int> > left;
  std::map<std::set<int>, int> right;

  const std::set<int>& get(int a)
  {
    return this->left[a];
  }

  int rget(const std::set<int>& a)
  {
    return this->right[a];
  }

  bool contains(int a)
  {
    return this->left.find(a) != this->left.end();
  }

  bool rcontains(const std::set<int>& a)
  {
    return this->right.find(a) != this->right.end();
  }

  void force_set(int a, const std::set<int>& b)
  {
    // set without checks
    this->left[a] = b;
    this->right[b] = a;
  }

  void set(int a, const std::set<int>& b)
  {
    auto it = this->left.find(a);
    auto jt = this->right.find(b);
    if (it != this->left.end()) {
      this->left.erase(it);
    }
    if (jt != this->right.end()) {
      this->right.erase(jt);
    }
    this->force_set(a, b);
  }
};
#endif
