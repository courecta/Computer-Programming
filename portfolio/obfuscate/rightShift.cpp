#include <iostream>
#include <string>
using namespace std;

string right_shift(string s, int start, int end);

int main(){

    string s = "aahhab";
    cout << "Obfuscated string: " << right_shift(s, 2, 5) << endl;
    
}

string right_shift(string s, int start, int end)
{
    int length = end - start + 1;
    if(start == end || length > s.length() || s.length() <= 1) return s;
    
    string substring = s.substr(start, length);
    char last = substring[length - 1];
    for(int i = length - 1 ; i > 0 ; i--) substring[i] = substring[i - 1];
    substring[0] = last;

    s.replace(start, length, substring);

    return s;
}