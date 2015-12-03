#include <iostream>
#include <cstdlib>
#include <string>
#include "trie.h"
#include <fstream>
#include <cctype>

using namespace std;

void addDownWord(int i, int j, vector<string>& words, string myBoard[15][15]){
	string word = "";
	if(i == 0){
		while(i < 15 && isalpha(myBoard[i][j].at(0))){
			word += myBoard[i][j];
			i++;
		}
		if(word.length() > 1){
			words.push_back(word);	
		}
		
	}
	else{
		if(!isalpha(myBoard[i - 1][j].at(0))){
			while(i < 15 && isalpha(myBoard[i][j].at(0))){
				word += myBoard[i][j];
				i++;
			}
			if(word.length() > 1){
				words.push_back(word);	
			}
		}
	}
}

void getWordsFromBoard(string& rackLetters, vector<string>& words, string myBoard[15][15]){
     ifstream boardFile("board.txt");
	 char my_character;
	 string word = "";
	 bool rackComplete = false;
	 int row = 0;
	 int column = 0;
	 while (!boardFile.eof()) {
	     boardFile.get(my_character);//get each character and store it in my_character
	     if(my_character == '\n' && !rackComplete){
	         rackComplete = true;
	         boardFile.get(my_character);
	     }
	      if(!rackComplete){
	         rackLetters += my_character;
	     }
	     else {//after recording the rack
	         if(my_character != '\n'){
	           //  cout << "row is: " << row << endl;
	           //  cout << "column is: " << column << endl;
	             myBoard[row][column] = my_character; // seg fault
	             column++;
	         }
	         else {
	             row++;
	             column = 0;
	         }
	     }
	 }
	 
	 boardFile.close();
	 
	 
	 word = "";
	 for(int i = 0; i < 15; i++){
	     for(int j = 0; j < 15; j++){
	         if (isalpha(myBoard[i][j].at(0))){//only add on if its a word horizontally
	               word += myBoard[i][j];
	               addDownWord(i, j, words, myBoard); // make this function
	         }
	         if((!isalpha(myBoard[i][j].at(0)) && word.length() > 1) ||
	            (j + 1 == 15 && word.length() > 1)){
	             words.push_back(word);
	             word = "";
	             addDownWord(i, j, words, myBoard); // make this function
	         }
	         else if(j + 1 == 15 && word.length() == 1){
	         	word = "";
	         	addDownWord(i, j, words, myBoard); // make this function
	         }
	         else if(!isalpha(myBoard[i][j].at(0))){
	         	word = "";
	         }
	     }
	 }
}



int main(){
    
    Trie root;
    string temp = "";
    ifstream infile("words.txt");
    string word;
    while (getline(infile, word)){//adding every word from the dictionary to the trie
        root.insert(&*word.begin(), word.length());
	}
	infile.close();
	
	string rackLetters = "";
	string myBoard[15][15];
	ifstream boardFile("board.txt");
	getline(boardFile, rackLetters);
    boardFile.close();
    
    
    //vector<string> words;
    
    //getWordsFromBoard(rackLetters, words, myBoard);
    // vector<string> results;
    // root.insert("andy", 4);
    // root.insert("nanny", 5);
    // root.insert("aa", 2);
    // root.insert("and", 3);
    // root.insert("nay", 3);
    // root.insert("anestri", 7);

    // map<char, int> rack;
    // rack['a'] = 1;
    // rack['n'] = 3;
    // rack['d'] = 1;
    // rack['y'] = 2;
    // root.getRackWords(temp, rack, results);
    // for(vector<string>::iterator it = results.begin(); it!= results.end(); it++){
    //     cout << *it << endl;
    // }
    
  
	 
	 //for(int i = 0; i < words.size(); i++){
	 //	cout << words.at(i) << endl;
	 //}
	 
	 //cout << rackLetters << endl;
	 
	//string temp = "";
	map<char, int> rack;
	map<string, WordInfo*> validWords;
	for(int i = 0; i < rackLetters.length(); i++){
		map<char, Trie*>::iterator result = rack.find(rackLetters.at(i));
		if(result != rack.end()){//if letter is already existing in the rack, just increment value by 1
			rack[rackLetters.at(i)] += 1;	
		}
		else {
			rack[rackLetters.at(i)] = 1;	
		}
	}
	
	root.findValidNewWords(rack, myBoard, validWords);
	//sort the validWords by score now
	//then create a print function that would iterate through the map which would finally hold the top 20 words,
	// the word, its row and column of the first letter, its orientation, its score
    
    root.getRackWords(temp, rack, results);
    for(vector<string>::iterator it = results.begin(); it!= results.end(); it++){
        cout << *it << endl;
    }
  

  return 0;
}
