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
public:
 X x;    //Embedded object....class object in class
 Y() { i=0; }
 void set(int j) { i = j; }
 int read() const { return i;}
};

int main() 
{
 Y y;
 y.set(47);
 y.x.set(37);
 cout <<"Y:"<<y.read()<<endl;
 cout <<"X:"<<y.x.read()<<endl;
}
