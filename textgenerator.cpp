/*
 * File: textGenerator.cpp
 * Name: Hiba Mirza
 * NetID: hmirz4
 * Course: CS 251 Data Structures (21814) 2024 Summer
 * Program Overview: This program generates random text based on a given file. It builds a map of word
 * sequences (N-grams) from the input file and used this map to generate text by predicting the next word
 * based on the current sequence of words. The user can build and print the map, generate text, or restart 
 * with a new file.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "myrandom.h"

using namespace std;

/*
 * reads words from the input file, creates prefixes of length N-1, and maps each prefix to
 * possible suffixes. The map is used for generating random text based on the n-gram model.
 * 
 * infile - input file stream from which to read words
 * N - the number of words in the n-gram
 * gramMap - the map to be populated with n-grams
 */
void buildMap(ifstream& infile, int N, map<vector<string>, vector<string>> &gramMap) {
    string word;
    vector<string> words;

    // reads all words from the file into the vector
    while (infile >> word) {
        words.push_back(word);
    }

    // creates n-grams for the middle of the file
    for (int i = 0; i <= words.size() - N; ++i) {
        vector<string> prefix(words.begin() + i, words.begin() + i + N - 1);
        string suffix = words[i + N - 1];
        gramMap[prefix].push_back(suffix);
    }

    // creates n-grams for the end, wrapping around to the beginning
    for (int i = words.size() - N + 1; i < words.size(); ++i) {
        vector<string> prefix(words.begin() + i, words.end());
        prefix.insert(prefix.end(), words.begin(), words.begin() + N - 1 - prefix.size());
        string suffix = words[(i + N - 1) % words.size()];
        gramMap[prefix].push_back(suffix);
    }
}

/*
 * iterates through the n-gram map and prints each prefix and its corresponding suffixes
 *
 * gramMap - the n-gram map to be printed
 */
void printMap(const map<vector<string>, vector<string>>& gramMap) {
    for (const auto& entry : gramMap) {
        cout << "{";
        const vector<string>& key = entry.first;
        for (int i = 0; i < key.size(); ++i) {
            if (i > 0) {
                cout << " ";
            }
            cout << key[i];
        }
        cout << "} -> {";
        const vector<string>& value = entry.second;
        for (int i = 0; i < value.size(); ++i) {
            if (i > 0) {
                cout << " ";
            }
            cout << value[i];
        }
        cout << "}" << endl;
    }
}

/*
 * generates random text based on the n-gram map.
 *
 * gramMap - the map containing the n-grams and their corresponding suffixes
 * N - the size of n-grams
 * totalWords - the total number of words to generate
 *
 * returns a string containing the generated text
 */
string generateText(const map<vector<string>, vector<string>>& gramMap, int N, int totalWords) {
    stringstream ss;
    vector<vector<string>> keys;

    // collects all n-gram keys
    for (const auto& pair : gramMap) {
        keys.push_back(pair.first);
    }

    // selects a random starting n-gram
    int randomIndex = randomInteger(0, keys.size() - 1);
    vector<string> currPrefix = keys[randomIndex];

    // ouputs the starting n-gram
    for (const auto& word : currPrefix) {
        ss << word << " ";
    }

    // generates the remaining words
    for (int i = N; i <= totalWords; ++i) {
        vector<string> suffixes = gramMap.at(currPrefix);
        string nextWord = suffixes[randomInteger(0, suffixes.size() -1)];
        ss << nextWord << " ";
        currPrefix.erase(currPrefix.begin());
        currPrefix.push_back(nextWord);
    }

    return ss.str();
}

/*
 * prompts the user for a valid input file and opens it
 * 
 * infile - input file stream to be opened
 *
 * returns the name of the valid input file
 */
string getFile(ifstream& infile) {
    string filename;
    cout << "Input file name?  ";
    do {
        cin >> filename;
        infile.open(filename);
        if (!infile.is_open()) {
            cout << "Invalid file, try again: ";
            infile.clear();
        }
    } while (!infile.is_open());
    return filename;
}

/*
 * prompts the user for a valid value of N
 * 
 * returns the valid value of N
 */
int getN() {
    // gets the value of N
    int N;
    cout << "Value of N?  ";
    cin >> N;
    while (N <= 1) {
        cout << "N must be > 1, try again: ";
        cin >> N;
    }
    return N;
}


/*
 * prompts the user for a valid total number of words to generate
 * 
 * N - the size of n-grams
 * 
 * returns the valid total number of words to generate
 */
int getTotalWords(int N) {
    int totalWords;
    // gets the total number of words to generate
    cout << "Total words you'd like to generate?  ";
    cin >> totalWords;
        while (totalWords < N) {
        cout << "Total words must be at least N, try again: ";
        cin >> totalWords;
    }
    return totalWords;
}

/*
 * handles users command selection for building the map, printing the map, generating text, and restarting
 *
 * infile - the input file stream
 * gramMap - the map to store the n-grams and their corresponding suffixes
 * N - the size of the n-grams
 * totalWords - the total number of words to generate
 */
void chooseCommands (ifstream& infile, map<vector<string>, vector<string>>& gramMap, int N, int totalWords, string filename) {
    char command;
    do {
        cout << "Type b-build map, p-print map, g-generate text, s-start over, x-to exit: ";
        cin >> command;
        switch (command) {
            case 'b':
                buildMap(infile, N, gramMap);
                cout << "...Building map: " << filename << "..." << endl;
                cout << endl;
                break;
            case 'p':
                printMap(gramMap);
                cout << endl;
                break;
            case 'g':
                cout << "..." << generateText(gramMap, N, totalWords) << "..." << endl;
                cout << endl;
                break;
            case 's':
                infile.close();
                gramMap.clear();
                break;
            case 'x':
                return;
        }
    } while (true);
}

int main() {
    cout << "Welcome to the Text Generator." << endl;
    cout << "This program makes random text based on a document." << endl;

    //input file stream
    ifstream infile;

    // gets the input file name
    string filename = getFile(infile);

    // gets the value of N
    int N = getN();

    // gets the total number of words
    int totalWords = getTotalWords(N);

    // map to store n-grams
    map<vector<string>, vector<string>> gramMap;

    // handles users command selection
    chooseCommands(infile, gramMap, N, totalWords, filename);

    return 0;
}