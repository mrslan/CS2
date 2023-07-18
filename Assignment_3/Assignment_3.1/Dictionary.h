#pragma once
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Dictionary{
public:
    Dictionary(const string& dictionaryPath);
    bool search(string& searchWord);
    void print();
    void addMoreWords(const string& filePath);
    void getSuggestion(const string& word);
    int levenshteinDistance(const string& word1, const string& word2);
    string weirdWordFinder(const string& str);
private:
    vector < vector < string > > words;
    string cleanse(const string& word);
    int getWordPos(string& word);
};