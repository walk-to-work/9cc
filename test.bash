#!/bin/bash
try() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

try 0 '0;'
try 42 '42;'
try 21 '5+20-4;'
try 41 '12 + 34 - 5;'
try 47 '5+6*7;'
try 15 '5*(9-6);'
try 4 '(3+5)/2;'
try 8 '-(-3+-5);'
try 8 '-(-3-5);'
try 4 '-(-3-5)/2;'
try 15 '-3*-5;'
try 0 '1==2;'
try 1 '1==1;'
try 1 '1 == 1;'
try 1 '2 > 1;'
try 0 '1 > 1;'
try 1 '3 >= 2;'
try 1 '2 >= 2;'
try 0 '1 >= 2;'
try 0 '2 < 1;'
try 0 '1 < 1;'
try 1 '0 < 1;'
try 0 '3 <= 2;'
try 1 '2 <= 2;'
try 1 '1 <= 2;'
try 1 'a=1;'
try 1 'a=b=1;'
try 2 'b=2;'
try 2 'a=1; a+1;'
try 2 'foo=2;'
try 5 'foo=5;'
try 4 'a=4; b=3; a;'
try 3 'a=4; b=3; b;'
try 7 'a=4; b=3; a+b;'
try 2 'a=b=1; a+b;'
try 2 'foo=bar=1; foo+bar;'
try 5 'foo=2; bar=3; foo+bar;'
try 6 'a=b=3; c=1; a+b;'
try 1 'a=b=3; c=1; c;'
try 6 'foo=bar=3; hoo=1; foo+bar;'
try 1 'foo=bar=3; hoo=1; hoo;'
try 10 'foo=2; bar=3; hoo=5; foo+bar+hoo;'
try 7 'a=b=3; c=1; a+b+c;'
try 3 'a=b=1; c=3; (a+b)*c/2;'
try 3 'foo=bar=1; hoo=3; (foo+bar)*hoo/2;'
try 25 'a=b=c=d=e=5; a+b+c+d+e;'

echo OK
