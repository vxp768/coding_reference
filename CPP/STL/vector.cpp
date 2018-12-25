#include <iostream>
#include <vector>

using namespace std;

int main()
{
atomic<int> x;
vector <int> vec;
vector <int> ::iterator it;
vector <int> :: reverse_iterator r_it;

 for (int i=0; i<=10; i++) {
    vec.push_back(i);
 }
 
 cout<<"Output from begin to end"<<endl;
 for(it = vec.begin(); it!=vec.end(); it++) {
     cout<<*it<<'\t';
 }
 cout<<endl; 
 cout<<"Output from end to begin"<<endl;
 for(r_it = vec.rbegin(); r_it!=vec.rend(); r_it++) {
     cout<<*r_it<<'\t';
 }
 cout<<endl; 
return 0;
}
