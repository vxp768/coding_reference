#include<iostream>
#include<list>

using namespace std;

void print_list(list<int> a)
{
    list<int>::iterator it;
    for (it=a.begin(); it != a.end(); it++) {
	cout<<*it<<' ';
    }
    cout<<endl;
}

int main()
{
 //constructors used
 list<int> first;                                //empty constructor
 list<int> second (4,100);                       //4 ints with value 100
 list<int> third (second.begin(), second.end()); //range constructor
 list<int> fourth (third);                       // a copy of third

 int arr[] = {1, 3, 5, 2};
 list<int> fifth (arr, arr+sizeof(arr)/sizeof(int));

 cout<<"content of fifth: "<<endl;
 print_list(fifth);
 
 fifth.push_back(9);
 fifth.push_front(10);
 cout<<"content after 2 push: "<<endl;
 print_list(fifth);


 fifth.pop_front();
 fifth.push_back(6);
 cout<<"content after pop and push: "<<endl;
 print_list(fifth);
  
return 0;
}
