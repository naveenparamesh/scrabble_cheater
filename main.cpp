#include <iostream>
#include <cstdlib>
#include <string>
#include "trie.h"
#include <fstream>
#include <cctype>
#include <queue>
#include <algorithm>

using namespace std;

// void addDownWord(int i, int j, vector<string>& words, char myBoard[15][15]){
// 	string word = "";
// 	if(i == 0){
// 		while(i < 15 && isalpha(myBoard[i][j])){
// 			word += myBoard[i][j];
// 			i++;
// 		}
// 		if(word.length() > 1){
// 			words.push_back(word);	
// 		}
		
// 	}
// 	else{
// 		if(!isalpha(myBoard[i - 1][j])){
// 			while(i < 15 && isalpha(myBoard[i][j])){
// 				word += myBoard[i][j];
// 				i++;
// 			}
// 			if(word.length() > 1){
// 				words.push_back(word);	
// 			}
// 		}
// 	}
// }

void setBoard(char myBoard[15][15], bool& isBoardEmpty){
     ifstream boardFile("board.txt");
	 char my_character;
	 //string word = "";
	 bool rackComplete = false;
	 int row = 0;
	 int column = 0;
	 while (!boardFile.eof()) {
	     //if(my_character == '\n' && !rackComplete){
	     //    rackComplete = true;
	     //    boardFile.get(my_character);
	     //}
	     // only gets the first line of the board: the rack
	      if(!rackComplete){
	      	 string temp = "";
	         getline(boardFile, temp);
	         rackComplete = true;
	     }
	     else {//after recording the rack
	     	 boardFile.get(my_character);//get each character and store it in my_character
	         if(my_character != '\n'){
	           //  cout << "row is: " << row << endl;
	           //  cout << "column is: " << column << endl;
	           	 if(isalpha(my_character)){
	           	 	isBoardEmpty = false;
	           	 }
	             myBoard[row][column] = tolower(my_character); 
	             column++;
	         }
	         else {
	             row++;
	             column = 0;
	         }
	     }
	 }
	 
	 boardFile.close();
	 
	 
	
}

// void getWordsFromBoard(string rackLetters, vector<string>& words, char myBoard[15][15]){
// 	 string word = "";
// 	 for(int i = 0; i < 15; i++){
// 	     for(int j = 0; j < 15; j++){
// 	         if (isalpha(myBoard[i][j])){//only add on if its a word horizontally
// 	               word += myBoard[i][j];
// 	               addDownWord(i, j, words, myBoard); // make this function
// 	         }
// 	         if((!isalpha(myBoard[i][j]) && word.length() > 1) ||
// 	            (j + 1 == 15 && word.length() > 1)){
// 	             words.push_back(word);
// 	             word = "";
// 	             addDownWord(i, j, words, myBoard); // make this function
// 	         }
// 	         else if(j + 1 == 15 && word.length() == 1){
// 	         	word = "";
// 	         	addDownWord(i, j, words, myBoard); // make this function
// 	         }
// 	         else if(!isalpha(myBoard[i][j])){
// 	         	word = "";
// 	         }
// 	     }
// 	 }
// }





int main(){
    
    Trie root;
    string temp = "";
    bool isBoardEmpty = true;
    ifstream infile("words.txt");
    string word;
    while (getline(infile, word)){//adding every word from the dictionary to the trie
    	transform(word.begin(), word.end(), word.begin(), ::tolower);
        root.insert(&*word.begin(), word.length());
	}
	infile.close();
	
	string rackLetters = "";
	char myBoard[15][15];
	ifstream boardFile("board.txt");
	getline(boardFile, rackLetters);
	transform(rackLetters.begin(), rackLetters.end(), rackLetters.begin(), ::tolower);
	setBoard(myBoard, isBoardEmpty);
    
    
 
    
    
    
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
    
  
	 
	 
	 //cout << rackLetters << endl;
	 
	//string temp = "";
	//rackLetters = "niiaroe";
	cout << "rackLetters is: " << rackLetters << endl; 
	map<char, int> rack;
	map<char, int> letterValues;
	vector<int> wildCardIndexes;
	priority_queue<WordInfo*, vector<WordInfo*>, score_less > validWords;
	//priority_queue<WordInfo*> validWords;
	for(int i = 0; i < rackLetters.length(); i++){
		map<char, int>::iterator result = rack.find(rackLetters.at(i));
		if(result != rack.end()){//if letter is already existing in the rack, just increment value by 1
			rack[rackLetters.at(i)] += 1;	
		}
		else {
			rack[rackLetters.at(i)] = 1;	
		}
	}
	
	
	ifstream letterFile("weights.txt");
	char letter;
	int value;
	while(letterFile >> letter >> value){
		
		letterValues[tolower(letter)] = value;
	}
	
	
	
	
	
	
	
	
	 //cout << root.children['x']->children['y']->children['z']->computeScore("hello", myBoard, 0, 0) << endl;
	if(isBoardEmpty){
		root.findEmptyValidWords(rack, myBoard, validWords, letterValues, wildCardIndexes);
	}
	else {
		root.findValidWords(rack, myBoard, validWords, letterValues, wildCardIndexes);
	}
	
	 //string str = "";
	 //cout << "ban is valid: " << root.hasWord(&*str.begin(), str.length()) << endl;
	 
	 //root.getRackWords(str, rack, 11, 9, myBoard, validWords, letterValues, &root, 0, 0, true);
	 
	//map<char, Trie*> children = root.children['a']->children['s']->children['w']->children;
	
	// cout << "letters under w are: " << endl;
	// for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
	// 	cout << it->first << endl;
	// }
	
	
	 int numValidWords = 0;
	 while(!validWords.empty() && numValidWords < 20){
	 	if(validWords.top()->across){
	 		cout << validWords.top()->word << " to be played across: " << "at (" << validWords.top()->row << ", "<< validWords.top()->column << ") with score: " << validWords.top()->score << endl;
	 	}
	 	else{
	 		cout << validWords.top()->word << " to be played down: " << "at (" << validWords.top()->row << ", "<< validWords.top()->column << ") with score: " << validWords.top()->score << endl;
	 	}
       //cout << validWords.top()->word << " to be played across[T(1)/F(0)] " << validWords.top()->across << ": at (" << validWords.top()->row << ", "<< validWords.top()->column << ") with score: " << validWords.top()->score << endl;
       validWords.pop();
       numValidWords += 1;
   	 }
   	 
  
   	 
   	 //cout << validWords.size() << endl;
   
	// sort the validWords by score now
	// then create a print function that would iterate through the map which would finally hold the top 20 words,
	// the word, its row and column of the first letter, its orientation, its score
    
    // root.getRackWords(temp, rack, results);
    // for(vector<string>::iterator it = results.begin(); it!= results.end(); it++){
    //     cout << *it << endl;
    // }
  

  return 0;
}
