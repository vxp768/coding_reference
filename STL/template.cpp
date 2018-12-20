#include<iostream>

template <class T>
T GetMax (T a, T b) 
{
  return (a>b?a:b);
}

//class templates
template <class T>
class mypair {
   T a, b;
  public:
   mypair (T first, T second) {
     a = first;
     b = second;
   }
   T getmax();
};

template <class T> // T is template parameter
T mypair<T>::getmax() // 1st T is return type, 2nd T is function's template param
{
 return a>b?a:b;
}

int main()
{
  int i = 5, j = 6;
  long l = 10, m = 5;
  std::cout<<GetMax(i,j)<<' ';
  std::cout<<GetMax(l,m)<<'\n';

  mypair <int> myobj(100,75);
  std::cout<<myobj.getmax();
}
