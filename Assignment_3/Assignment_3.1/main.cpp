#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include "Dictionary.h"

using namespace std;

string remakeString(const string& str, const string& weirdWord);

int main(){

    Dictionary d("C:/Users/ryanh/Documents/CS2/LAB_10/LAB_10.2/bigDictionary.txt");
    string in = "";

    printf("input a string to spellcheck:\n");
    getline(cin, in);
    for (;d.weirdWordFinder(in) != "nww";){
        string weirdWord = d.weirdWordFinder(in);
        printf("\nweird word '%s' detected! here are some suggestions:\n", weirdWord.c_str());
        d.getSuggestion(weirdWord);
        in = remakeString(in, weirdWord);
    }
    main();
    return 0;
}

string remakeString(const string& str, const string& weirdWord){
    string retString = str;
    size_t pos = retString.find(weirdWord);
    if (pos != string::npos){
        retString.erase(pos, weirdWord.length());
        if (pos > 0 and retString[pos - 1] == ' '){
            retString.erase(pos - 1, 1);
        }
    }
    if (retString[0] == ' ') retString.erase(0, 1);
    return retString;
}
