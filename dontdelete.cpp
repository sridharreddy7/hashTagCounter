#include <iostream>
#include <stdlib.h>

#include <fstream>
#include <sstream>

#include <math.h>

#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <string>





using namespace std;
// #include "fibonacci_heap.hpp"

// using boost::heap::fibonacci_heap;

// typedef fibonacci_heap<int, boost::heap::compare<std::greater<int> > > boost_heap;	

#define tr(container, it) for(typeof(container.begin()) it = container.begin(); it != container.end(); it++)
#define all(c) c.begin(), c.end() 
#define present(container, element) (container.find(element) != container.end()) 
#define cpresent(container, element) (find(all(container),element) != container.end()) 
#define sz(a) int((a).size()) 

stack <string> stk1;
stack <int> stk2;

struct Node {
	// Kept public to allow for O(1) access
	int data;       //The data in this node
	int rank;       // Number of children
	bool isMarked;  // Whether the node is marked 
	string hashTag; // Save the hashtag for searching the map.


	Node *l_sib; // The sibling pointers
	Node *r_sib;
	
	Node *parent;   // Parent of the node if any	
	Node *child;    // One of the children 	
		
	//Constructor
	 Node (int val, string hshTag) {
		data 		= val;
		rank 		= 0;
		isMarked 	= false;
		l_sib 		= this;
		r_sib 		= this;
		parent 	= NULL;
		child  	= NULL;
		hashTag = hshTag;
	}
};

class FibHeap {
	//pointer to the node storing min. O(1) lookup.
	private:
		Node * min_;
		unsigned int size_;
		// map<int, Node*> index_;
		map<string, Node*> index_;
				
	public:
	FibHeap() : min_ (NULL) , size_() {};
	
	//Insert a new node with the given value	
	void insertNode(int val, string hshTag){

		// int isNew = index_.count(hshTag);
		// cout<<"isNew--->"<<isNew<<endl;
		if(index_.find(hshTag) == index_.end()){

			// cout<<"inserted in node"<<endl;
			Node * newNode = new Node(val, hshTag);
			// cout<<"inserted node val "<<val<<endl;
			//if no nodes created yet
			if (!min_){
				min_ = newNode;
			}
			else {
				
				//Merge the new node with the old root list and update min
				min_ = mergeLists(min_, newNode);	
			}
			
			//insert it into the index
			index_[hshTag] = newNode;
			
			//Increment size of heap
			size_++;
		}
		else{
			// cout<<"decrease key started"<<endl;
			decreaseKey1(val, hshTag);
		}
	}

	//Merge two lists pointed to by the input parameters. Returns the smaller node
	Node * mergeLists(Node * a, Node * b){
		//When atleast one of the input lists is null
		if (!a && !b) return NULL;
		else if (!a)  return b;
		else if (!b)  return a;
			
		//General merge
		Node * tmp;
		tmp = a->r_sib;
		
		a->r_sib = b->r_sib;
		b->r_sib = tmp;
		
		a->r_sib->l_sib = a;
		b->r_sib->l_sib = b;
		
		return (a->data > b->data) ? a : b;
	}
	
	string getMin(){
		if (min_) {

			return min_->hashTag;
		}
		cout << "\nCannot get min from Empty heap ";
		return "NA";
		
	}
	
	int getMinCount(){
		if (min_) {

			return min_->data;
		}
		cout << "\nCannot get min from Empty heap ";
		return 0;
	}

	int getSize(){
		return size_;
	}	
	
	//Removes the node n from the doubly linked-list it is a part of.
	//Returns the right sibling of the node.
	Node * removeNode(Node * n){
		Node * ret = n->r_sib;
		
		//If its the last node
		if (ret == n){
			return NULL;
		}
		
		n->r_sib->l_sib = n->l_sib;
		n->l_sib->r_sib = n->r_sib;
		n->r_sib = n;
		n->l_sib = n;
		
		return ret;
	}
	
	//Deletes and returns the minimum value in the heap. 
	int deleteMin(){
		
		if (!min_) {
			cout << "\nCannot delete min from Empty heap \n";
			return INT_MIN;
			//throw invalid_argument("\n Cannot delete from empty heap");
		}
		
		Node * deletedNode = min_;
		
		//Remove from index
		index_.erase(min_->hashTag);
		
		//Decrement size of heap 
		size_--;
		
		//Get a handle to the remaining list;
		min_ = removeNode(min_);
		
		//Fix the parent pointers of the children, if any //TO TEST
		if (deletedNode->child){
			Node * curr = deletedNode->child;
			Node * start = curr;
			do{
				curr->parent = NULL;
				curr = curr->r_sib;
			} while(curr != start);
			
			//Merge children of deleted node with the root list
			min_ = mergeLists(deletedNode->child, min_);
		}
		//If we deleted the last node, the heap is empty
		if (!min_) return deletedNode->data;
		
		//Perform consolidation on the new root list
		consolidateList();
		
		return deletedNode->data;
	}
	
	//Performs consolidation of all the nodes in the root list pointed to by min_
	//Fixes min list with no root of the same rank
	void consolidateList(){
	
		// Start with the bound = log_2[size] possible ranks in the tree
		// Expanded if we encounter higher ranks
		vector<Node *> rank_to_nodes (int(log2(size_)) + 1);

		//If singleton tree
		if (min_->r_sib == min_) return;

		/* We need to ensure we don't go over the same node twice. 
		* Here we choose to go with a high memory overhead vs. a runtime overhead.
		* We go through and store all the nodes in the root list in a fixed array
		* and iterate over this array.
		*/ 
		
		//Setup list of nodes to visit.
		vector<Node *> nodes_to_visit;
		Node *curr = min_;
		Node *start = curr;
		do{
			nodes_to_visit.push_back(curr);
			curr = curr->r_sib;
		} while(curr != start);
		
		//Go over all the nodes in the nodes_to_visit array
		for (int i = 0; i < nodes_to_visit.size(); i++){
			curr = nodes_to_visit[i];
			
			//Ensure the rank list is big enough
			if (curr->rank >= rank_to_nodes.size()) rank_to_nodes.resize(curr->rank+1);

			// If it is the first node of its rank, just add it
			if (!rank_to_nodes[curr->rank]){
				rank_to_nodes[curr->rank] = curr;
				if (curr->data >= min_->data) min_ = curr;
			}
			// Otherwise we merge the two trees. 
			else {
				while (rank_to_nodes[curr->rank]){					
					//Assign the new curr after linking. 
					curr = link(rank_to_nodes[curr->rank], curr);
					//Empty the previous rank
					rank_to_nodes[curr->rank - 1] = NULL;
					//Update min, making sure its a root pointer.
					if(curr->data >= min_->data) min_ = curr;
					//If it needs to be resized.
					if (curr->rank >= rank_to_nodes.size()) rank_to_nodes.resize(curr->rank+1);
				}
				//Add the node to this new rank
				rank_to_nodes[curr->rank] = curr;
			}
		}
	}
	
	
	Node * link(Node *a, Node *b){
		
		if (a == b) cout << "\n Warning: linking node to itself \n";
		
		//Remove the larger node from the root list
		Node *min, *max;
		if (a->data > b->data){
			min = a;
			max = b;
		} else {
			min = b;
			max = a;
		}
		removeNode(max);
		
		//Make the larger node a child of the smaller 		
		min->child = mergeLists(min->child, max);
		max->parent = min;
		
		//Increase the rank of the smaller node
		min->rank++;
		
		return min;
	}
	void decreaseKey1(int new_count, string hsTag){
		Node *curr = index_[hsTag];

		curr->data = curr->data + new_count;

		index_.erase(hsTag);

		index_[hsTag] = curr;
		if (curr->parent){
			if (curr->parent->data < curr->data) 
				cutNode(curr);
		}
		
		//Make sure min is updated if necessary
		if (curr->data > min_->data) min_=curr;

	}
	
	// void decreaseKey(int old_key, int new_key){
	// 	//Find the node corresponding to the key
	// 	Node *curr = index_[old_key];
		
	// 	//Update key
	// 	curr->data = new_key;
		
	// 	//Update index
	// 	index_.erase(old_key);
	// 	index_[new_key] = curr;
		
	// 	//Check if heap order violated
	// 	if (curr->parent){
	// 		if (curr->parent->data > curr->data) 
	// 			cutNode(curr);
	// 	}
		
	// 	//Make sure min is updated if necessary
	// 	if (curr->data < min_->data) min_=curr;
		
	// }

	//Recursively cut and and mark nodes until we hit the root list
	void cutNode(Node *to_cut){
		to_cut->isMarked = false;
		
		//If we hit a root, stop the cuts
		if (!to_cut->parent) return;
		
		//If it was the node that its parent pointed to, change the parent's child pointer
		if (to_cut->parent->child == to_cut){
			if (to_cut->r_sib == to_cut) to_cut->parent->child = NULL;
			else to_cut->parent->child = to_cut->r_sib;
		}
		
		//Remove the node from its list
		removeNode(to_cut);
		
		//Merge it with the root list
		min_ = mergeLists(min_, to_cut);
		
		//Decrease parents degree
		to_cut->parent->rank--;
		
		//Recursive call
		if (!to_cut->parent->isMarked) to_cut->parent->isMarked = true;
		else cutNode(to_cut->parent);
		
		//Set parent to null since it is in root list
		to_cut->parent = NULL;
	}
	
	
	void testLink(){
		min_ = link(min_,min_->r_sib);
	}
	
	// Overloaded << operator for pretty printing the state of the heap
	friend ostream& operator<<(ostream &strm, const FibHeap &f) {
		
		//In case of an empty heap
		if (!f.min_) return strm << "\nNothing to print : Empty Heap "; 

		//Traverse the nodes and print their value
		ostringstream result;
		
		queue<const Node *> currentLevel;
		queue<const Node *> nextLevel;
		const Node *temp;
		const Node *cur_min;
		
		currentLevel.push(f.min_);

		//Print right
		result << "\nSize: "<< f.size_ << "\nRight siblings: ";
		
		while (!currentLevel.empty()){
			cur_min = currentLevel.front();
			currentLevel.pop();
			temp = cur_min;
			if (cur_min){
			do {
				result << temp->data; 
				if (temp->child) {
				result << "(" << temp->child->data << ")";
				nextLevel.push(temp->child);
				}
				result << " -> "	;
				temp = temp->r_sib;
			} while (temp != cur_min);
			result << temp->data;
			result << '\t';
			}
			if (currentLevel.empty()){
				swap(currentLevel, nextLevel);
				result << '\n';
			}
		}

		//Print left
		result << "Left siblings: ";
		temp = f.min_;
		do {
			result << temp->data << " -> "	;
			temp = temp->l_sib;
		} while (temp != f.min_);
		result << temp->data;
		result << '\n';

  		return strm << result.str();
	}
};


int main(int argc, char* argv[]) {
	
	FibHeap fib;
	// cout<<argv[1]<<endl;
	ifstream infile(argv[1]);
	ofstream outfile;
	string line;
	string hTag;
	int count;
	int topNTags;
	int type;
	
	outfile.open("output.txt", ios::app);
	
	
	while (std::getline(infile, line))
	{
	    std::istringstream iss(line);
	    iss>>hTag;
	    if(hTag[0]=='#'){

	    	iss>>count;
	    	type=1;
	    }
	    else if(hTag[0]!='#' && hTag!="STOP"){
	    	 stringstream convert(hTag);
	    	 convert>>topNTags;
	    	 type=2;
	    	
	    }
	    else if(hTag=="STOP"){
	    	break;
	    }
	    if(type==1){

	     	fib.insertNode(count, hTag);
	 	}
	 	else if(type==2){
	 		
	 		while(topNTags--){
	 		
	 			if(topNTags!=0){
	 				outfile<<fib.getMin().substr(1,fib.getMin().length())<<",";
	 			}
	 			else{
	 				outfile<<fib.getMin().substr(1,fib.getMin().length());
	 			}
	 		
	 			stk1.push(fib.getMin());
	 			stk2.push(fib.getMinCount());
	 			fib.deleteMin();
	 		}
	 		outfile<<endl;
	 		while(stk1.size()>0){
	 			fib.insertNode(stk2.top(), stk1.top());
	 			stk1.pop();
	 			stk2.pop();
	 		}
	 	}
	}
}