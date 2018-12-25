#include <iostream>

using namespace std;

class X {
 int i;
public:
 X() { i=0;}
 void set(int j) { i=j; }
 int read() const {return i;}
 int permute() { return i = i * 47; }
};

//Y contains subobject of X
class Y: public X {
 int i; //different from X's i
public:
 Y() { i=0; }
 int change() {
    i = permute(); //Different name function call...no scope resolution
    return i;
 } 
 void set(int j) {
    i = j;
    X::set(j);     //Same name funcion call...use scope resolution
 }
};

int main() 
{
    cout<<"sizeof(X) = "<<sizeof(X)<<endl;
    cout<<"sizeof(Y) = "<<sizeof(Y)<<endl; //private elements of X are still there and take up sapce but can't be accessed directly
    Y D;
    D.change();
    //class X's function "read" interface comes through
    D.read();
    D.permute(); //inherited as public member from X both read and permute...X is inherited as public
    //Redefined "set" function hides base version
    D.set(12);
}







