use crate::symbol;
use std::collections::{BTreeMap, HashMap, HashSet};

type Q = u32;
type A = symbol::Symbol;

struct Nfa {
    transitions: HashMap<Q, BTreeMap<A, HashSet<Q>>>,
    start: Q,
    accept: Q,
}

impl Nfa {
    fn order(&self) -> Q {
        self.transitions.len() as Q // panics?
    }

    fn add(&mut self, q: Q) {
        if !self.transitions.contains_key(&q) {
            self.transitions.insert(q, BTreeMap::new());
        }
    }

    fn link(&mut self, q: Q, a: A, r: Q) {
        if let Some(map) = self.transitions.get_mut(&q) {
            if let Some(transitions) = map.get_mut(&a) {
                transitions.insert(r);
            } else {
                let transitions: HashSet<Q> = vec![r].into_iter().collect();
                map.insert(a, transitions);
            }
        } else {
            let transitions: HashSet<Q> = vec![r].into_iter().collect();
            let mut map = BTreeMap::new();
            map.insert(a, transitions);
            self.transitions.insert(q, map);
        }
    }

    fn translate(mut self, offset: Q) -> Nfa {
        self.start += offset;
        self.accept += offset;
        let transitions = self.transitions;
        self.transitions = HashMap::new();
        for (state, map) in transitions.iter() {
            for (key, set) in map.iter() {
                for val in set.iter() {
                    self.link(*state, key.clone(), *val);
                }
            }
        }
        self
    }

    fn merge(self, other: &Nfa) -> Nfa {
        let offset = other.order();
        let mut nfa = self.translate(offset);
        for (state, map) in other.transitions.iter() {
            for (key, set) in map.iter() {
                for val in set.iter() {
                    nfa.link(*state, key.clone(), *val);
                }
            }
        }
        nfa
    }

    pub fn empty() -> Nfa {
        let mut transitions = HashMap::new();
        transitions.insert(0, BTreeMap::new());
        transitions.insert(1, BTreeMap::new());
        Nfa {
            transitions,
            start: 0,
            accept: 1,
        }
    }

    pub fn epsilon() -> Nfa {
        let mut nfa = Nfa::empty();
        let map = nfa.transitions.get_mut(&nfa.start).unwrap();
        if let Some(transitions) = map.get_mut(&symbol::Symbol::Epsilon) {
            transitions.insert(nfa.accept);
        } else {
            let mut transitions = HashSet::new();
            transitions.insert(nfa.accept);
            map.insert(symbol::Symbol::Epsilon, transitions);
        }
        nfa
    }

    pub fn single(c: symbol::Symbol) -> Nfa {
        let mut nfa = Nfa::empty();
        nfa.link(nfa.start, c, nfa.accept);
        nfa
    }

    pub fn union(self, other: &Nfa) -> Nfa {
        let mut nfa = self.merge(other);
        let start = nfa.order();
        let accept = start + 1;
        let eps = symbol::Symbol::Epsilon;
        nfa.link(start, eps.clone(), nfa.start);
        nfa.link(start, eps.clone(), other.start);
        nfa.link(nfa.accept, eps.clone(), accept);
        nfa.link(other.accept, eps, accept);
        nfa.add(accept);
        nfa
    }

    pub fn concatenation(self, other: &Nfa) -> Nfa {
        let mut nfa = self.merge(other);
        let start = nfa.order();
        let accept = start + 1;
        let eps = symbol::Symbol::Epsilon;
        nfa.link(start, eps.clone(), nfa.start);
        nfa.link(start, eps.clone(), other.start);
        nfa.link(nfa.accept, eps.clone(), accept);
        nfa.link(other.accept, eps, accept);
        nfa.add(accept);
        nfa
    }

    pub fn closure(mut self) -> Nfa {
        let start = self.order();
        let accept = start + 1;
        let eps = symbol::Symbol::Epsilon;
        self.link(start, eps.clone(), self.start);
        self.link(self.accept, eps.clone(), accept);
        self.link(self.accept, eps, self.start);
        self.add(accept);
        self
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 3, 3);
    }
}
