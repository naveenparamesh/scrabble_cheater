#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <fstream>
#include <sstream>
using namespace std;

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
    
    void getRackWords(const string& prefix, map<char, int>& rack, vector<string>& results){
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
    
    void insert(char* input, int length){
        if (length == 0){
            isEOW = true;
            return;
        }
        cout << input << endl;
        if (!hasChild(input[0])){
            Trie* newTrie = new Trie();
            children[input[0]] = newTrie;
        }

        (children[input[0]])->insert(input+1, length-1);
    }
    
    Trie(){
        isEOW = false;
    }
};

int main() {
    Trie root;
     string temp = "";
    // vector<string> results;
    string andy = "andy";
     root.insert(&*andy.begin(), 4);
    // root.insert("nanny", 5);
    // root.insert("aa", 2);
    // root.insert("and", 3);
    // root.insert("nay", 3);
    // root.insert("anestri", 7);
    root.printAll(temp);

    // map<char, int> rack;
    // rack['a'] = 1;
    // rack['n'] = 3;
    // rack['d'] = 1;
    // rack['y'] = 2;
    // root.getRackWords(temp, rack, results);
    // for(vector<string>::iterator it = results.begin(); it!= results.end(); it++){
    //     cout << *it << endl;
    // }
    
//      ifstream infile("words.txt");//wrds.txt");
// 	 string word;
//      while (getline(infile, word)){
//          root.insert(&*word.begin(), word.length());
// 	 }
// 	 infile.close();
	 
// 	string temp = "";
// 	map<char, int> rack;
// 	vector<string> results;
//     rack['r'] = 1;
//     rack['e'] = 1;
//     rack['t'] = 1;
//     rack['i'] = 1;
//     rack['n'] = 1;
//     rack['a'] = 1;
//     rack['s'] = 1;
//     root.getRackWords(temp, rack, results);
//     for(vector<string>::iterator it = results.begin(); it!= results.end(); it++){
//         cout << *it << endl;
//     }
	 
	 
    
    
    
    
    
}
