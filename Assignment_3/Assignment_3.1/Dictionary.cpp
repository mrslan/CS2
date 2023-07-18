#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "Dictionary.h"
 Dictionary::Dictionary(const string& dictionaryPath){
    words.resize(260);

    ifstream infile;
    infile.open(dictionaryPath);

    if (infile.good()){
        string word;
        infile >> word;

        for (int i = 0; infile.good(); i++){
            word = cleanse(word);
            words[getWordPos(word)].push_back(word);
            infile >> word;
        }
    }
    else{
        printf("Cannot open this file: %s", dictionaryPath.c_str());
    }
}

void Dictionary::getSuggestion(const string& word){
    //each time it finds a word closer in levdistance than the current suggestions[suggestions.size() - 1] to the word it's looking--
    //for it pushes it back into a vector so you don't exactly end up with only the closest words, kinda flawed now that I'm thinking about it ngl...
    vector < string > suggestions;
    string closestWord = "";
    for (int i = 0; i < words.size(); i++){
        for (int j = 0; j < words[i].size(); j++){
            if (levenshteinDistance(words[i][j], word) <= levenshteinDistance(closestWord, word)){
                closestWord = words[i][j];
                suggestions.push_back(closestWord);
            }
        }
    }
    
    //handles the outputting of the suggestions and the prompting for more suggestions.
    int elementsToShow = 5;
    int currentIndex = suggestions.size() - 1;
    int count = 0;
    char inputPrompt;
    do {
        int count = 0;
        for (int i = currentIndex; i >= 0 and count < elementsToShow; i--, count++) {
            printf("%s\n", suggestions[i].c_str());
        }

        cout << "\nDo you want to see more suggestions? (y/n): ";
        cin >> inputPrompt;

        if (inputPrompt == 'y' or inputPrompt == 'Y') {
            currentIndex -= elementsToShow;
            if (currentIndex < 0) {
                currentIndex = 0;
            }
        } 
        else break;
    } while (currentIndex > 0);
}

int Dictionary::levenshteinDistance(const string& word1, const string& word2) {
    int m = word1.length();
    int n = word2.length();

    vector < vector < int > > dp(m + 1, vector < int > (n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        dp[i][0] = i;
    }

    for (int i = 1; i <= n; ++i) {
        dp[0][i] = i;
    }

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (word1[i - 1] == word2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }
    return dp[m][n];
}

string Dictionary::weirdWordFinder(const string& str){
    vector < string > vec;
    string word = "";

    for (int i = 0; i < str.size(); i++){
        if (str[i] == ' '){
            word = cleanse(word);
            vec.push_back(word);
            word = "";
        }
        else{
            word += str[i];
        }
    }

    word = cleanse(word);
    if (word != ""){
        vec.push_back(word);
    }
    
    for (int i = 0; i < vec.size(); i++){
        if (!search(vec[i])){
            return vec[i];
        }
    }
    return "nww"; //"no weird word" lol
}

void Dictionary::addMoreWords(const string& filePath){
    ifstream infile;
    infile.open(filePath);

    if (infile.good()){
        vector < int > indices;
        string word;
        infile >> word;

        for (int i = 0; infile.good();){                    
            word = cleanse(word);
            bool existsInIndices = false;
            for (int j = 0; j < indices.size(); j++){
                if (indices[j] == getWordPos(word)){
                    existsInIndices = true;
                }
            }
            if (existsInIndices == false){
                indices.push_back(getWordPos(word));
            }
            words[getWordPos(word)].push_back(word);
            infile >> word;                                            
        }
        for (int i = 0; i < indices.size(); i++){
            sort(words[indices[i]].begin(), words[indices[i]].end());
        }
    }
    
}

int Dictionary::getWordPos(string& word){
    word = cleanse(word);
    int charIndex = (int(word[0]) - 97);
    int wordLengthIndex;
    
    if (word.size() > 10) {
        wordLengthIndex = 9;
    }
    else{
       wordLengthIndex = word.size() - 1;
    }
    
    return stoi(to_string(charIndex) + to_string(wordLengthIndex));
}

void Dictionary::print(){
    for (int i = 0; i < words.size(); i++){
        for (int j = 0; j < words[i].size(); j++){
            printf("%s ", words[i][j].c_str());
        }
        printf("\n\n");
    }
}

bool Dictionary::search(string& searchWord){
    searchWord = cleanse(searchWord);
    int index = getWordPos(searchWord);
    int end = words[index].size() - 1;
    int start = 0;
    int middle;

    for (int i = 0; start <= end; i++){
        middle = (start + end) / 2;

        if (searchWord == words[index][middle]){
            return true;
        }
        else if (searchWord < words[index][middle]){
            end = middle - 1;
        }
        else{
            start = middle + 1;
        }
    }

    return false;
}

string Dictionary::cleanse(const string& word){
    string retString = "";
    int x;
    int count = 0;

    for (int i = 0; i < word.size(); i++){
        x = int(word[i]);

        if ((x > 96) and (x <123) or (x > 191) and (x < 256)){ //if it's a lowercase letter or utf8 character, add it to the return string.
            retString += char(x);
        }
        else if (x > 64 and x < 91){ //if it's an uppercase letter, convert to it's lowercase equivalent, and add it to the return string.
            x += 32;
            retString += char(x);
        }
    }

    return retString;
}