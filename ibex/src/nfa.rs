use crate::symbol;
use std::collections::{BTreeMap, HashMap, HashSet};

type Q = u64;

pub struct Nfa<A: symbol::SymbolLike> {
    transitions: HashMap<Q, BTreeMap<A, HashSet<Q>>>,
    start: Q,
    pub accept: Q,
}

/// Invariants:
/// - States are a permutation of 0, 1, ..., order.
/// - Has one start state with no incoming transitions.
/// - Has one accept state with no outgoing transitions.
impl<A: symbol::SymbolLike + Clone + Ord> Nfa<A> {
    pub fn order(&self) -> Q {
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

    fn merge(&mut self, other: &Nfa<A>) {
        let offset = self.order();
        for (state, map) in other.transitions.iter() {
            for (key, set) in map.iter() {
                for val in set.iter() {
                    self.link(*state + offset, key.clone(), *val + offset);
                }
            }
        }
        self.add(other.start + offset);
        self.add(other.accept + offset);
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

    pub fn union(mut self, other: &Nfa<A>) -> Nfa<A> {
        self.merge(other);
        let mut nfa = self;
        let start = nfa.order();
        let accept = start + 1;
        nfa.link(start, A::epsilon(), nfa.start);
        nfa.link(start, A::epsilon(), other.start);
        nfa.link(nfa.accept, A::epsilon(), accept);
        nfa.link(other.accept, A::epsilon(), accept);
        nfa.add(accept);
        nfa.start = start;
        nfa.accept = accept;
        nfa
    }

    pub fn concatenation(mut self, other: &Nfa<A>) -> Nfa<A> {
        self.merge(other);
        let mut nfa = self;
        let start = nfa.order();
        let accept = start + 1;
        nfa.link(start, A::epsilon(), nfa.start);
        nfa.link(start, A::epsilon(), other.start);
        nfa.link(nfa.accept, A::epsilon(), accept);
        nfa.link(other.accept, A::epsilon(), accept);
        nfa.add(accept);
        nfa.start = start;
        nfa.accept = accept;
        nfa
    }

    pub fn closure(mut self) -> Nfa<A> {
        let start = self.order();
        let accept = start + 1;
        self.link(start, A::epsilon(), self.start);
        self.link(self.accept, A::epsilon(), accept);
        self.link(self.accept, A::epsilon(), self.start);
        self.add(accept);
        self.start = start;
        self.accept = accept;
        self
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::symbol;

    #[derive(Clone, Eq, Ord, PartialEq, PartialOrd)]
    enum Symbol {
        Epsilon,
        Character(char),
    }

    impl symbol::SymbolLike for Symbol {
        fn epsilon() -> Symbol {
            Symbol::Epsilon
        }
    }

    #[test]
    fn nfa_order_returns_number_of_states() {
        assert_eq!(Nfa::<Symbol>::empty().order(), 2);
        assert_eq!(Nfa::<Symbol>::epsilon().order(), 2);

        let nfa = Nfa::single(Symbol::Character('c'));
        assert_eq!(nfa.order(), 2);

        let nfa = nfa.closure();
        assert_eq!(nfa.order(), 4);

        let other = Nfa::<Symbol>::empty();
        let nfa = nfa.union(&other);
        assert_eq!(nfa.order(), 8);

        let other = Nfa::<Symbol>::epsilon();
        let nfa = nfa.concatenation(&other);
        assert_eq!(nfa.order(), 12);
    }
}
