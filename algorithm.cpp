

#include "algorithm.h"
#include <string>
#include <queue>
#include <iostream>
#include <iomanip>
#include <unordered_map>

using namespace std;

struct Node {
  Puzzle* data;
  Node *next;
  //int AHeuristic;//add admissible heuristic value for aStar
};

class CompareHeuristics{//operator overloading () to be able to sort priority queue according to FCost
public:
	bool operator()( Puzzle *& p1,  Puzzle *& p2)  {
		if (p1->getFCost() > p2->getFCost()) return true;
		return false;
	}
};

class Stack {
private:                         //the data differs from the array
  Node *listpointer;
public:                          //the functions (methods) are identical
  Stack();
  ~Stack();
  void Push(Puzzle* newthing);
  void Pop();
  Puzzle* Top();
  bool isEmpty();
};

Stack::Stack() {
// constructor
  listpointer = nullptr;
}

Stack::~Stack() {
// destructor
}

void Stack::Push(Puzzle* newthing) {
// place the new thing on top of the stack
Node *temp;
  temp = new Node;             //same as add node to front of linked-list
  temp->data = newthing;
  temp->next = listpointer;    //NOTE: no overflow problem
  listpointer = temp;
}


void Stack::Pop() {
// remove top item from the stack
Node *p;
  p = listpointer;
  if (listpointer != nullptr) {     //        check to avoid underflow
	 listpointer = listpointer->next;
	 delete p;                //always delete a TEMPORARY variable
  }
}

Puzzle* Stack::Top() {
// return the value of the top item
  return listpointer->data;  //WARNING: what if listpointer is nullptr?
}


bool Stack::isEmpty() {
// returns true if the stack is empty
  if (listpointer == nullptr) {
	 return true;
  }
  return false;
}

class Queue {
private:
  Node *front, *rear;
 
public:
  Queue();
  ~Queue();
  void Join(Puzzle* newthing);
  void Leave();
  Puzzle* Front();
  bool isEmpty();
  //char FrontMove();
  void PrintQueue(); 
};


Queue::Queue() {
// constructor
  front = nullptr;  rear = nullptr;
}

Queue::~Queue() {
// destructor
}

void Queue::Join(Puzzle* newthing) {
// place the new thing at the rear of the queue
Node *temp;
  temp = new Node;
  temp->data = newthing;
  temp->next = nullptr;
  if (rear != nullptr) { rear->next = temp; }
  rear = temp;
  if (front == nullptr) { front = temp; }
}

void Queue::PrintQueue() {
	Node *temp;
	temp = new Node;
	temp = front;
	while (temp != nullptr){
		cout << temp->data->strBoard << " "<<temp->data->getPath()<< endl;
		temp = temp->next;
	}
	cout<< "*******************" <<endl;
	return;
}


void Queue::Leave() {
// remove the front item from the queue
Node * temp;
  if (front == nullptr) { return; }
  temp = front;
  front = front->next;
  if (front == nullptr) { rear = nullptr; }
  if (front != nullptr) {return;} 
  return ;
}


Puzzle* Queue::Front() {
// return the value of the front item
  if (front != nullptr) {return front->data;} 
  return 0;
}


bool Queue::isEmpty() {
// return true if the queue is empty
  if (front == nullptr) { return true; }
  return false;
}


int MaxQlength(int Q, int maxQ){
	if(Q>maxQ){maxQ=Q;}
	return maxQ;
	
}
///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Breadth-First Search 
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////
string breadthFirstSearch(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime){
	clock_t startTime;
	startTime = clock();
	
	Queue Enqueued;
	Puzzle *Dequeued;
	Puzzle* temp;
	string path;
	
	int QLength = 1;
	maxQLength = 1;
	numOfStateExpansions = 0;
	actualRunningTime = 0;
	
	Enqueued.Join(new Puzzle(initialState, goalState));
	numOfStateExpansions ++;
	
	while(!Enqueued.isEmpty()) {
		Dequeued = Enqueued.Front();
		if(Dequeued->goalMatch()){break;}
		
		Enqueued.Leave();
		QLength--;

		
		if(Dequeued->canMoveUp()) {
			temp = Dequeued->moveUp();
			Enqueued.Join(temp);
			QLength++;
			if(QLength>maxQLength) {maxQLength = QLength;}			
		}
		if(Dequeued->canMoveRight()) {
			temp = Dequeued->moveRight();
			Enqueued.Join(temp);
			QLength++;
			if(QLength>maxQLength) {maxQLength = QLength;}
		}
		if(Dequeued->canMoveDown()) {
			temp = Dequeued->moveDown();
			Enqueued.Join(temp);
			QLength++;
			if(QLength>maxQLength) {maxQLength = QLength;}
		}
		if(Dequeued->canMoveLeft()) {
			temp = Dequeued->moveLeft();
			Enqueued.Join(temp);
			QLength++;
			if(QLength>maxQLength) {maxQLength = QLength;}
		}
		numOfStateExpansions++;
		
	}
	path = Dequeued->getPath();

	
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	return path;		
		
}
///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Breadth-First Search with VisitedList
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////

string breadthFirstSearch_with_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime){
    
	clock_t startTime;
   startTime = clock();
	
	Queue Enqueued;
	Puzzle *Dequeued;
	Puzzle* temp;
	string path;
      
   bool isInVlist;
	vector<string> Vlist;
	
   int QLength = 1;
	maxQLength=1;
	numOfStateExpansions = 0;
   actualRunningTime = 0;
	
	Enqueued.Join(new Puzzle(initialState, goalState)); 
	Vlist.push_back(initialState);
	numOfStateExpansions++;
	
	while(!Enqueued.isEmpty()) {
		Dequeued = Enqueued.Front();
		if(Dequeued->goalMatch()){break;}
		
		Enqueued.Leave();
		QLength--;
		
		if(Dequeued->canMoveUp()) {
			temp = Dequeued->moveUp();
			isInVlist = false;
			for ( int i = 0; i < Vlist.size(); i++) {
				if (temp->strBoard.compare(Vlist[i]) == 0){
					isInVlist = true;
					break;
				}
			}
			if (!isInVlist) {
				Enqueued.Join(temp);
				QLength++;
				if(QLength>maxQLength) {maxQLength = QLength;}
				Vlist.push_back(temp->strBoard);
			}				
		}
		if(Dequeued->canMoveRight()) {
			temp = Dequeued->moveRight();
			isInVlist = false;
			for ( int i = 0; i < Vlist.size(); i++) {
				if (temp->strBoard.compare(Vlist[i]) == 0){
					isInVlist = true;
					break;
				}
			}
			if (!isInVlist) {
				Enqueued.Join(temp);
				QLength++;
				if(QLength>maxQLength) {maxQLength = QLength;}
				Vlist.push_back(temp->strBoard);
			}
		}
		if(Dequeued->canMoveDown()) {
			temp = Dequeued->moveDown();
			isInVlist = false;
			for ( int i = 0; i < Vlist.size(); i++) {
				if (temp->strBoard.compare(Vlist[i]) == 0){
					isInVlist = true;
					break;
				}
			}
			if (!isInVlist) {
				Enqueued.Join(temp);
				QLength++;
				if(QLength>maxQLength) {maxQLength = QLength;}
				Vlist.push_back(temp->strBoard);
			}
		}
		if(Dequeued->canMoveLeft()) {
			temp = Dequeued->moveLeft();
			isInVlist = false;
			for ( int i = 0; i < Vlist.size(); i++) {
				if (temp->strBoard.compare(Vlist[i]) == 0){
					isInVlist = true;
					break;
				}
			}
			if (!isInVlist) {
				Enqueued.Join(temp);
				QLength++;
				if(QLength>maxQLength) {maxQLength = QLength;}
				Vlist.push_back(temp->strBoard);
			}
		}
		numOfStateExpansions++;
		
	}
	path = Dequeued->getPath();

   

	
	
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	//path = "DDRRLLLUUU";  //this is just a dummy path for testing the function           
	return path;		
		
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  progressiveDeepeningSearch_No_VisitedList
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////
string progressiveDeepeningSearch_No_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, int ultimateMaxDepth){
	clock_t startTime;
	startTime = clock();
	Puzzle* Dequeued;
	Stack Enqueued;
	
   Puzzle* temp;
	string path;
	
	int maxDepth;
	int QLength;
	int i;
		int value;
	numOfStateExpansions = 0;
	bool isInLoop;
	vector<int>::iterator it;
	
	//each time we go through this loop, we go through one depth and then increment.  
	//A while loop is nested within it which loops through the expansions at a given depth.  
	//Each for loop increment starts with a clean slate (pop the queue, clear any visited lists), and repeats the previous search but goes one level deeper.
	
	
	maxQLength=1;
	for(maxDepth = 1; maxDepth<100; maxDepth++) { 
		Enqueued.Push(new Puzzle(initialState, goalState));
		QLength = 1;
		numOfStateExpansions++;
		
		while(!Enqueued.isEmpty()) {
			Dequeued = Enqueued.Top();
			if(Dequeued->goalMatch()){break;}	
			
			Enqueued.Pop();
			QLength--;			
			
			if(Dequeued->getPathLength()<maxDepth) {//here we check if Dequeued is already at max depth.  If so we don't expand.
				numOfStateExpansions++; //we are about to expand Dequeued
				if(Dequeued->canMoveLeft()) { //these nested if statements check if we can move Left.  If so, using a temp variable (so that listptr doesn't change) we expand in this direction and push the new puzzle on to Enqueued			
					temp = Dequeued->moveLeft();
					isInLoop = false;
					value = atoi(temp->strBoard.c_str());
					it = find(Dequeued->pathSet.begin(),Dequeued->pathSet.end(), value);
					if (it != Dequeued->pathSet.end()) {isInLoop = true;}
					if(!isInLoop) {
						temp->addToPathSet();
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
					}				
				} 
				if(Dequeued->canMoveDown()) {
					temp = Dequeued->moveDown();
					isInLoop = false;
					value = atoi(temp->strBoard.c_str());
					it = find(Dequeued->pathSet.begin(),Dequeued->pathSet.end(), value);
					if (it != Dequeued->pathSet.end()) {isInLoop = true;}
					if(!isInLoop) {
						temp->addToPathSet();
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
					}				
				} 
				if(Dequeued->canMoveRight()) {
					temp = Dequeued->moveRight();
					isInLoop = false;
					value = atoi(temp->strBoard.c_str());
					it = find(Dequeued->pathSet.begin(),Dequeued->pathSet.end(), value);
					if (it != Dequeued->pathSet.end()) {
						isInLoop = true;
							//cout <<"found one"<< endl;
					}
					if(!isInLoop) {
						temp->addToPathSet();
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
					}				
				} 
				if(Dequeued->canMoveUp()) {
					temp = Dequeued->moveUp();
					isInLoop = false;
					isInLoop = false;
					value = atoi(temp->strBoard.c_str());
					it = find(Dequeued->pathSet.begin(),Dequeued->pathSet.end(), value);
					if (it != Dequeued->pathSet.end()) {
						isInLoop = true;
							
					}
					if(!isInLoop) {
						temp->addToPathSet();
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
					}				
				} 
			}
			
		}
		
		if(Dequeued->goalMatch()){break;}
		//need to break out of two loops if the goal is found.  This is the second break.
				
		while(!Enqueued.isEmpty()) {Enqueued.Pop();}//clear the Enqueued list and start again with the new depth
				
	}
		
	path = Dequeued->getPath();
	ultimateMaxDepth = maxDepth;
		
			
//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	return path;		
		
}
	
///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  progressiveDeepeningSearch_with_VisitedList
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////
string progressiveDeepeningSearch_with_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, int ultimateMaxDepth){
   clock_t startTime;
   startTime = clock();	
	actualRunningTime = 0;
	
   Stack Enqueued;
   Puzzle *Dequeued;
	Puzzle* temp;
	//Puzzle* Previous;
	string path;
	
	int maxDepth;
	int QLength;
		int value;
	numOfStateExpansions = 0;
	
	vector<int> Vlist; //for the visited list
	vector<int>::iterator it;
	
	maxQLength=1;
	for(maxDepth = 1; maxDepth<1000; maxDepth++) {//as with PDS no VL, we start at 2 as there is one expansion before the while loop expansions
		Enqueued.Push(new Puzzle(initialState, goalState));
		value = atoi(initialState.c_str());
		Vlist.push_back(value);
		QLength = 1;
		numOfStateExpansions++;
		while(!Enqueued.isEmpty()) {
			Dequeued = Enqueued.Top();
			if(Dequeued->goalMatch()){break;}
			Enqueued.Pop();
			QLength--;
			
			if((Dequeued->getPathLength())<maxDepth) {
				numOfStateExpansions++; //we are now expanding
				if(Dequeued->canMoveLeft()) { //if it is in the visited list, skip it, if not, push it on enqueued and Visited List
					temp = Dequeued->moveLeft();
					value = atoi(temp->strBoard.c_str());
					it = find(Vlist.begin(),Vlist.end(), value);
					if (it == Vlist.end()) {
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
						Vlist.push_back(value);
					}				
				}
				if(Dequeued->canMoveDown()){
					temp = Dequeued->moveDown();
					value = atoi(temp->strBoard.c_str());
					it = find(Vlist.begin(),Vlist.end(), value);
					if (it == Vlist.end()) {
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
						Vlist.push_back(value);
					}				
				}
				if(Dequeued->canMoveRight()) {
					temp = Dequeued->moveRight();
					value = atoi(temp->strBoard.c_str());
					it = find(Vlist.begin(),Vlist.end(), value);
					if (it == Vlist.end()) {
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
						Vlist.push_back(value);
					}				
				}
				if(Dequeued->canMoveUp()) {
					temp = Dequeued->moveUp();
					value = atoi(temp->strBoard.c_str());
					it = find(Vlist.begin(),Vlist.end(), value);
					if (it == Vlist.end()) {
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
						Vlist.push_back(value);
					}				
				}
			}
		}
		if(Dequeued->goalMatch()){break;}
	//we need to break out of two loops if the goal is found.  This is the second break.
		while(!Enqueued.isEmpty()) {Enqueued.Pop();} //clear the Enqueued list and start again with the new depth
		Vlist.clear(); //clear the visited list and start again with the new depth
	}
		
	path = Dequeued->getPath();
	ultimateMaxDepth = maxDepth;
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	return path;		
	
}	
//***********************************************************************************************************

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  progressiveDeepeningSearch_with_NonStrict_VisitedList
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////
	
		

string progressiveDeepeningSearch_with_NonStrict_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, int ultimateMaxDepth){
	clock_t startTime;
   startTime = clock();	
	actualRunningTime = 0;
	
  Stack Enqueued;
   Puzzle *Dequeued;
	Puzzle* temp;
	string path;
	
	int maxDepth;
	int QLength;
		int value;
	numOfStateExpansions = 0;
	
	//use an unordered_map for the visited list.  Unordered map is for key-value searching (here we want to 
	//store both state and path length).  It uses a hidden hash table - yay!  Is quicker than map (I tried that too).
	unordered_map <int, int> Vlist;
	unordered_map<int, int>::iterator it;
	
	maxQLength=1;
	for(maxDepth = 1; maxDepth<100; maxDepth++) {
		Enqueued.Push(new Puzzle(initialState, goalState));
		
		//put initial state in the Visited List
		value = atoi(initialState.c_str());
		Vlist.insert({value, 0});
		QLength = 1;
		numOfStateExpansions++;
		
		while(!Enqueued.isEmpty()) {//expanding Dequeued
			Dequeued = Enqueued.Top();
			if(Dequeued->goalMatch()){break;}
			Enqueued.Pop();
			QLength--;
						
			if((Dequeued->getPathLength())<maxDepth) {
				numOfStateExpansions++; //we are now expanding
				if(Dequeued->canMoveLeft()) { 
					temp = Dequeued->moveLeft();
					value = atoi(temp->strBoard.c_str());
					it = (Vlist.find(value));  //iterator for searching visited list map
					if(it == Vlist.end()) { //if temp wasn't in there, push it on to the queue
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
						Vlist.insert({value, temp->getPathLength()});//and insert it into the Visited List
						} else {
						if(it->second > temp->getPathLength()) {	
							//if temp has the same state as a Puzzle already in the queue and if temp has a smaller path:
							Enqueued.Push(temp); //push it on to Enqueued
							QLength++;
							if(QLength>maxQLength) {maxQLength = QLength;}
							it->second = temp->getPathLength();//change path length in Vlist
						}
					}
				}
				
				if(Dequeued->canMoveDown()){
					temp = Dequeued->moveDown();
					value = atoi(temp->strBoard.c_str());
					it = (Vlist.find(value));  //iterator for searching visited list map
					if(it == Vlist.end()) { //if temp wasn't in there, push it on to the queue
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
						Vlist.insert({value, temp->getPathLength()});//and insert it into the Visited List
					} else {
						if(it->second > temp->getPathLength()) {	
							//if temp has the same state as a Puzzle already in the queue and if temp has a smaller path:
							Enqueued.Push(temp); //push it on to Enqueued
							QLength++;
							if(QLength>maxQLength) {maxQLength = QLength;}
							it->second = temp->getPathLength();//change path length in Vlist
						}
					}
				}
				if(Dequeued->canMoveRight()) {
					temp = Dequeued->moveRight();
					value = atoi(temp->strBoard.c_str());
					it = (Vlist.find(value));  //iterator for searching visited list map
					if(it == Vlist.end()) { //if temp wasn't in there, push it on to the queue
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
						Vlist.insert({value, temp->getPathLength()});//and insert it into the Visited List
					} else {
						if(it->second > temp->getPathLength()) {	
							//if temp has the same state as a Puzzle already in the queue and if temp has a smaller path:
							Enqueued.Push(temp); //push it on to Enqueued
							QLength++;
							if(QLength>maxQLength) {maxQLength = QLength;}
							it->second = temp->getPathLength();//change path length in Vlist
							
						}
					}
				}
				if(Dequeued->canMoveUp()) {
					temp = Dequeued->moveUp();
					value = atoi(temp->strBoard.c_str());
					it = (Vlist.find(value));  //iterator for searching visited list map
					if(it == Vlist.end()) { //if temp wasn't in there, push it on to the queue
						Enqueued.Push(temp);
						QLength++;
						if(QLength>maxQLength) {maxQLength = QLength;}
						Vlist.insert({value, temp->getPathLength()});//and insert it into the Visited List
					} else {
						if(it->second > temp->getPathLength()) {	
							//if temp has the same state as a Puzzle already in the queue and if temp has a smaller path:
							Enqueued.Push(temp); //push it on to Enqueued
							QLength++;
							if(QLength>maxQLength) {maxQLength = QLength;}
							it->second = temp->getPathLength();//change path length in Vlist
						}
					}
				}
			}
		}
		if(Dequeued->goalMatch()){break;} //we need to break out of two loops if the goal is found.  This is the second break.
		Vlist.clear();
		
		while(!Enqueued.isEmpty()) {Enqueued.Pop();} //clear the Enqueued list and start again with the new depth
		
	}
	path = Dequeued->getPath();
	ultimateMaxDepth = maxDepth;
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	return path;			
		
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  aStar_ExpandedList
//
// Move Generator:  
//
////////////////////////////////////////////////////////////////////////////////////////////

	

string aStar_ExpandedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, 
							   float &actualRunningTime, int &numOfDeletionsFromMiddleOfHeap, int &numOfLocalLoopsAvoided, int &numOfAttemptedNodeReExpansions, heuristicFunction heuristic){
											 
  
  	clock_t startTime;
   
    numOfDeletionsFromMiddleOfHeap=0;
    numOfLocalLoopsAvoided=0;
    numOfAttemptedNodeReExpansions=0;
	numOfStateExpansions = 0;
	maxQLength = 0;
	int QLength=0;
	actualRunningTime = 0.0;	
	startTime = clock();

	string path;
	bool isInSElist;//boolean for is in expended list
	vector<string> SExpandlist; //vector of string to store expanded list
	
	priority_queue<Puzzle*, vector<Puzzle*>, CompareHeuristics> aStarEnqueued;
	//priority queue as enqueued, so its always sorted
	//class CompareHeuristics uses operator overloading () to be able to sort 
	// priority queue according to FCost, lowest value will be the root
	
	Puzzle* aStartemp; // this a temp puzzle to store nextMove and then used to update Fcost and check expanded list							 
	aStartemp = new Puzzle(initialState, goalState);

	Puzzle* listptr; // this puzzle object is used as dequeued 
	listptr = new Puzzle(initialState, goalState);
	
	QLength++;//first element in enqueue is start state
	maxQLength=MaxQlength(QLength, maxQLength);

	numOfStateExpansions++;//start from 1 as start state is expanded at the beginning

	SExpandlist.push_back(listptr->strBoard);
	
	if(listptr->canMoveUp()) {
		aStartemp=listptr->moveUp();// store the state of next moveUp in temp
		aStartemp->updateFCost(heuristic);//update F cost with approriate heuristic
		aStarEnqueued.push(aStartemp);//push to priority queue 
		QLength++;
		maxQLength=MaxQlength(QLength, maxQLength);

	}
	if(listptr->canMoveRight()) {
		aStartemp=listptr->moveRight();
		aStartemp->updateFCost(heuristic);
		aStarEnqueued.push(aStartemp);
		QLength++;
		maxQLength=MaxQlength(QLength, maxQLength);

	}
	if(listptr->canMoveDown()) {
		aStartemp=listptr->moveDown();
		aStartemp->updateFCost(heuristic);
		aStarEnqueued.push(aStartemp);
		QLength++;
		maxQLength=MaxQlength(QLength, maxQLength);

	}
	if(listptr->canMoveLeft()) {
		aStartemp=listptr->moveLeft();
		aStartemp->updateFCost(heuristic);
		aStarEnqueued.push(aStartemp);
		QLength++;
		maxQLength=MaxQlength(QLength, maxQLength);

	}

	listptr = aStarEnqueued.top(); 
	// root of the priority queue is dequeued as its FCost is the lowest
	//the next loop will be expending this puzzle state
	aStarEnqueued.pop();//remove it from the queue as its expended
	SExpandlist.push_back(listptr->strBoard); //push it to the expended list vector as well;
	
	numOfStateExpansions++;
	QLength--;


	while(!aStarEnqueued.empty()) {	//looping as the enqueued (priority queue) is not empty
	
		if(goalState.compare(listptr->strBoard)==0){//goal match
			break; 
		}

		//the same for all four direction
		if(listptr->canMoveUp()) { 
		    aStartemp=listptr->moveUp();
			aStartemp->updateFCost(heuristic);

			isInSElist = false; //initialise the bool expended list as false
			for (int i = 0; i < SExpandlist.size(); i++) { //linear search through the expended list 
				if (aStartemp->strBoard.compare(SExpandlist[i]) == 0){ //compare, if found in the list
					isInSElist = true; 
					break; // end this move direction 
				}
			}
			if (!isInSElist){// if not in the list
				 aStarEnqueued.push(aStartemp); // temp pushed to the enqueued
				 QLength++;
				 maxQLength=MaxQlength(QLength, maxQLength);

			}
		}
		
		if(listptr->canMoveRight()) {
			aStartemp=listptr->moveRight();
			aStartemp->updateFCost(heuristic);
			isInSElist = false;

			for (int i = 0; i < SExpandlist.size(); i++) {
				if (aStartemp->strBoard.compare(SExpandlist[i]) == 0){
					isInSElist = true;
					break;
				}
			}
			if (!isInSElist) {
				aStarEnqueued.push(aStartemp);
				QLength++;
				maxQLength=MaxQlength(QLength, maxQLength);

			}
		}

		if(listptr->canMoveDown()) {
			aStartemp= listptr->moveDown();
			aStartemp->updateFCost(heuristic);
			isInSElist = false;
	
			for (int i = 0; i < SExpandlist.size(); i++) {
				if (aStartemp->strBoard.compare(SExpandlist[i]) == 0){
					isInSElist = true;
					break;
				}
			}			
			if (!isInSElist) {
				aStarEnqueued.push(aStartemp);	
				QLength++;
				maxQLength=MaxQlength(QLength, maxQLength);
				
			}			
		}
	
		if(listptr->canMoveLeft()) {
			  aStartemp = listptr->moveLeft();
			  aStartemp->updateFCost(heuristic);
			  isInSElist = false;
			for (int i = 0; i < SExpandlist.size(); i++) {
				if (aStartemp->strBoard.compare(SExpandlist[i]) == 0){
					isInSElist = true;
					break;
				}
			}
				
			if (!isInSElist) {
				aStarEnqueued.push(aStartemp);
				QLength++;
				maxQLength=MaxQlength(QLength, maxQLength);

			}				
		}
		
		//the following is the same as before

		listptr = aStarEnqueued.top(); //first get the root of the priority as dequeued
		aStarEnqueued.pop();//then romove it from the queue
		SExpandlist.push_back(listptr->strBoard);//finally add it to the expended list	

		QLength--;
		
		numOfStateExpansions++;		
	}
	path =listptr->getPath();//result we want

//***********************************************************************************************************
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	
	//path = "DDRRLLLUUU";  //this is just a dummy path for testing the function
				 
	return path;

}
	

//////////////////////////////////////////////////////////////////////////////////////////////////



