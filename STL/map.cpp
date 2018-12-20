#include<iostream>
#include<string>
#include<map>


bool fncomp (char lhs, char rhs)
{
    return lhs<rhs;
}

void print_map_char(std::map<char,int> a)
{
    std::map<char,int>::iterator it;
    for(it = a.begin(); it!=a.end(); it++) {
        std::cout<<it->second<<' ';
    }
    std::cout<<'\n';
}

void print_map_str(std::map<std::string,int> a)
{
    std::map<std::string,int>::iterator it;
    for(it = a.begin(); it!=a.end(); it++) {
        std::cout<<it->second<<' ';
    }
    std::cout<<'\n';
}

int main()
{
 std::map<char, int> first;
 
 // map are always sorted by its keys
 // no two elements can have equvalent keys
 first['b'] = 30;
 first['a'] = 90;
 first['d'] = 50;
 first['c'] = 40;

 std::map<char,int> second (first.begin(), first.end());
 std::map<char,int> third (second);

 //function pointer as Compare
 bool (*fn_pt)(char, char) = fncomp;
 std::map<char,int, bool(*)(char,char)> fifth (fn_pt);   

 print_map_char(first);

 std::map<std::string,int> map_str = {
			{"beta",  0},
			{"alpha", 0},
			{"gamma", 0}};

 map_str.at("alpha")  = 10;
 map_str.at("gamma") = 30;

 print_map_str(map_str);

return 0;
}
