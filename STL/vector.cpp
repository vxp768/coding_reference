#include<iostream>
#include<vector>

void print_vector(std::vector<int> a)
{
    std::vector<int>::iterator it;
    for (it=a.begin(); it != a.end(); it++) {
	std::cout<<*it<<' ';
    }
    std::cout<<'\n';
}

int main()
{
 std::vector<int> first;  				//empty vector of ints
 std::vector<int> second(4,100);  			//four ints with value=100
 std::vector<int> third (second.begin(), second.end()); //iterating through second
 std::vector<int> fourth (third);

 int arr[] = {1, 3, 5, 2};
 std::vector<int> fifth (arr, arr+sizeof(arr)/sizeof(int));

 std::cout<<"content of fifth: "<<'\n';
 print_vector(fifth);

 fifth.push_back(9);
 fifth.push_back(10);
 print_vector(fifth);

 std::cout<<"vector val at idx[2] = "<<fifth.at(2)<<" or "<<fifth[2]<<'\n';


 std::vector<int> v1[10];
 
}
