#include <iostream>

using namespace std;

/*
 Constructor initializer list allows to explicitly call constructors for member objects.
 The idea is that constructors for all member objects are called before entering in 
 new class's constructor .
 Even the built-in types can be initialized in similar way. They can be treated as if they 
 have single constructor which takes single arguemnt.
*/

class A {
 int a;
public:
 A(int i) {a=i;}
};

class B {
 int b;
public:
 B(int i) {b=i;}
}

class X: public A {  //BASE CLASS A
  int i;
  float f;
  char c;
  char *s;
  B b; 		     //MEMBER CLASS B
public:
  X(): A(2), b(3), i(7), f(1.4), c('x'), s("howdy"){ }
};

int main()
{
  X x;
  int i(100);  //same as i=100
  int *p = new int(47);
}
