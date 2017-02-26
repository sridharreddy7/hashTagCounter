//header files added...
#include <stdio.h>
#include <vector> 
#include <queue>
#include <map> 
#include <set>
#include <utility> 
#include <algorithm>
#include <sstream> 
#include <iostream> 
#include <sstream>
#include <fstream>
#include <iomanip> 
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <cassert>
#include <fstream>
#include <limits>
#include <climits>
#include <stack>
#include <stdexcept>

using namespace std;


//macros defined here...
typedef long long ll;
typedef vector <int> vi;
typedef pair< int ,int > pii;
typedef istringstream iss;
typedef ostringstream oss;
#define pb push_back
#define mp make_pair
#define ff first
#define ss second
// #define sz size()
#define sz(a) int((a).size()) 
#define ln length()
#define cpresent(ctr, element) (find(all(ctr),element) != ctr.end()) 
#define tr(ctr, it) for(typeof(ctr.begin()) it = ctr.begin(); it != ctr.end(); it++)
#define rep(i,n) for(int i=0;i<n;i++)
#define fu(i,a,n) for(int i=a;i<=n;i++)
#define fd(i,n,a) for(int i=n;i>=a;i--)
#define all(a)  a.begin(),a.end() 
 
//modify these

#define gi(n) scanf("%d",&n)
#define gl(n) cin >> n
#define pi(n) printf("%d",n)
#define pl(n) cout << n
#define ps printf(" ")
#define pn printf("\n")
#define dg(n,s); printf("%s %d",s,n)
#define imax numeric_limits<int>::max()
#define imin numeric_limits<int>::min()
#define lmax numeric_limits<ll>::max()
#define lmin numeric_limits<ll>::min()
#define present(ctr, element) (ctr.find(element) != ctr.end()) 


//stacks required for storing removeMax values...
//stack for hashtag...
stack <string> hashStringStack;
//stack for the corresponding hash tag count...
stack <int> hashCountStack;


//Fibonacci node structure.
struct FibonacciNode{
	//parent node...
	FibonacciNode *nodeParent;
	//right sibling of the node...
	FibonacciNode *rightSibling;
	//hash tag count value...
	int count;
	//degree of the node...
	int degree;
	//is child cut value marked?...
	bool isMarked;
	//storing hashtag value...
	string hashTag;
	//left sibling of the node...
	FibonacciNode *leftSibling;
	//child node...
	FibonacciNode *nodeChild;
	//constructor for a new node...
	//which takes hastag and the corresponding count values...
	FibonacciNode(int hashCount, string hshTag){
		nodeParent = NULL;
		//circular linked list right sibling points to same node..
		rightSibling = this;

		count = hashCount;
		//every newnode degree will be 0...
		degree =0;
		//default ismark is set to false...
		isMarked = false;
		
		hashTag = hshTag;
		// circular linked list left sibling points to same node...
		leftSibling = this;
		nodeChild = NULL;
		
	}
};
//main class starts here..
class FibonacciClass {
	private:
		//pointer to the max node...
		FibonacciNode * maxPtr;
		//total number of nodes in the heap...
		 int noOfNodes;
		 //hashtable to store hashtag and the corresponding pointer to the heap...
		map<string, FibonacciNode*> hashTable;
				
	public:
	//default constructor to handle corner cases properly...
	FibonacciClass() : maxPtr (NULL) , noOfNodes() {};

	void createNode(int hshCount, string hshTag){
		//check whether the tag is already there in hashtable or not...

		if(hashTable.find(hshTag) == hashTable.end()){
			//if not found create a new node and insert in to hashtable and heap...
			FibonacciNode *newNode = new FibonacciNode(hshCount, hshTag);
			//if max pointer is not present then make the new node as max pointer...
			if(!maxPtr){
				maxPtr = newNode;
			}
			else{
				//else find the max pointer and merge with the existing heap...
				//redundant remove later....
				if(!maxPtr && !newNode){
					maxPtr= NULL;
				}
				else if(!maxPtr){
					maxPtr= newNode;
				}
				//combining new node with heap...
				FibonacciNode *swapNode;
				swapNode = maxPtr->rightSibling;
				maxPtr->rightSibling = newNode->rightSibling;
				newNode->rightSibling = swapNode;
				maxPtr->rightSibling->leftSibling = maxPtr;
				newNode->rightSibling->leftSibling = newNode;
				maxPtr= (maxPtr->count > newNode->count)? maxPtr: newNode;
			}
			//insert the hash tag in the hash table...
			hashTable[hshTag]= newNode;
			//increment the number of nodes in the heap...
			noOfNodes++;
		}
		else{
			//if hash tag is already there increase the count of that tag and if required modify the heap max pointer...
			increaseKey(hshCount, hshTag);
		}
	}
	
	string getMax(){
		//if max pointer is present then return the correponding hash tag...
		if (maxPtr) {

			return maxPtr->hashTag;
		}
		//else return not found... and exit
		cout << "\n Empty heap";
		return "NOT FOUND";
		
	}
	
	int getMaxCount(){
		//if max pointer is present then return the corresponding hash tag count...
		if (maxPtr) {
			return maxPtr->count;
		}
		//else return 0... and exit
		cout << "\nEmpty heap";
		return 0;
	}

	FibonacciNode * deleteNode(FibonacciNode * node){
		//deleting the node from the heap.. in O(1)...
			FibonacciNode * tempNode = node->rightSibling;
			//check whether only one node in the sibling list
			//if yes, then return NULL...
			if (tempNode == node){
				return NULL;
			}
			//make the change to remove the node and return the pointer of the original node's right sibling...
			node->leftSibling->rightSibling = node->rightSibling;
			node->rightSibling->leftSibling = node->leftSibling;
			node->leftSibling = node;
			node->rightSibling = node;
			return tempNode;
			//return and exit...
	}	


	int removeMax(){
		//delete min operation...
		//check whether max pointer is present or not...
		if(!maxPtr){
			//if no max pointer then return 0 ... and exit..
			cout<<"E456mpty heap"<<endl;
			return 0;
		}
		//save the max pointer in delNode..
		FibonacciNode *delNode = maxPtr;
		//decrease the number of nodes and remove the hash tag from the hash table..
		noOfNodes--;
		hashTable.erase(maxPtr->hashTag);
		//get the node's right sibling after deleting max pointer...
		maxPtr = deleteNode(maxPtr);

		if(delNode -> nodeChild){
			//merge remaining with heap...
			FibonacciNode *tnode  = delNode->nodeChild;
			FibonacciNode *start = tnode;

			tnode->nodeParent = NULL;
			tnode = tnode->rightSibling;
			while(tnode != start){
				tnode->nodeParent = NULL;
				tnode = tnode->rightSibling;
			}
			if(!delNode->nodeChild && !maxPtr){
				maxPtr= NULL;
			}
			else if(!maxPtr){
				maxPtr= delNode->nodeChild;
			}
			FibonacciNode *swapNode;
			swapNode = delNode->nodeChild->rightSibling;
			delNode->nodeChild->rightSibling = maxPtr->rightSibling;
			maxPtr->rightSibling = swapNode;
			delNode->nodeChild->rightSibling->leftSibling = delNode->nodeChild;
			maxPtr->rightSibling->leftSibling = maxPtr;
			
			maxPtr= (delNode->nodeChild->count > maxPtr->count)? delNode->nodeChild: maxPtr;
		}
		if (!maxPtr)
			 return delNode->count;
		//if max pointer is true then do pair wise combine...
		pairWiseCombine();		
		//now return the hash tag count...
		//returning only for the testing purpose...
		return delNode->count;
	}
	
	void pairWiseCombine(){
		//started pair wise combine...
		if (maxPtr->rightSibling == maxPtr) {
			return;
		}
		vector<FibonacciNode *> toCheck;
		FibonacciNode *curr = maxPtr;
		FibonacciNode *start = curr;
		
		toCheck.push_back(curr);
		curr = curr->rightSibling;
		
		while(curr != start){
			toCheck.push_back(curr);
			curr = curr->rightSibling;
		}
		//calculate the closet number to hold the values...
		vector<FibonacciNode *> degrees (int(log2(noOfNodes)) + 1);
		for (int i = 0; i < toCheck.size(); i++){
			curr = toCheck[i];
			if (curr->degree >= degrees.size()){
				degrees.resize(curr->degree+1);	
			} 
			if (degrees[curr->degree]){
				while (degrees[curr->degree]){					
					
					//linking the nodes and updating the degrees vector...
					FibonacciNode *min, *max;
					if (degrees[curr->degree]->count > curr->count){
						min = degrees[curr->degree];
						max = curr;
					} else {
						min = curr;
						max = degrees[curr->degree];
					}
					FibonacciNode * ret = max->rightSibling;
					max->rightSibling->leftSibling = max->leftSibling;
					max->leftSibling->rightSibling = max->rightSibling;
					max->rightSibling = max;
					max->leftSibling = max;

					if(!min->nodeChild && !max){
						min->nodeChild= NULL;
					}
					else if(!min->nodeChild){
						min->nodeChild= max;
					}

					//find and update the max pointer...
					FibonacciNode *swapNode;

					swapNode = min->nodeChild->rightSibling;
					min->nodeChild->rightSibling = max->rightSibling;
					max->rightSibling = swapNode;

					min->nodeChild->rightSibling->leftSibling = min->nodeChild;
					max->rightSibling->leftSibling = max;

					min->nodeChild= (min->nodeChild->count > max->count)? min->nodeChild: max;
					max->nodeParent = min;
					min->degree++;
					
					curr= min;
					degrees[curr->degree - 1] = NULL;
					if(curr->count >= maxPtr->count){
						 maxPtr = curr;
					}
					if (curr->degree >= degrees.size()){
						degrees.resize(curr->degree+1);	
					} 
				}
				
				degrees[curr->degree] = curr;


 
			}
			else {
				degrees[curr->degree] = curr;
				if (curr->count >= maxPtr->count){
					maxPtr = curr;	
				}
			}
		}
	}

	void increaseKey(int new_count, string hsTag){
		//get the pointer of the hashtag from the hashtable...
		FibonacciNode *curr = hashTable[hsTag];
		
		//increase the hashtag count value
		curr->count = curr->count + new_count;
		//remove the hash tag from the hashtable...
		hashTable.erase(hsTag);
		//insert the hashtag with updated count...
		hashTable[hsTag] = curr;
		//check whether it violates max heap property...
		if (curr->nodeParent && (curr->nodeParent->count < curr->count)){
				childCut(curr);
		}
		//check whether we need to update the max pointer...
		if (curr->count > maxPtr->count){
			maxPtr=curr;	
		} 
	}
	void childCut(FibonacciNode *node){

		if (!node->nodeParent) {
			return;
		}
		node->isMarked =false;
		//merging the child cut node with the heap again..
		FibonacciNode * ret = node->rightSibling;
		
		FibonacciNode *swapNode;

		if (node->nodeParent->nodeChild ==node){
			if (node->rightSibling !=node){
				node->nodeParent->nodeChild =node->rightSibling;	
			} 
			else{
				node->nodeParent->nodeChild =NULL;	
				
			} 
		}
		node->rightSibling->leftSibling = node->leftSibling;
		node->leftSibling->rightSibling = node->rightSibling;
		node->rightSibling = node;
		node->leftSibling = node;

		if(!maxPtr && !node){
			maxPtr= NULL;
		}
		else if(!maxPtr){
			maxPtr= node;
		}


		swapNode = maxPtr->rightSibling;
		maxPtr->rightSibling = node->rightSibling;
		node->rightSibling = swapNode;

		maxPtr->rightSibling->leftSibling = maxPtr;
		node->rightSibling->leftSibling = node;

		maxPtr=(maxPtr->count > node->count)? maxPtr: node;

		//decrease the parent degree...
		node->nodeParent->degree--;
		//check whether the parent is already false...
		if (node->nodeParent->isMarked) {
			//otherwise call childcut for the parent node too...
			childCut(node->nodeParent);	
		}
		else{
			//else mark the parent node as true...
			node->nodeParent->isMarked=true;
		} 
		node->nodeParent=NULL;
	}

};


int main(int argc, char* argv[]) {
	//initialize the fibonacci heap class object...
	FibonacciClass fibObj;
	// cout<<argv[1]<<endl;
	//use argv[1] for getting the file name...
	ifstream infile(argv[1]);
	ofstream outfile;
	string line;
	string hTag;
	int count;
	int topNTags;
	int type;
	//create if not exists or open the open the output_file.txt file to write the output...
	outfile.open("output_file.txt", ios::app);
	
	//start the while loop until we receive STOP or stop...
	while (std::getline(infile, line))
	{
	    std::istringstream iss(line);
	    iss>>hTag;
	    if(hTag[0]=='#'){
	    	//type1 for insert or increase key...
	    	iss>>count;
	    	type=1;
	    }
	    else if(hTag[0]!='#' && hTag!="STOP"){
	    	//type2 for number of remove Maxs... to be computed....
	    	 stringstream convert(hTag);
	    	 convert>>topNTags;
	    	 type=2;
	    	
	    }
	    else if(hTag=="STOP"){
	    	//base case to exit the program....
	    	break;
	    }
	    if(type==1){
	    	//create if not exists or call increase key operation...
	     	fibObj.createNode(count, hTag);
	 	}
	 	else if(type==2){
	 		//call remove max for topNTags times...
	 		while(topNTags--){
	 		
	 			if(topNTags!=0){
	 				outfile<<fibObj.getMax().substr(1,fibObj.getMax().length())<<",";
	 			}
	 			else{
	 				outfile<<fibObj.getMax().substr(1,fibObj.getMax().length());
	 			}
	 			//push the hash tag and hash tag count to reinsert again....
	 			hashStringStack.push(fibObj.getMax());
	 			hashCountStack.push(fibObj.getMaxCount());
	 			//now call the remove max operation...
	 			fibObj.removeMax();
	 		}
	 		//insert a new line in the output file...
	 		outfile<<endl;
	 		//reinsert the removed nodes in to the heap...
	 		while(hashStringStack.size()>0){
	 			//same as type 1 operation..
	 			fibObj.createNode(hashCountStack.top(), hashStringStack.top());
	 			hashStringStack.pop();
	 			hashCountStack.pop();
	 		}
	 	}
	}
	return 0;
}