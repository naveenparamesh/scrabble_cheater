#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
using namespace std;

// struct coord{
//     public:
//         int x, y;
//         coord(int resX, int resY){
//             x = resX;
//             y = resY;
//         }
// };

// struct coordValue{
//     int value;
//     bool isUsed;
//     coordValue(int theValue, bool used = false){
//         value = theValue;
//         isUsed = used;
//     }
// };

struct WordInfo {
    public:
        string word;
        int row, column;
        bool across;
        int score;
        
        WordInfo(string theWord, int theRow, int theCol, bool isAcross, int theScore){
            word = theWord;
            row = theRow;
            column = theCol;
            across  = isAcross;
            score = theScore;
        }
        // bool operator<(const WordInfo& wrdInfo1, const WordInfo& wrdInfo2){
        //         return wrdInfo1 < wrdInfo2;	
        // }
        
    };
    
     struct score_less {
            bool operator()(const WordInfo* left, const WordInfo* right) const {
                return (left->score < right->score);
            }
    };
        
        




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
        
        if (!hasChild(input[0])){//if it does not have that letter child, return false
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
    
    bool validDown(char myBoard[15][15], int row, int col, Trie* root, char letter){
        string verticalWord = ""; 
        int currentRow = row;
        if((currentRow - 1) >= 0 && !isalpha(myBoard[currentRow - 1][col]) &&
           (currentRow + 1) < 15 && !isalpha(myBoard[currentRow + 1][col])){
            return true;
        }
        while((currentRow - 1) >= 0 && isalpha(myBoard[currentRow - 1][col])){
            currentRow -= 1;
        }
        if(currentRow == row){
            verticalWord += letter;
            currentRow += 1;
        }
        while(currentRow < 15 && (isalpha(myBoard[currentRow][col]) || currentRow == row)){
            if(currentRow == row){//meaning if you got back to the original row which would have the letter your testing for, then add that
               verticalWord += letter; 
            }
            else {
                verticalWord += myBoard[currentRow][col];
            }
            currentRow += 1;
        }
        return root->hasWord(&*verticalWord.begin(), verticalWord.length()); //returns true if the word is valid, false otherwise
    }
    
    bool validAcross(char myBoard[15][15], int row, int col, Trie* root, char letter){
        string horizontalWord = ""; 
        int currentCol = col;
        if((currentCol - 1) >= 0 && !isalpha(myBoard[row][currentCol - 1]) &&
           (currentCol + 1) < 15 && !isalpha(myBoard[row][currentCol + 1])){
            return true;
        }
        while((currentCol - 1) >= 0 && isalpha(myBoard[row][currentCol - 1])){
            currentCol -= 1;
        }
        if(currentCol == col){
            horizontalWord += letter;
            currentCol += 1;
        }
        while(currentCol < 15 && (isalpha(myBoard[row][currentCol]) || currentCol == col)){
            if(currentCol == col){//meaning if you got back to the original row which would have the letter your testing for, then add that
               horizontalWord += letter; 
            }
            else {
                horizontalWord += myBoard[row][currentCol];
            }
            currentCol += 1;
        }
        return root->hasWord(&*horizontalWord.begin(), horizontalWord.length()); //returns true if the word is valid, false otherwise
    }
    
  
    
    //check for what tile its on
    //check the letter value
    //if we used all 7 letters its a bingo bonus (50 pts)
    //check for all other new words formed and add those scores on too
     // 9 = triple word score -> sum_so_far *3 = total
     // 4 = double word score
     // 2 = double letter score -> score += letterValue * 2  
     // 3 = triple letter score -> score += letterValue * 3
     // assault -> 14
     // ass -> 3
     // as -> 2
     // ask -> 7
     // asks -> 8
    int computeScore(string word, char myBoard[15][15], int row, int col, bool across, map<char, int>& rack, map<char, int>& letterValues){
        int score = 0;
        //bool bingoBonus = false;
        int letterCount = 0;
        //cout << "word length is: " << word.length() <<endl;
        // if(word == "foa*ing"){
        //     for(map<char, int>::iterator it = rack.begin(); it != rack.end(); it++){
        //     cout << it->first << " has " << rack[it->first] << " occurences"<<endl;
        //  }
        // }
        
     
             if(across){
                for(int i = 0; i < word.length(); i++){
                    if(myBoard[row][col + i] == '2'){//if its a digit, meaning bonus tile then add that value on to the score so far
                        score += letterValues[word.at(i)] * 2;
                    }
                    else if(myBoard[row][col + i] == '3'){
                        score += letterValues[word.at(i)] * 3;
                    }
                    else {
                        score += letterValues[word.at(i)];
                    }
                }
                
                for(int i = 0; i < word.length(); i++){
                    if(myBoard[row][col + i] == '9' && word.at(i) != '*'){//if u run into a bonus tile, multiply word by that amount only if that bonus tile was not where the wild card was
                        score *= 3;
                    }
                    else if(myBoard[row][col + i] == '4' && word.at(i) != '*'){
                        score *= 2;
                    }
                }
                
                //now figuring out all new words created from intersections and adding their scores on
               for(int i = 0; i < word.length(); i++){
                   int score_intersection = 0;
                   if(!isalpha(myBoard[row][col + i])){//if an empty tile, that is where we test if a new vertical word was made there
                       int tempRow = row;
                       if((tempRow - 1) >= 0 && !isalpha(myBoard[tempRow - 1][col + i]) &&
                           (tempRow + 1) < 15 && !isalpha(myBoard[tempRow + 1][col + i])){//if nothing above or below, just go to next letter
                            continue;//just go to the next letter in the word
                        }
                       while((tempRow - 1) >= 0 && isalpha(myBoard[tempRow - 1][col + i])){ // figures out the anchor
                           tempRow -= 1;
                       }
                       bool doubleWord = false;
                       bool tripleWord = false;
                       while(tempRow < 15 && (isalpha(myBoard[tempRow][col + i]) || tempRow == row)){
                           if(tempRow == row){//if anchor is letter its on, then add that letters value and move onto next row
                                // double letter tile
                                if(myBoard[tempRow][col + i] == '2'){
                                    score_intersection += letterValues[word.at(i)] * 2;
                                }
                                // triple letter tile
                                else if(myBoard[tempRow][col + i] == '3'){
                                     score_intersection += letterValues[word.at(i)] * 3;
                                }
                                // triple word tile
                                else if(myBoard[tempRow][col + i] == '9' && word.at(i) != '*'){
                                    score_intersection += letterValues[word.at(i)];
                                    doubleWord = true;
                                }
                                // double word tile
                                else if(myBoard[tempRow][col + i] == '4' && word.at(i) != '*'){
                                    score_intersection += letterValues[word.at(i)];
                                    tripleWord = true;
                                }
                                else { // no bonuses
                                    score_intersection += letterValues[word.at(i)];    
                                }
                                
                                tempRow += 1;
                        }
                        else {
                                score_intersection += letterValues[myBoard[tempRow][col + i]];
                                tempRow += 1;
                            }
                       }
                       
                       if(doubleWord){
                           score_intersection *= 2;
                       }
                       else if(tripleWord){
                           score_intersection *= 3;
                       }
                       score += score_intersection;
                   }
               }
            }
            else {// calculating if play is down
                
                // computing with double and triple letter tiles
                for(int i = 0; i < word.length(); i++){
                    if(myBoard[row + i][col] == '2'){//if its a digit, meaning bonus tile then add that value on to the score so far
                        score += letterValues[word.at(i)] * 2;
                    }
                    else if(myBoard[row + i][col] == '3'){
                        score += letterValues[word.at(i)] * 3;
                    }
                    else {
                        //cout << "letterValues" << letterValues[word.at(i)] << endl;
                        //cout << "letter: " << word.at(i) << " -> " << letterValues[word.at(i)]<< endl;
                        score += letterValues[word.at(i)];
                    }
                }
               
                
                // computing with double and triple word tiles
                for(int i = 0; i < word.length(); i++){
                    if(myBoard[row + i][col] == '9' && word.at(i) != '*'){
                        score *= 3;
                    }
                    else if(myBoard[row + i][col] == '4' && word.at(i) != '*'){
                        score *= 2;
                    }
                }
                
                
                
                 // INTERSECTIONS
                 //now figuring out all new words created from intersections and adding their scores on
                for(int i = 0; i < word.length(); i++){
                   int score_intersection = 0;
                   if(!isalpha(myBoard[row + i][col])){//if an empty tile, that is where we test if a new vertical word was made there
                       int tempCol = col;
                       if((tempCol - 1) >= 0 && !isalpha(myBoard[row + i][tempCol - 1]) &&
                           (tempCol + 1) < 15 && !isalpha(myBoard[row + i][tempCol + 1])){//if nothing above or below, just go to next letter
                           
                            continue;//just go to the next letter in the word
                        }
                       while((tempCol - 1) >= 0 && isalpha(myBoard[row + i][tempCol - 1])){ // figures out the anchor
                           tempCol -= 1;
                       }
                       bool doubleWord = false;
                       bool tripleWord = false;
                       while(tempCol < 15 && (isalpha(myBoard[row + i][tempCol]) || tempCol == col)){
                           if(tempCol == col){//if anchor is letter its on, then add that letters value and move onto next row
                                if(myBoard[row + i][tempCol] == '2'){
                                    //cout << "score so far for word " << word << " is: " << score << endl;
                                    score_intersection += letterValues[word.at(i)] * 2;
                                     
                                }
                                else if(myBoard[row + i][tempCol] == '3'){
                                    // cout << "score so far for word " << word << " is: " << score << endl;
                                     score_intersection += letterValues[word.at(i)] * 3;
                                }
                                else if(myBoard[row + i][tempCol] == '9' && word.at(i) != '*'){
                                    // cout << "score so far for word " << word << " is: " << score << endl;
                                    score_intersection += letterValues[word.at(i)];
                                    doubleWord = true;
                                }
                                else if(myBoard[row + i][tempCol] == '4' && word.at(i) != '*'){
                                    // cout << "score so far for word " << word << " is: " << score << endl;
                                    
                                    score_intersection += letterValues[word.at(i)];
                                    tripleWord = true;
                                }
                                else {
                                    // cout << "score so far for word " << word << " is: " << score << endl;
                                    score_intersection += letterValues[word.at(i)];    
                                   
                                }
                                // if(word == "egos" && col == 9){
                                //     cout << "top value would be: " << score_intersection << endl;
                                // }
                                
                                tempCol += 1;
                        }
                        else {
                                // cout << "score so far for word " << word << " is: " << score << endl;
                                
                                score_intersection += letterValues[myBoard[row + i][tempCol]];
                                tempCol += 1;
                            }
                       }
                       
                      
                       
                       // computes intersection double and triple word score:
                       if(doubleWord){
                            // cout << "score so far for word " << word << " is: " << score << endl;
                           score_intersection *= 2;
                       }
                       else if(tripleWord){
                            // cout << "score so far for word " << word << " is: " << score << endl;
                           score_intersection *= 3;
                       }
                        //cout << "score before addition of score and score_intersection "<< score_intersection << endl;
                        // cout << "score so far for word " << word << " is: " << score << endl;
                       score += score_intersection;
                   }
               }
            }
            
            for(int i = 0; i < word.length(); i++){
            // if(rack.find('f') != rack.end() && rack['f')] > 0){
            //         cout << "found it!" << endl;
            //     }
            if(rack.find(word.at(i)) != rack.end() && rack[word.at(i)] == 0){
                //cout << "gets to line 182" << endl;
                //rack[word.at(i)] -= 1;
                letterCount++;
            }
          }
        
        // for(map<char, int>::iterator it = rack.begin(); it != rack.end(); it++){
        //     if(it->second != 0){//then u know every letter wasn't used from the rack, thus no bingo
        //         bingoBonus = false;
        //     }
        // }
            
            
        if(letterCount == 7){//if all 7 rack letters are used then add a bingo bonus
        cout << "bingo for: " << word << endl;
            score += 50;
        }
       
        
       
        // cout << "score so far for word " << word << " is: " << score << endl;
        return score;

    }
    
    // find all words that are playable from board with the rack letters
    void getRackWords(const string& prefix, map<char, int>& rack, int row, int col, char myBoard[15][15], priority_queue<WordInfo*, vector<WordInfo*>, score_less >& validWords, map<char, int>& letterValues, Trie* root, int offset, bool across, vector<int>& wildcardIndices){
        if(across){
            if (isEOW){//insert into priority queue, if end of word is reached and its not a word already on the board
                bool validWord = false;
                for(int i = 0; i < prefix.length(); i++){
                    if(isalpha(myBoard[row][col + i])){
                        validWord = true;
                    }
                }
                if(validWord){
                    
                    int tempCol = col;
                    string temp = "";
                    while((tempCol - 1) >= 0 && isalpha(myBoard[row][tempCol - 1])){
                        tempCol -= 1;
                    }
                    while(tempCol < col){
                        temp += myBoard[row][tempCol];
                        tempCol += 1;
                    }
                    temp += prefix;
                    tempCol += prefix.length();
                    while(tempCol < 15 && isalpha(myBoard[row][tempCol])){
                        temp += myBoard[row][tempCol];
                        tempCol += 1;
                    }
                    
                   //cout << "temp is: " << temp << endl;
    
                    if(root->hasWord(&*temp.begin(), temp.length())){
                        //cout << prefix << " is valid across " << "at: "<< row << ", " << col << endl; 
                        string word = "";
                        for(int i = 0; i < prefix.length(); i++){
                            if(find(wildcardIndices.begin(), wildcardIndices.end(), i) != wildcardIndices.end()){
                                word += '*';
                            }
                            else {
                                word += prefix.at(i);
                            }
                        }
                      int score = computeScore(word, myBoard, row, col, true, rack, letterValues);
                      validWords.push(new WordInfo(prefix, row, col, across, score));
                        
                    }
                }
                
                
               
                //cout << "added word: " << prefix << endl;
             
            }
            if((col + offset) >= 15){
             
                return;
            }
            if(!isalpha(myBoard[row][col + offset])){//if its an empty tile, then check for a letter from the rack and 
                 for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
                     //cout << "letter: " << it->first << endl;
                    if (rack.find(it->first) != rack.end() && rack[it->first] > 0 && validDown(myBoard, row, col + offset, root, it->first)){
                            
                            //cout << "letter at offset " << offset << " is: " << it->first << endl;
                            offset += 1;
                            rack[it->first] -=1;
                            it->second->getRackWords(prefix + it->first, rack, row, col, myBoard, validWords, letterValues, root, offset, across, wildcardIndices);
                            offset -= 1;
                            rack[it->first] +=1;
                    }
                    // use wildcard if there are no more valid words found
                    else if(rack.find('*') != rack.end()  && rack['*'] > 0 && validDown(myBoard, row, col + offset, root, it->first)){
                            offset += 1;
                            rack['*'] -= 1;
                            it->second->getRackWords(prefix + it->first, rack, row, col, myBoard, validWords, letterValues, root, offset, across, wildcardIndices);
                            offset -= 1;
                            rack['*'] +=1;
                            wildcardIndices.pop_back();
                    }
                }
            }
            else {//it was a tile with a letter in it, thus directly access whatever is in the tile and use it in children
                char letter = myBoard[row][col + offset];
                //cout << "bottom letter at offset " << offset << " is: " << letter << endl; 
                if(hasChild(letter)){
                    offset += 1;
                    children[letter]->getRackWords(prefix + letter, rack, row, col, myBoard, validWords, letterValues, root, offset, across, wildcardIndices);
                }
                
                
            }
           
        }
        else {
            if (isEOW){//insert into priority queue
                bool validWord = false;
                for(int i = 0; i < prefix.length(); i++){
                    if(isalpha(myBoard[row + i][col])){
                        validWord = true;
                    }
                }
                if(validWord){
                    
                  int tempRow = row;
                    string temp = "";
                    while((tempRow - 1) >= 0 && isalpha(myBoard[tempRow][col])){
                        tempRow -= 1;
                    }
                    while(tempRow < row){
                        temp += myBoard[tempRow][col];
                        tempRow += 1;
                    }
                    temp += prefix;
                    tempRow += prefix.length();
                    while(tempRow < 15 && isalpha(myBoard[tempRow][col])){
                        temp += myBoard[tempRow][col];
                        tempRow += 1;
                    }
                    //cout << "temp is: " << temp << endl;
                    if(root->hasWord(&*temp.begin(), temp.length())){
                      //  cout << prefix << " is valid down" << "at: "<< row << ", " << col << endl; 
                     
                            string word = "";
                            for(int i = 0; i < prefix.length(); i++){
                                if(find(wildcardIndices.begin(), wildcardIndices.end(), i) != wildcardIndices.end()){
                                    word += '*';
                                }
                                else {
                                    word += prefix.at(i);
                                }
                            }
                             int score = computeScore(word, myBoard, row, col, false, rack, letterValues);
                             validWords.push(new WordInfo(prefix, row, col, across, score));
                        
                       
                    }
                
                }
               
                //cout << "word added is: " << prefix << endl;
                
            }
            if((row + offset) >= 15){
                return;
            }
            if(!isalpha(myBoard[row + offset][col])){//if its an empty tile, then check for a letter from the rack and 
            //cout << "occurrs" << endl;
                 for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
                     //cout << "letter is: " << it->first << endl;
                     //cout << "validity: " << validAcross(myBoard, row, col, it->first) << endl;
                     //cout << "words so far is: " << prefix << endl;
                    if (rack.find(it->first) != rack.end() && rack[it->first] > 0 && validAcross(myBoard, row + offset, col, root, it->first)){
                        //cout << "happened at top for " << it->first << endl;
                        
                            offset += 1;
                            rack[it->first] -=1;
                            it->second->getRackWords(prefix + it->first, rack, row, col, myBoard, validWords, letterValues, root, offset, across, wildcardIndices);
                            offset -= 1;
                            rack[it->first] +=1;
                    }
                    // use wildcard if there are no more valid words found
                    else if(rack.find('*') != rack.end()  && rack['*'] > 0 && validAcross(myBoard, row, col + offset, root, it->first)){
                            wildcardIndices.push_back(offset);
                            offset += 1;
                            rack['*'] -= 1;
                            it->second->getRackWords(prefix + it->first, rack, row, col, myBoard, validWords, letterValues, root, offset, across, wildcardIndices);
                            offset -= 1;
                            rack['*'] +=1;
                            wildcardIndices.pop_back();
                    }
                }
            }
            else {//it was a tile with a letter in it, thus directly access whatever is in the tile and use it in children
                char letter = myBoard[row + offset][col];
                
                if(hasChild(letter)){
                   //cout << "happened at bottom for " << letter << endl;
                   offset += 1;
                    children[letter]->getRackWords(prefix + letter, rack, row, col, myBoard, validWords, letterValues, root, offset, across, wildcardIndices);
                }
               
            }
        }
        
    }
    
    void getFirstMoves(const string& prefix, map<char, int>& rack, char myBoard[15][15], int row, int col, priority_queue<WordInfo*, vector<WordInfo*>, score_less >& validWords, map<char, int>& letterValues, vector<int>& wildcardIndices, int offset){
        if (isEOW){
            for(int i = 0; i < prefix.length(); i++){
                if(col + i == 7){
                    string word = "";
                    for(int i = 0; i < prefix.length(); i++){
                        if(find(wildcardIndices.begin(), wildcardIndices.end(), i) != wildcardIndices.end()){
                            word += '*';
                        }
                        else {
                            word += prefix.at(i);
                        }
                    }
                    
                        int score = computeScore(word, myBoard, row, col, false, rack, letterValues);
                        validWords.push(new WordInfo(prefix, row, col, true, score));
                    
                   
                }
            }
        }
        
        for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
            if (rack.find(it->first) != rack.end() && rack[it->first] > 0){
                    offset += 1;
                    rack[it->first] -=1;
                    it->second->getFirstMoves(prefix + it->first, rack, myBoard, row, col, validWords, letterValues, wildcardIndices, offset);
                    rack[it->first] +=1;
                    offset -= 1;
            }
            else if(rack.find('*') != rack.end()  && rack['*'] > 0){
                    wildcardIndices.push_back(offset);
                    offset += 1;
                    rack['*'] -= 1;
                    it->second->getFirstMoves(prefix + it->first, rack, myBoard, row, col, validWords, letterValues, wildcardIndices, offset);
                    rack['*'] +=1;
                    offset -= 1;
                    wildcardIndices.pop_back();
            }
        }
    }
    
    // starting at (row,col), to find playable locations: (locations w/ letters on board)
    bool isPlayable(int row, int col, char myBoard[15][15], bool across){
        if(across){
             if(isalpha(myBoard[row][col])){//if current position is a letter -> false
                    return false;// this should be false because you can't start playing a word in a spot that is already occupied
                }
            for(int i = 1; i <= 7; i++){
                if((col + i) < 15 && isalpha(myBoard[row][col + i])){
                    return true; // place to start a word-> letter on board
                }
                if((col + i) >= 15){
                    return false;
                }
            }
            return false; // no letters found
        }
        else {//check down
                if(isalpha(myBoard[row][col])){//if current position is a letter -> true
                    return false;
                }
            for(int i = 1; i <= 7; i++){
                if((row + i) < 15 && isalpha(myBoard[row + i][col])){
                    return true; // place to start a word-> letter on board
                }
                if((row + i) >= 15){
                    return false;
                }
            }
            return false; 
        }
    }
    
    void findEmptyValidWords(map<char, int>& rack, char myBoard[15][15], priority_queue<WordInfo*, vector<WordInfo*>, score_less >& validWords, map<char, int>& letterValues, vector<int>& wildcardIndices){
        string temp = "";
        for(int col = 0; col < 7; col++){
            getFirstMoves(temp, rack, myBoard, 7, col, validWords, letterValues, wildcardIndices, 0);
        }
    }
    
    // gets every single valid word that can be played on the board 
    void findValidWords(map<char, int>& rack, char myBoard[15][15], priority_queue<WordInfo*, vector<WordInfo*>, score_less >& validWords, map<char, int>& letterValues, vector<int>& wildcardIndices){
        string temp = "";
        for(int row = 0; row < 15; row++){
            for(int col = 0; col < 15; col++){
                bool playable_across = isPlayable(row, col, myBoard, true);
                bool playable_down = isPlayable(row, col, myBoard, false);
                if(playable_across){
                    getRackWords(temp, rack, row, col, myBoard, validWords, letterValues, this, 0, true, wildcardIndices);
                }
                if(playable_down){
                    getRackWords(temp, rack, row, col, myBoard, validWords, letterValues, this, 0, false, wildcardIndices);    
                }
                
            }
        }
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
    

