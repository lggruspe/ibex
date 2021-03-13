mod dfa;
pub mod nfa;
mod partition;
mod symbol;

mod ma_nfa;

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }
}
