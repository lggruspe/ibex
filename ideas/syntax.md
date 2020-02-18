Definitions:

```c
#include "templates/vector.h"

VECTOR_GENERATE(_operand_v, int)
VECTOR_GENERATE(_operator_v, char)

_operand_v_type operands_p = _operand_v_create();
_operator_v_type operators_p = _operator_v_create();

void push_operand(int op)
{
    _operand_v_push(&operands_p, op); 
}

int pop_operand()
{
    return _operand_v_pop(&operands_p);
}

void push_operator(char op)
{
    _operator_v_push(&operators_p, op);
}

char pop_operator()
{
    return _operator_v_pop(&operators_p);
}
```

Reduce `E -> E + T` using "add.c".

Reduce `E -> E - T`:

```C
    pop_operator();
    int b = pop_operand();
    int a = pop_operand();
    push_operand(a-b);
```

Reduce `E -> T`.

Reduce `T -> T * F` using "multiply.c".

Reduce `T -> T / F`:

```c
pop_operator();
int b = pop_operand();
int a = pop_operand();
if (b == 0) {
    throw_exception(E_DIVISION_BY_ZERO);
}
push_operand(a/b);
```
<!--or just write function body?-->

Reduce `F -> $number`.
<!--no action-->

Accept.

```c
int accept()
{
    return pop_operand();
}
```

Shift `+`:

~~~c
void shift_plus()
{
    push_operator('+');
}
~~~

Shift `-` using "minus.c".

Shift `*` using "multiply.c".

Shift `/` with `push_operator($this)`.
<!--inline shift callback-->

Shift `$number`:

```c
void shift_number()
{
    String lexeme = get_current_lexeme();
    int n = 0;
    for (int i = lexeme.length - 1; i >= 0; --i) {
        n *= 10;
        n += lexeme.string[i] - '0';
    }
    push_operand(n);
}
```
