#include <iostream>
#include <map>
using namespace std;
int main()
{
 map<char,int> char_map;
 map<string,int> str_map = {
			  {"alpha",0},
			  {"beta", 0},
			  {"gamma",0} };
 map<char,int>::iterator it;
 char_map['a'] = 10;
 char_map['b'] = 20;
 char_map['c'] = 30;
 char_map['d'] = 40;
 char_map['e'] = 50;
 for (it = char_map.begin(); it != char_map.end(); ++it) { 
      cout<<it->first<<":"<<it->second<<"\n";
 }

 str_map.at("alpha") = 10;
 str_map["beta"]     = 20;
 str_map["gamma"]    = 30;

 for (auto& x: str_map) {
   cout << x.first << ": " << x.second << '\n';
 }
}
