#include "utilities.h"

Regex regex;

Re empty;
Re zero;
Re nonzero;
Re digit;
Re positive;
Re integer;
Re real;
Re lower;
Re upper;
Re letter;
Re identifier;

void re_load_defaults()
{
  empty = regex.symbol('\0');
  zero = regex.symbol('0');
  nonzero = regex.symbol('1');
  for (char a = '2'; a <= '9'; ++a) {
    nonzero = regex.alternate(nonzero, regex.symbol(a));
  }
  digit = regex.alternate(zero, nonzero);
  positive = regex.concatenate(nonzero, regex.closure(digit));
  integer = regex.alternate(zero, positive);

  {
    Re decimal = regex.concatenate(regex.symbol('.'), digit);
    decimal = regex.concatenate(decimal, regex.closure(digit));
    decimal = regex.optional(decimal);

    Re sign = regex.alternate(regex.symbol('+'), regex.symbol('-'));
    sign = regex.optional(sign);

    Re exponent = regex.alternate(regex.symbol('e'), regex.symbol('E'));
    exponent = regex.concatenate(exponent, sign);
    exponent = regex.concatenate(exponent, integer);
    exponent = regex.optional(exponent);

    real = regex.concatenate(integer, decimal);
    real = regex.concatenate(real, exponent);
  };

  lower = regex.symbol('a');
  for (char a = 'b'; a <= 'z'; ++a) {
    lower = regex.alternate(lower, regex.symbol(a));
  }
  
  upper = regex.symbol('A');
  for (char a = 'B'; a <= 'Z'; ++a) {
    upper = regex.alternate(upper, regex.symbol(a));
  }

  letter = regex.alternate(lower, upper);

  {
    Re first = regex.alternate(regex.symbol('_'), letter);
    Re rest = regex.alternate(first, digit);
    rest = regex.closure(rest);
    identifier = regex.concatenate(first, rest);
  }
}
