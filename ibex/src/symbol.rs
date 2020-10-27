use crate::partition;

#[derive(Clone, Eq, Ord, PartialEq, PartialOrd)]
pub enum Symbol {
    Bracket(partition::Partition),
    Epsilon,
    EmptySet,
}
