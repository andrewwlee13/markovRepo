#include "markov.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(){

    srand(time(0));

    // variables intialized and then assigned by user
    // order is 1 or 2 or 3, maxWords is how many words i want out, cant be smaller than order because the prefix is already that many words
    string fileName = "";
    int order = 0;
    int maxWords = 0;

    // array initialization
    // maxChainSize = 5000, arbitrarily, same as when i picked 1000 before. file can be longer, we just stop reading
    const int maxChainSize = 5000;
    string words[maxChainSize] = {};
    string prefixes[maxChainSize] = {};
    string suffixes[maxChainSize] = {};

    cout << "Enter input filename: ";
    cin >> fileName;

    bool orderOk = false;
    while(orderOk == false){
        cout << "Enter order (1, 2, or 3): ";
        bool gotOrder = false;
        if(cin >> order){
            gotOrder = true;
        }
        if(gotOrder == false){
            cout << "Order has to be a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        } else if(order < 1 || order > 3){
            cout << "Order has to be 1, 2, or 3." << endl;
        } else {
            orderOk = true;
        }
    }

    // i type abc, cin >> order fails, gotOrder stays false, ignore eats the rest of that line or the next cin just reads abc again
    // i type 0, gotOrder = true, but 0 < 1, orderOk stays false, ask again
    // i type 2, 2 is fine, orderOk = true, out of the loop

    bool countOk = false;
    while(countOk == false){
        cout << "Enter maximum number of words to generate: ";
        bool gotCount = false;
        if(cin >> maxWords){
            gotCount = true;
        }
        if(gotCount == false){
            cout << "The word count has to be a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        } else if(maxWords < order){
            cout << "Ask for at least " << order << " words. The starting prefix already uses " << order << "." << endl;
        } else {
            countOk = true;
        }
    }

    // order is 2, i type 1, 1 < 2, no, the prefix is already "barack hussein", thats 2 words, cant generate 1
    // i type 6, 6 >= 2, countOk = true

    // variables to be used exclusively by functions and never seen by users
    // numWords = whatever readWordsFromFile actually got, not neccesarrily the maxWords i typed
    // chainSize = however many pairs got stored, not neccesarrily maxChainSize
    int numWords = 0;
    int chainSize = 0;

    numWords = readWordsFromFile(fileName, words, maxChainSize);
    if(numWords == -1){
        cout << "Could not open " << fileName << "." << endl;
        return 1;
    }
    if(numWords <= order){
        cout << "Need at least " << order + 1 << " words in the file. Read " << numWords << "." << endl;
        return 1;
    }
    if(numWords == maxChainSize){
        cout << "At most " << maxChainSize << " input words were used. Additional words, if any, were ignored." << endl;
    }

    chainSize = buildMarkovChain(words, numWords, order, prefixes, suffixes, maxChainSize);
    if(chainSize <= 0){
        cout << "No prefix-suffix pairs were built." << endl;
        return 1;
    }

    // obama.txt = {"barack", "hussein", "obama", "used", "to", "be", "president"}, numWords = 7
    // order = 1, 7 <= 1? no. keep going
    // "hi there", order = 2, numWords = 2, 2 <= 2, yeah thats not enough, need 3 because the suffix needs one more word past the prefix
    // bad filename, readWordsFromFile gives -1
    // numWords == 5000 means the array is full, rest of the file is just gone

    // 7 words, order 1, i only goes to 5, chainSize = 6
    // prefixes[0] = "barack", suffixes[0] = "hussein"
    // prefixes[5] = "be", suffixes[5] = "president"
    // president is never on the left side, getRandomSuffix comes back "", generation dies there

    string output = generateText(prefixes, suffixes, chainSize, order, maxWords);

    // actual = how many words are in output. start at 1, "used" has no space in front of it
    int actual = 0;
    if(output != ""){
        actual = 1;
        int i = 0;
        while(i < (int)output.length()){
            if(output[i] == ' '){
                actual = actual + 1;
            } i = i + 1;
        }
    }

    // output = "used to be president"
    // actual = 1 ("used"), space --> 2 ("to"), space --> 3 ("be"), space --> 4 ("president")
    // maxWords was 15, 4 < 15, died early, yeah

    cout << endl;
    cout << output << endl;
    cout << "Generated " << actual << " of at most " << maxWords << " words.";

    // stopped short, so the last order words are the prefix that had nothing after it
    // order 1, "used to be president", spacesNeeded starts at 1, i starts at the end
    // walk left, first space is the one before president, spacesNeeded = 0, start is the p
    // failingPrefix = "president"
    // order 3, "The dog ran away. The cat sat down."
    // space before down. --> spacesNeeded = 2, space before sat --> 1, space before cat --> 0
    // failingPrefix = "cat sat down."
    if(actual < maxWords){
        string failingPrefix = "";
        int spacesNeeded = order;
        int start = 0;
        int i = (int)output.length() - 1;
        while(i >= 0 && spacesNeeded > 0){
            if(output[i] == ' '){
                spacesNeeded = spacesNeeded - 1;
                if(spacesNeeded == 0){
                    start = i + 1;
                }
            } i = i - 1;
        }
        i = start;
        while(i < (int)output.length()){
            failingPrefix = failingPrefix + output[i];
            i = i + 1;
        }
        cout << " Stopped early: no successor for \"" << failingPrefix << "\".";
    }
    cout << endl;

    return 0;
}
