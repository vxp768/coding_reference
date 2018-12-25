#include <iostream>

using namespace std;

class X {
 int i;
public:
 X() { i = 0;}
 void set(int j) { i = j; }
 int read() const {return i;}
};

class Y {
 int i;
 //it is more common to make embedded object private so that implementation of set/read etc. can be changed if required
 X x;    //Embedded object....class object in class
public:
 Y() { i=0; }

 void set(int j) { i = j; }
 void set_x(int j) { x.set(j); } // "i" is private member of X so can't be accessed in Y directly..."x.i = j" is incorrect

 int read() const { return i;}
 int read_x() const { return x.read();}
};

int main() 
{
 Y y;
 y.set(47);
 y.set_x(37);
 cout <<"Y:"<<y.read()<<endl;
 cout <<"X:"<<y.read_x()<<endl;
}
