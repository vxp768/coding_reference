#include <iostream>

using namespace std;

class Base{
 int i;
public:
 Base(int j): i(j) { cout<<"Class Base constructor"<<endl;  }
 ~Base() { cout<<"Class Base Destructor"<<endl;  }
};

class A {
 int i;
public:
 A(int j): i(j) { cout<<"Class A constructor"<<endl;  }
 ~A() { cout<<"Class A Destructor"<<endl;  }
};

class B {
 int i;
public:
 B(int j): i(j) { cout<<"Class B constructor"<<endl; }
 ~B() {cout<<"Class B destructor"<<endl; }
};

class C {
 int i;
public:
 C(int j): i(j) { cout<<"Class C constructor"<<endl; }
 ~C() {cout<<"Class C destructor"<<endl; }
};

class D {
 int i;
public:
 D(int j): i(j) { cout<<"Class D constructor"<<endl; }
 ~D() {cout<<"Class D destructor"<<endl; }
};


class X: public Base {
 A a; 
 B b;
public:
 X(int j): Base(j), a(j), b(j) {cout<<"Class X constructor"<<endl;}  //The order of calls in initializer list does not affect
							   //which constructore gets called first
 ~X() {cout<<"Class X destructor"<<endl;}
};

class Y: public X {
 D d;
 C c;
public:
 Y(int i): X(i),c(i),d(i) {cout<<"Class Y constructor"<<endl;}
 ~Y() {cout<<"Class Y destructor"<<endl;}
};

/*
 Construction starts at very root of class heirarchy and at each base class constructor is
 called first, followed my member object constructors.
 The destructors are called in exact opposite order because of potential dependancies
*/

int main()
{
 Y y(2);
}
