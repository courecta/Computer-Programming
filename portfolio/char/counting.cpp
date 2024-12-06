#include <iostream>
#include <string>
using namespace std;

int main(){
    string str;
    cin >> str;
    int occurrences[26] = {0};
    for(int i = 0 ; i < str.length(); i++)
    {
        if(str[i] > 'a' && str[i] < 'z') occurrences[str[i] - 'a']++;
        else occurrences[str[i] - 'A']++;
    }
    for(int i = 0 ; i < 26; i++) cout << (char)(i + 'a') << " : " << occurrences[i] << endl;
}