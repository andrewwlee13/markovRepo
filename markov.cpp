#include "markov.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int readWordsFromFile(string fileName , string words[], int maxWords){
    ifstream inputFile;
    inputFile.open(fileName);
    bool fileOpen = inputFile.is_open();
    if(fileOpen == false){
        return -1;
    }
    int i = 0;
    while (i < maxWords && inputFile >> words[i]){ // >> words[counter]
        i = i + 1;
    }
    int numWords = i;
    inputFile.close();
    return numWords;
}

// words[1000] = {"barack", "hussein", "obama", "used", "to", "be", "president", ...}

string joinWords(const string words[], int i, int order){ 
    string joinedWords = "";
    for(int k = 0; k < order; k++){
        joinedWords = joinedWords + words[i + k];
        if (k < order - 1) {
            joinedWords = joinedWords + " ";
        }
    }
    return joinedWords;
}

// words[1000] = {"barack", "hussein", "obama", "used", "to", "be", "president", ...}


// order = 1, startIndex 


int buildMarkovChain(const string words[], int numWords, int order, string prefixes[], string suffixes[], int maxChainSize){
    if(abs(order) > 3 || order == 0 || numWords <= order || maxChainSize <= 0){
        return 0;
    }

    int currentChainSize = 0;
    int i = 0;
    while( i < numWords - order && currentChainSize < maxChainSize){
        prefixes[i] = joinWords(words, i, order);
        suffixes[i] = words[i + order];
        currentChainSize = currentChainSize + 1;
        i = i + 1; 
    }
    int finalChainSize = currentChainSize;
    return finalChainSize;
}

// words[1000] = {"barack", "hussein", "obama", "used", "to", "be", "president", ...}

// i = 0, i has to be less than 7, the current chainsize is zero, current chainsize cant be bigger than the max, which is arbitrarily 1100

// traverse words[], i = 0 means take the first word "barack", i = 0 < 7, currentChainsize = 0 < 7
// prefixes[0] = joinWords(words, start at 0, order 1) means the first entry of prefixes[] is the first entry of words[] + nothing else
// suffixes[0] = words[0 + 1] = "hussein"
// now prefixes[] = {barack ,} and suffixes[] = {hussein ,
// while --> current chain size = 0 + 1 = 1 < 1100, i = i + 1 = 0 + 1 = 1 < 7 - 1
// prefixes[1] = words[1]
// suffixes[1] = words[1 + 1] = words[2]
// prefixes[maxChainSize] = {"barack","hussein",...}
// suffixes[maxChainSize] = {"hussein", "obama",...}, i++ --> i = 2, currentchainSize = 2
// i = 2 --> prefixes[] = {"barack","hussein","obama",...}
// suffixes[] = {"hussein", "obama","used"}


// now i = 1 < 7, currentChainSize = 1 < 7





// order = 2, prefixes[0] would be --> i = 0 < 7, words[0] + " " + words[1] 




string getRandomSuffix(const string prefixes[], const string suffixes[], int chainSize, string currentPrefix){
    
    int matchCount = 0;
    int r = 0; // entries in prefixes 

    while(r < chainSize){
        if(prefixes[r] == currentPrefix){
            matchCount = matchCount + 1;
        } r = r + 1;
    }

    if(matchCount == 0){
        return "";
    }

    int pick = rand() % matchCount;

    r = 0;
    int prefixCounter = 0;
    while(r < chainSize && prefixCounter < pick){
        if(prefixes[r] == currentPrefix){
            prefixCounter = prefixCounter + 1;
        } r = r + 1;
    }
    while(r < chainSize && prefixes[r] != currentPrefix){
        r = r + 1;
    }
    if(r >= chainSize){
        return "";
    } else {
        return suffixes[r];
    };
}

// getRandomPrefix
// just pick a random prefix to start off the next sentence after encountering a terminal suffix
// return type: string
// parameters: string prefixes[], int chainSize
// remember that chainSize is the number of entries in prefixes[]

string getRandomPrefix(const string prefixes[], int chainSize){
   
    if(chainSize > 0){
        int randomEntry = rand() % chainSize;
        return prefixes[randomEntry];
    };
    return "";
    
}

// generateText
// uses getRandomSuffix and getRandomPrefix to start generating text
// return type: string
// parameters: const prefixes[], const suffixes[], int chainSize, int order, int numWords

string generateText(const string prefixes[], const string suffixes[], int chainSize, int order, int numWords){
    if(chainSize <= 0 || order < 1 || order > 3 || numWords < order){
        return "";
    }

    string currentPrefix = getRandomPrefix(prefixes, chainSize);
    string result = currentPrefix;

    string currentWords[3];
    int wordIndex = 0;
    string temp = "";
    int i = 0;
    while(i < (int)currentPrefix.length()){
        if(currentPrefix[i] == ' '){
            currentWords[wordIndex] = temp;
            wordIndex = wordIndex + 1;
            temp = "";
        } else {
            temp = temp + currentPrefix[i];
        }
        i = i + 1;
    }
    currentWords[wordIndex] = temp;

    i = 0;
    while(i < numWords - order){
        string newWord = getRandomSuffix(prefixes, suffixes, chainSize, currentPrefix);
        if(newWord == ""){
            break;
        }
        result = result + " " + newWord;
        int j = 0;
        while(j < order - 1){
            currentWords[j] = currentWords[j + 1];
            j = j + 1;
        }
        currentWords[order - 1] = newWord;
        currentPrefix = joinWords(currentWords, 0, order);
        i = i + 1;
    }

    return result;
}

/*

int main(){

    // variables intialized and then assigned by user, maxWords <= 1000
    int maxWords = 0;
    int order = 0;
    cout << "what's the max number of words that should be read by the dummy generator?" << endl;
    cin >> maxWords;
    cout << "how many words of context should the dummy have for predicting each new word?" << endl;
    cin >> order;

    // array initialization
    string words[1000] = {};
    string prefixes[997] = {};
    string suffixes[997] = {};

    // variables to be used exclusively by functions and never seen by users
    int numWords = 0; // returned by readWordsFromFile, not neccesarrily equal to the maxWords entered by user
    int chainSize = 0; // returned by buildMarkovChain, not neccesarrily equal to the maxChainSize arbitrarily chosen as 1000
    int maxChainSize = 997;

   
    numWords = readWordsFromFile("obama.txt", words, maxWords);
    cout << "numWords is " << numWords << endl; 
    chainSize = buildMarkovChain(words, numWords, order, prefixes, suffixes, maxChainSize);
    cout << "chainSize is " << chainSize << endl;
    string currentPrefix = "Hussein";
    string matchingSuffix = getRandomSuffix(prefixes, suffixes, chainSize, currentPrefix);
    cout << "matching suffix is" << matchingSuffix << endl;
    cout << "the 4th entry of prefix is " << prefixes[3] << endl;

    return 0;
}





































// getRandomPrefix
// just pick a random prefix to start off the next sentence after encountering a terminal suffix
// return type: string
// parameters: string prefixes[], int chainSize
// remember that chainSize is the number of entries in prefixes[]

string getRandomPrefix(const string prefixes[], int chainSize);

// generateText
// uses getRandomSuffix and getRandomPrefix to start generating text
// return type: string
// parameters: const prefixes[], const suffixes[], int chainSize, int order, int numWords

string generateText(const string prefixes[], const string suffixes[], int chainSize, int order, int numWords);


*/
