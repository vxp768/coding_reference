#include<iostream>
#include<set>

// NO duplicates allowed in set...multiset allows duplicates
// elements stored in soted order

bool fncomp (int lhs, int rhs)
{
    return lhs< rhs;
}

void print_set(std::set<int> a)
{
 std::set<int>::iterator it;
 for(it=a.begin(); it!=a.end(); it++) {
     std::cout<<*it<<' ';
 }
 std::cout<<'\n';
}

int main()
{

 std::set<int> first;
 int arr[] = {10, 40, 30, 50, 10};
 std::set<int> second (arr, arr+sizeof(arr)/sizeof(int));

 std::set<int> third(second);

 std::set<int> fourth(second.begin(), second.end());

 bool (*fn_pt)(int, int) = fncomp;
 std::set<int,bool(*)(int,int)> sixth(fn_pt);

 second.insert(60);
 print_set(second);

return 0;
}
