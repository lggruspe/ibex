package scanners

type Scanner interface {
    State() int
    ChangeState(nextState int, checkpoint bool) bool
    Next(char rune) bool
    BacktrackSteps() int
}

type BaseScanner struct {
    checkpoint []int
    Token string
    accepts false
}

func (scanner *BaseScanner) State() {
    n := len(scanner.checkpoint)
    if n == 0 {
        return -1
    }
    return scanner.checkpoint[n-1]
}

func (scanner *BaseScanner) ChangeState(nextState int, checkpoint bool) bool {
    if checkpoint {
        scanner.checkpoint = make([]int)
        scanner.accepts = true
    }
    scanner.checkpoint = append(scanner.checkpoint, nextState)
    return nextState != -1
}

func (scanner *BaseScanner) BacktrackSteps() int {
    return len(scanner.checkpoint) - 1
}

func (scanner *BaseScanner) String() string {
    return fmt.Sprintf("<Scanner %q state:%d checkpoint:%v>", self.Token, self.State(), self.checkpoint)
}

func IdentifierScanner() BaseScanner {
    type temp struct IdentifierScanner {
        BaseScanner
    }

    func (scanner *temp) Next(char rune) bool {
        switch state := scanner.State(); state {
        case 0:
            switch rune {
            case 'a':
            default:
                //error
            }
        case 1:
        case 2:
        default:
            // error
        }
    }

    scanner := BaseScanner{
        checkpoint: []int{0},
        Token: "identifier",
        accepts: false,
    }
    return scanner
}

func main() {
    scanner := IdentifierScanner()
    scanner.Next()

}
