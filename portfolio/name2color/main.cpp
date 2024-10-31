#include <iostream>
#include <string>
using namespace std;

int name_to_color(string name, int color_chart[26]);

int main(){

    int letter_to_number[26] = {0};
    for(int i = 0 ; i < 26 ; i++) letter_to_number[i] = i + 1;
    string first, middle, last;
    int rgb[3];

    cout << "Enter first name: ";
    getline(cin, first);
    rgb[0] = 4 * name_to_color(first, letter_to_number);

    cout << "Enter middle name: ";
    getline(cin, middle);
    rgb[1] = 4 * name_to_color(middle, letter_to_number);

    cout << "Enter last name: ";
    getline(cin, last);
    rgb[2] = 4 * name_to_color(last, letter_to_number);

    cout << "Your name color is ";
    for(int i = 0; i < 3 ; i++)
    {
        while(rgb[i] > 255) rgb[i] -= 256;
        cout << rgb[i] << ":";
    }

    cout << "\b \nYour Hex Value: https://colorkit.co/color/";
    for(int i = 0 ; i < 3 ; i++) cout << hex << rgb[i] / 16 << hex << rgb[i] % 16;
    printf("/\n");
    
    return 0;
}

int name_to_color(string name, int color_chart[26])
{
    int sum = 0, character = 0;
    for(int i = 0 ; i < name.length() ; i++)
    {
        character = name[i];
        if((character >= 65 && character <= 90) || (character >= 97 && character <= 122)) sum += color_chart[toupper(name[i]) - 'A'];
    }
    return sum;
}