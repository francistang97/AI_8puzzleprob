#include <string>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;


enum heuristicFunction{misplacedTiles, manhattanDistance};



class Puzzle{
		
private:

    string path;
    int pathLength;
    int hCost;
    int fCost;
    int depth; 
        
    int goalBoard[3][3];
    
    int x0, y0; //coordinates of the blank or 0-tile
    
    int board[3][3];
    
public:
    
    string strBoard;
    vector<int> pathSet;
    int value;   

    Puzzle(const Puzzle &p); //Constructor
    Puzzle(string const elements, string const goal);
     
    void printBoard();
    
    int h(heuristicFunction hFunction);
     

    void updateFCost(heuristicFunction hFunction); 
    void updateHCost(heuristicFunction hFunction);
    void updateDepth(){
		 depth++;
	}		 	 	  
	 
    bool goalMatch();
	 string toString();
	
    string getString(){
		return strBoard;
	 }
    
    void addToPathSet();
    bool canMoveLeft();
    bool canMoveRight();
    bool canMoveUp();
    bool canMoveDown();  

//----------------------------------------
//these functions will be useful for Progressive Deepening Search    
	 bool canMoveLeft(int maxDepth);
	 bool canMoveDown(int maxDepth);
	 bool canMoveRight(int maxDepth);
	 bool canMoveUp(int maxDepth);	 
//----------------------------------------
	  
    Puzzle * moveLeft();
    Puzzle * moveRight();
    Puzzle * moveUp();
    Puzzle * moveDown();
	 
	 
    const string getPath();
    
    void setDepth(int d);
    int getDepth();
    
    int getPathLength();
    int getFCost();
	int getHCost();
	int getGCost();	

};
