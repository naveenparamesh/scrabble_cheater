#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <fstream>
#include <sstream>
using namespace std;



struct WordInfo {
    private:
        int row, column;
        bool across;
        int score;
    public:
        WordInfo(int theRow, int theCol, bool isAcross, int theScore){
            row = theRow;
            column = theCol;
            across  = isAcross;
            score = theScore;
        }
}

struct Trie {
    map<char, Trie*> children;
    bool isEOW;
    
    bool hasChild(char key){
        map<char, Trie*>::iterator result = children.find(key);
        return result != children.end();      
    }

    bool hasWord(char* input, int length){
        if (length == 0){
            return isEOW;
        }
        
        if (!hasChild(input[0])){
            return false;
        }
        
        return (children[input[0]])->hasWord(input+1, length-1);
    }
    
    void printAll(const string& prefix){
        if (isEOW){
            cout << prefix << endl;
        }
        
        for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
            it->second->printAll(prefix + it->first);
        }
    }
    
    void getAllWords(const string& prefix, vector<string>& results){
        if (isEOW){
            results.push_back(prefix);
        }
        
        for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
            it->second->getAllWords(prefix + it->first, results);
        }
    }
    
    void getRackWords(const string& prefix, map<char, int>& rack, char tileLetter, vector<string>& results){
        if (isEOW){
            results.push_back(prefix);
        }
        
        for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
            if (rack.find(it->first) != rack.end() && rack[it->first] > 0){
                    rack[it->first] -=1;
                    it->second->getRackWords(prefix + it->first, rack, results);
                    rack[it->first] +=1;
            }
        }
    }
    
    
    
    void findValidNewWords(map<char, int>& rack, string myBoard[15][15], map<string, WordInfo*>& validWords){
        // for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
            
        // }
        // Trie* trie = 0;
        // for(int i = 0; i < word.length(); i++){
        //     if(i == 0){
        //         trie = children[word.at(0)];
        //     }
        //     else {
        //         trie = trie->children[word.at(i)];    
        //     }
        // }
        for(int row = 0; row < 15; row++){
            for(int col = 0; col < 15; col++){
                 char tileLetter = myBoard[row][col];
                 if(isalpha(tileLetter)){//if its a tile, then add it to rack properly and call getRackWords
                    map<char, Trie*>::iterator result = rack.find(tileLetter);
                    if(result != rack.end()){//if letter is already existing in the rack, just increment value by 1
                    	rack[tileLetter] += 1;	//
                    }
                    else {
                    	rack[tileLetter] = 1;	
                    }
                    string temp = "";
                    vector<string> results;
                    getRackWords(temp, rack, tileLetter, results); // this function figures out every possible word that INCLUDES the tileLetter from the board and any of the rack letters
                    //validateAndSetInfo(results, validWords, row, col, myBoard);
                    // setWordsInfo(results, validWords); will be inside of validateAndSetWords function
                }
            }
        }
        
        
        
        // now trie is at the last letter spot, and we can now figure out if there are any words that can be made 
    }
    
    void insert(char* input, int length){
        if (length == 0){
            isEOW = true;
            return;
        }
        if (!hasChild(input[0])){
            Trie* newTrie = new Trie();
            children[input[0]] = newTrie;
        }

        (children[input[0]])->insert(input+1, length-1);
    }
    
    Trie(){
        isEOW = false;
    }
};// end of struct
    

