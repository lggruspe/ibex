use crate::partition;
use std::collections::BTreeMap;

pub trait SymbolLike {
    fn epsilon() -> Self;
}

#[derive(Clone, Eq, Ord, PartialEq, PartialOrd)]
pub enum Symbol {
    EmptySet,
    Epsilon,
    Bracket(partition::Partition),
}

impl SymbolLike for Symbol {
    fn epsilon() -> Symbol {
        Symbol::Epsilon
    }
}

struct SymbolTable {
    table: BTreeMap<Symbol, usize>,
}

impl SymbolTable {
    fn new() -> Self {
        SymbolTable {
            table: BTreeMap::new(),
        }
    }

    /// Returns ID of inserted symbol.
    fn add(&mut self, symbol: Symbol) -> usize {
        if let Some(val) = self.table.get(&symbol) {
            *val
        } else {
            let id = self.table.len();
            self.table.insert(symbol, id);
            id
        }
    }
}
