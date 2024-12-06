#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string obfuscate(string str);

int main(){
    string s = "aahhbb";
    cout << "Obfuscated string: " << obfuscate(s) << endl;
}

string obfuscate(string str)
{
    if(str.length() <= 1) return str;

    string result = str;
    int length = result.length();

    char minChar = result[length - 1];
    int minPos = length - 1;

    for(int i = length - 2 ; i >= 0 ; i--)
    {
        if(result[i] < minChar)
        {
            swap(result[i], result[minPos]);
            minChar = result[i];
            minPos = i;
        } else if(result[i] < result[minPos]) minPos = i;
    }
    return result;
}