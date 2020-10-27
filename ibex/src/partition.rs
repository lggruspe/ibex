use std::collections::BTreeSet;

#[derive(Clone, Eq, Ord, PartialEq, PartialOrd)]
struct BoundaryPoint(char);

struct Interval(BoundaryPoint, BoundaryPoint);

impl Interval {
    fn is_empty(&self) -> bool {
        self.0 == self.1
    }

    fn single(c: char) -> Self {
        let end = std::char::from_u32(c as u32 + 1).unwrap(); // panics?
        Interval(BoundaryPoint(c), BoundaryPoint(end))
    }
}

#[derive(Clone, Eq, Ord, PartialEq, PartialOrd)]
pub struct Partition {
    not: bool,
    set: BTreeSet<BoundaryPoint>,
}

impl Partition {
    fn insert(&mut self, range: Interval) {
        if !range.is_empty() {
            self.set.insert(range.0);
            self.set.insert(range.1);
        }
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 3);
    }
}
