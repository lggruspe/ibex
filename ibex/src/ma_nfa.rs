use crate::nfa;
use crate::symbol;
use std::collections::{HashMap};
use std::rc::Rc;

type Q = u64;

pub struct MultiAcceptNfa<A: symbol::SymbolLike> {
    nfa: nfa::Nfa<A>,
    labels: HashMap<Q, Rc<nfa::Nfa<A>>>,
}

impl<A: symbol::SymbolLike + Clone + Ord> MultiAcceptNfa<A> {
    fn from(mut nfas: Vec<nfa::Nfa<A>>) -> MultiAcceptNfa<A> {
        let mut result = nfa::Nfa::empty();
        let mut labels = HashMap::new();
        while let Some(nfa) = nfas.pop() {
            let offset = result.order();
            result = result.union(&nfa);
            labels.insert(offset + nfa.accept, Rc::new(nfa));
        }
        MultiAcceptNfa { nfa: result, labels }
    }
}
