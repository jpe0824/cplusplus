#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <random>
#include <fstream>
#include <iostream>
#include <set>
#include <cstring>

//test for 2,3,4 char
//test for sodiums, rehearings
//use maps (unordered), sets
//use set overall
//use map of strings and vectors

using namespace std;

int main(int argc, char *argv[]) {
    vector<string> words;
    const int NWORDS = 81483;
    //reading words from file to vector words
    {
        ifstream file("words.txt");
        string word;
        while(file >> word) {
            words.emplace_back(move(word));
            //words_set.insert(word);
        }
    }
    string word;
    //check for args from command line
    if (argc > 1) {
        // check for agr length
        size_t arg_size = strlen(argv[1]);
        if (arg_size >= 3) {
            string arg1(argv[1]);
            word = arg1;
        }
        else {
            default_random_engine dre(time(nullptr));   // Seed the engine
            uniform_int_distribution<int> di(0,NWORDS); // Use uniform distribution
            int n = di(dre);                            // Get a random int
            word = words.at(n);
        }
    }
    //if no arg choose ranodom word from file
    else {
        default_random_engine dre(time(nullptr));   // Seed the engine
        uniform_int_distribution<int> di(0,NWORDS); // Use uniform distribution
        int n = di(dre);                            // Get a random int
        word = words.at(n);
    }
    cout << "The word is: " << word << endl;
    const auto comparator=[] (const string& word1, const string& word2) {
        if (word1.length() < word2.length()) {
            return true;
        } else if (word1.length() > word2.length()) {
            return false;
        }
        return word1 < word2;
    };

    set<string, decltype(comparator)> subwords(comparator);
    sort(word.begin(),word.end());

    do {
        for (size_t i = 3; i <= word.length(); ++i) {
            string temp = word.substr(0,i);
            if (count(words.begin(),words.end(),temp)) {
                subwords.insert(temp);
            }
        }
    } while (next_permutation(word.begin(),word.end()));

    for (const string& subword : subwords) {
        cout << subword << endl;
    }

    return 0;
}