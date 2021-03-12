use std::collections::{HashMap, HashSet};

type Q = u64;
type A = u64;

struct Dfa {
    transitions: HashMap<Q, HashMap<A, Q>>,
    start: Q,
    accepts: HashSet<Q>,
}

impl Dfa {
    fn new() -> Dfa {
        let mut transitions = HashMap::new();
        transitions.insert(0, HashMap::new());
        Dfa {
            transitions,
            start: 0,
            accepts: HashSet::new(),
        }
    }

    fn add(&mut self, q: Q) {
        if !self.transitions.contains_key(&q) {
            self.transitions.insert(q, HashMap::new());
        }
    }

    fn accept(&mut self, q: Q) {
        self.add(q);
        self.accepts.insert(q);
    }

    fn link(&mut self, q: Q, a: A, r: Q) {
        self.add(q);
        self.add(r);
        let map = self.transitions.get_mut(&q).unwrap();
        map.insert(a, r);
    }
}
