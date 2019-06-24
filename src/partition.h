#pragma once
#include <algorithm>
#include <map>
#include <set>
#include <vector>

/* used for partition refinement */
template <class T>
class Partition {
    using size_type = typename std::vector<std::vector<T>>::size_type;
    std::vector<std::vector<T>> classes; 
    std::map<T, size_type> class_indices;

public:
    template <class In>
    Partition(In b, In e)
    {
        classes.push_back({});
        auto it = classes.begin();
        std::for_each(b, e, [&it, this](const T& t) {
                    it->push_back(t);
                    (this->class_indices)[t] = 0;
                });
    }

    using iterator = typename std::vector<std::vector<T>>::iterator;
    using const_iterator = typename std::vector<std::vector<T>>::const_iterator;

    iterator begin() { return classes.begin(); }
    iterator end() { return classes.end(); }
    const_iterator begin() const { return classes.begin(); }
    const_iterator end() const { return classes.end(); }

    template <class Pred>
    bool split_by_predicate(std::vector<T>& cls, Pred pred)
    // pred has one parameter (T)
    {
        // returns true if anything changed
        auto it = std::partition(cls.begin(), cls.end(), pred);
        std::vector<T> new_cls;
        std::copy(it, cls.end(), std::back_inserter(new_cls));
        cls.erase(it, cls.end());

        if (cls.empty()) {
            cls = new_cls;
            return false;
        } else if (!new_cls.empty()) {
            size_type ind = classes.size();
            classes.push_back(new_cls);
            for (const auto& t: new_cls) {
                class_indices[t] = ind;
            }
            return true;
        }
        return false;
    }

    // split by predicate
    template <class Pred>   // parameter: type T
    void split_by_predicate(Pred pred)
    {
        bool changed = true;
        while (changed) {
            changed = false;
            for (std::vector<T>& cls: classes) {
                if (split_by_predicate(cls, pred)) {
                    changed = true;
                    break;
                }
            }
        }
    }

    // calling this once only splits cls by comparing with the first element
    template <class Compare>
    bool split_by_comparison(std::vector<T>& cls, Compare compare)
    {
        if (cls.empty()) {
            throw "empty equivalence class can't be split";
        }
        return split_by_predicate(cls, [&cls, &compare](const T& t) { 
                    return compare(t, cls[0]);
                });
    }


    // split all classes by pairwise comparisons
    template <class Compare> // (T, T) -> bool
    void split_by_comparison(Compare compare)
    {
        bool changed = true;
        while (changed) {
            changed = false;
            for (std::vector<T>& cls: classes) {
                if (split_by_comparison(cls, compare)) {
                    changed = true;
                    break;
                }
            }
        }
    }

    // split by input set
    void split_by_set(const std::set<T>& set)
    {
        split_by_predicate([&set](const T& t) {
                    return set.find(t) == set.end();
                });
    }

    // split by input container
    template <class In>
    void split_by_iterator(In b, In e)
    {
        split_by_predicate([&b, &e](const T& t) {
                    return std::find(b, e, t) == e;
                });
    }

    size_type index(const T& t) const
    {
        return class_indices.at(t);
    }
};
