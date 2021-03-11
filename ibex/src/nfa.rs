use crate::symbol;
use std::collections::{BTreeMap, HashMap, HashSet};

type Q = u64;

struct Nfa<A: symbol::SymbolLike> {
    transitions: HashMap<Q, BTreeMap<A, HashSet<Q>>>,
    start: Q,
    accept: Q,
}

impl<A: symbol::SymbolLike + Clone + Ord> Nfa<A> {
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

    fn translate(mut self, offset: Q) -> Nfa<A> {
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

    fn merge(self, other: &Nfa<A>) -> Nfa<A> {
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

    pub fn empty() -> Nfa<A> {
        let mut transitions = HashMap::new();
        transitions.insert(0, BTreeMap::new());
        transitions.insert(1, BTreeMap::new());
        Nfa {
            transitions,
            start: 0,
            accept: 1,
        }
    }

    pub fn epsilon() -> Nfa<A> {
        let mut nfa = Nfa::empty();
        nfa.link(nfa.start, A::epsilon(), nfa.accept);
        nfa
    }

    pub fn single(c: A) -> Nfa<A> {
        let mut nfa = Nfa::empty();
        nfa.link(nfa.start, c, nfa.accept);
        nfa
    }

    pub fn union(self, other: &Nfa<A>) -> Nfa<A> {
        let mut nfa = self.merge(other);
        let start = nfa.order();
        let accept = start + 1;
        nfa.link(start, A::epsilon(), nfa.start);
        nfa.link(start, A::epsilon(), other.start);
        nfa.link(nfa.accept, A::epsilon(), accept);
        nfa.link(other.accept, A::epsilon(), accept);
        nfa.add(accept);
        nfa
    }

    pub fn concatenation(self, other: &Nfa<A>) -> Nfa<A> {
        let mut nfa = self.merge(other);
        let start = nfa.order();
        let accept = start + 1;
        nfa.link(start, A::epsilon(), nfa.start);
        nfa.link(start, A::epsilon(), other.start);
        nfa.link(nfa.accept, A::epsilon(), accept);
        nfa.link(other.accept, A::epsilon(), accept);
        nfa.add(accept);
        nfa
    }

    pub fn closure(mut self) -> Nfa<A> {
        let start = self.order();
        let accept = start + 1;
        self.link(start, A::epsilon(), self.start);
        self.link(self.accept, A::epsilon(), accept);
        self.link(self.accept, A::epsilon(), self.start);
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
