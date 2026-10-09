# ifndef MARKOV_H
# define MARKOV_H
# include <string>
using namespace std;

// joinWords
// glues multiple words into one token, for order higher than 1
// return type: string 
// parameters: string words[] , int startIndex, int count
// words[] is the array of words
// int startIndex and int count just say where to start and finish the joining process

string joinWords(const string words[], int startIndex, int count);


// readWordsFromFile
// puts words into words[] array
// return type: int
// parameters: string filename, string words[], int maxWords
// filename is the name of the file
// words[] is the array it takes words from and puts it into
// maxWords tells the fxn() the max number of worrds from the file to put into words[]

int readWordsFromFile(string fileName, string words[], int maxWords);


// buildMarkovChain
// uses sliding windows to add stuff into prefixes[] and suffixes[]
// return type: int
// parameters: const string words[], int numWords, int order, string prefixes[], string suffixes[], int maxChainSize
// int numWords is the size of words[]
// order is the context window basically, like how many words to join together into prefixes[]
// prefixes[] store all your prefixes
// suffixes[] store all your suffixes
// maxChainSize the max, not necessarily the actual, number of elements in prefixes[] and suffixes[]

int buildMarkovChain(const string words[], int numWords, int order, string prefixes[], string suffixes[], int maxChainSize);


// getRandomSuffix
// pull an entry from prefixes[] given an entry from suffixes[]
// return type: string
// parameters: const string prefixes[], const string suffixes[], int chainSize, string currentPrefix

string getRandomSuffix(const string prefixes[], const string suffixes[], int chainSize, string currenPrefix);


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


# endif 
