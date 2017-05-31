#include "puzzle.h"
#include  <cmath>
#include  <assert.h>
#include  <stdio.h>
#include  <algorithm>

using namespace std;

//////////////////////////////////////////////////////////////
//constructor
//////////////////////////////////////////////////////////////
Puzzle::Puzzle(const Puzzle &p) : path(p.path){
	
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){	
		    board[i][j] = p.board[i][j];
		    goalBoard[i][j] = p.goalBoard[i][j];
		}
	}	
	
	x0 = p.x0;
	y0 = p.y0;
	//path = p.path;
	pathLength = p.pathLength;
	hCost = p.hCost;
	fCost = p.fCost;	
	strBoard = toString(); //uses the board contents to generate the string equivalent
	depth = p.depth;
	pathSet = p.pathSet;
}


//////////////////////////////////////////////////////////////
//constructor
//inputs:  initial state, goal state
//////////////////////////////////////////////////////////////
Puzzle::Puzzle(string const elements, string const goal){
	
	int n;
	
	n = 0;
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){
		    board[i][j] = elements[n] - '0';
		    if(board[i][j] == 0){
			    x0 = j;
			    y0 = i;
			 }
		    n++;
		}
	}
		
	///////////////////////
	n = 0;
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){
		    goalBoard[i][j] = goal[n] - '0';
		    n++;
		} 
	}		
	///////////////////////	
	path = "";
	pathLength=0;
	hCost = 0;
	fCost = 0;
	depth = 0;
	strBoard = toString();	
	//rachel added
	int value = atoi(strBoard.c_str());
	pathSet.push_back(value);
}


void Puzzle::setDepth(int d){
	depth = d;
}

int Puzzle::getDepth(){
	return depth;
}

void Puzzle::updateHCost(heuristicFunction hFunction){
	hCost = h(hFunction);
}

void Puzzle::updateFCost(heuristicFunction hFunction){
	updateHCost(hFunction);
	fCost = getGCost() + getHCost();
}

int Puzzle::getFCost(){
	//updateFCost(hFunction);
	return fCost;
}

int Puzzle::getHCost(){
	return hCost;
}

int Puzzle::getGCost(){
	return pathLength;
}

//Heuristic function implementation
int Puzzle::h(heuristicFunction hFunction){
	
	int sum=0;
	int h=0;
	int numOfMisplacedTiles=0;
	string goalState="123804765";

	switch(hFunction){
		case misplacedTiles:			      
				//place your implementation here	
		for (int i=0;i<9;i++){
			 
			if (strBoard[i]!=goalState[i]&&strBoard[i]!='0'){
			numOfMisplacedTiles++;
			}
		}
		
		h = numOfMisplacedTiles; 					
		break;
		         
		case manhattanDistance:
		        //place your implementation here
		int x=0,y=0;
		for(int i=0; i < 3; i++){
			for(int j=0; j < 3; j++){
				if(board[i][j]!=goalBoard[i][j]&&board[i][j]!=0){
					int temp=board[i][j];
					for(int x=0; x < 3; x++){
						for(int y=0; y< 3; y++){
							if(temp==goalBoard[x][y]){
								//sum=sum+abs((i-x)-(j-y));
								sum = sum + (abs(i-x) + abs(j-y));
								break;
							}
						}
					}
				}
			}
		}
			
        h = sum; 					
        break;         
		           
	};
	
	return h;
	
}


//converts board state into its string representation
string Puzzle::toString(){
  int n;
  string stringPath;
  
  n=0;
  for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){			    
		    stringPath.insert(stringPath.end(), board[i][j] + '0');
		    n++;
		} 
  }
  
  return stringPath;
}



bool Puzzle::goalMatch(){
	bool result=false;
	string goalState = "123804765";
	if(goalState==strBoard) {result = true;}
	return result;
}

bool Puzzle::canMoveLeft(){
   return (x0 > 0);
	
}

const string Puzzle::getPath(){
	return path;
}

bool Puzzle::canMoveRight(){

   return (x0 < 2);
	
}


bool Puzzle::canMoveUp(){

   return (y0 > 0);
	
}

bool Puzzle::canMoveDown(){

   return (y0 < 2);
	
}

///////////////////////////////////////////////
//these functions will be useful for Progressive Deepening Search 

bool Puzzle::canMoveLeft(int maxDepth){
  	//put your implementations here
  	return (x0 > 0) && (path.length()< maxDepth);
}
bool Puzzle::canMoveRight(int maxDepth){
   //put your implementations here
	return (x0 < 2) && (path.length() < maxDepth);
}


bool Puzzle::canMoveUp(int maxDepth){
   //put your implementations here	
	return (y0 > 0) && (path.length()< maxDepth);
}

bool Puzzle::canMoveDown(int maxDepth){
   //put your implementations here
	return (y0 < 2) && (path.length() < maxDepth);
}

///////////////////////////////////////////////
//new function convert string to int then add to vector
//to check local loop later
void Puzzle::addToPathSet(){
	value = atoi(strBoard.c_str());
	pathSet.push_back(value);
}

Puzzle *Puzzle::moveLeft(){
	
	Puzzle *p = new Puzzle(*this);
	
	
   if(x0 > 0){
		
		p->board[y0][x0] = p->board[y0][x0-1];
		p->board[y0][x0-1] = 0;
		
		p->x0--;
		
		p->path = path + "L";
		p->pathLength = pathLength + 1;  
		p->depth = depth + 1; 
		
		
	}
	p->strBoard = p->toString();

	return p;
	
}


Puzzle *Puzzle::moveRight(){
	
   Puzzle *p = new Puzzle(*this);
	
	
   if(x0 < 2){
		
		p->board[y0][x0] = p->board[y0][x0+1];
		p->board[y0][x0+1] = 0;
		
		p->x0++;
		
		p->path = path + "R";
		p->pathLength = pathLength + 1; 
     	
		p->depth = depth + 1;
		
	}
	
	p->strBoard = p->toString();
	
	return p;
	
}


Puzzle *Puzzle::moveUp(){
	
   Puzzle *p = new Puzzle(*this);
	
	
   if(y0 > 0){
		
		p->board[y0][x0] = p->board[y0-1][x0];
		p->board[y0-1][x0] = 0;
		
		p->y0--;
		
		p->path = path + "U";
		p->pathLength = pathLength + 1;  
	
		p->depth = depth + 1;
		
	}
	p->strBoard = p->toString();
	
	return p;
	
}

Puzzle *Puzzle::moveDown(){
	
   Puzzle *p = new Puzzle(*this);
	
	
   if(y0 < 2){
		
		p->board[y0][x0] = p->board[y0+1][x0];
		p->board[y0+1][x0] = 0;
		
		p->y0++;
		
		p->path = path + "D";
		p->pathLength = pathLength + 1;  
		
		p->depth = depth + 1;
		
	}
	p->strBoard = p->toString();	
	
	return p;
	
}

/////////////////////////////////////////////////////


void Puzzle::printBoard(){
	cout << "board: "<< endl;
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){	
		  cout << endl << "board[" << i << "][" << j << "] = " << board[i][j];
		}
	}
	cout << endl;
	
}

int Puzzle::getPathLength(){
	return pathLength;
}
