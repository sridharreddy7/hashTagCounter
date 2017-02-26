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
#define ln length()
#define rep(i,n) for(int i=0;i<n;i++)
#define fu(i,a,n) for(int i=a;i<=n;i++)
#define fd(i,n,a) for(int i=n;i>=a;i--)
#define all(a)  a.begin(),a.end() 
 
//modify these
#define tr(ctr, it) for(typeof(ctr.begin()) it = ctr.begin(); it != ctr.end(); it++)
#define present(ctr, element) (ctr.find(element) != ctr.end()) 
#define cpresent(ctr, element) (find(all(ctr),element) != ctr.end()) 
#define sz(a) int((a).size()) 


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


stack <string> hashStringStack;
stack <int> hashCountStack;

//Fibonacci node structure.
struct FibonacciNode{
	FibonacciNode *nodeParent;
	FibonacciNode *rightSibling;
	
	int count;

	int degree;
	bool isMarked;
	string hashTag;
	
	FibonacciNode *leftSibling;
	FibonacciNode *nodeChild;

	FibonacciNode(int hashCount, string hshTag){
		nodeParent = NULL;
		rightSibling = this;

		count = hashCount;
		degree =0;
		isMarked = false;
		
		hashTag = hshTag;
		
		leftSibling = this;
		nodeChild = NULL;
		
	}
};

class FibonacciClass{
	private:
		FibonacciNode *maxPtr;
		int noOfNodes;
		//hashtable to store hash tag and pointer to fibonacci heap...
		map<string, FibonacciNode*>hashTable;
	public:
		//check this after removing this line too....
		// FibonacciClass(): maxPtr(NULL), noOfNodes(){};

		void createNode(int hshCount, string hshTag){
			//check whether the tag is already there in hashtable or not...
			if(hashTable.find(hshTag) == hashTable.end()){
				FibonacciNode *newNode = new FibonacciNode(hshCount, hshTag);
				if(!maxPtr){
					maxPtr = newNode;
				}
				else{
					maxPtr = mergeLists(maxPtr, newNode);
				}
				hashTable[hshTag]= newNode;
				noOfNodes++;
			}
			else{
				increaseKey(hshCount, hshTag);
			}
		}

		FibonacciNode * mergeLists(FibonacciNode *first, FibonacciNode *second){
			if(!first && !second){
				return NULL;
			}
			else if(!first){
				return second;
			}
			else if(!second){
				return first;
			}

			FibonacciNode *swapNode;

			swapNode = first->rightSibling;
			first->rightSibling = second->rightSibling;
			second->rightSibling = swapNode;

			first->rightSibling->leftSibling = first;
			second->rightSibling->leftSibling = second;

			return (first->count > second->count)? first: second;
		}

		string getMin(){
			if(maxPtr){
				return maxPtr->hashTag;
			}
			
			return "E123mpty heap";
			
		}

		int getMinCount(){
			if(maxPtr){
				return maxPtr->count;
			}
			return 0;
		}

		//change the function ...

		FibonacciNode * removeNode(FibonacciNode * n){
			FibonacciNode * ret = n->rightSibling;
			
			//If its the last node
			if (ret == n){
				return NULL;
			}
			
			n->rightSibling->leftSibling = n->leftSibling;
			n->leftSibling->rightSibling = n->rightSibling;
			n->rightSibling = n;
			n->leftSibling = n;
			
			return ret;
		}

		int removeMax(){
			if(!maxPtr){
				cout<<"E456mpty heap"<<endl;
				return 0;
			}
			FibonacciNode *delNode = maxPtr;

			hashTable.erase(maxPtr->hashTag);
			noOfNodes--;

			maxPtr = removeNode(maxPtr);

			if(delNode -> nodeChild){

				FibonacciNode *a  = delNode->nodeChild;
				FibonacciNode *start = a;
				do{
					a->nodeParent = NULL;
					a = a->rightSibling;
				} while(a != start);
				
				maxPtr = mergeLists(delNode->nodeChild, maxPtr);

			}
			if (!maxPtr) return delNode->count;
		
			//Perform consolidation on the new root list
			consolidateList();
		
			return delNode->count;
		}


		void consolidateList(){
		
			// Start with the bound = log_2[size] possible ranks in the tree
			// Expanded if we encounter higher ranks
			vector<FibonacciNode *> rank_to_nodes (int(log2(noOfNodes)) + 1);

			//If singleton tree
			if (maxPtr->rightSibling == maxPtr) return;

			/* We need to ensure we don't go over the same node twice. 
			* Here we choose to go with a high memory overhead vs. a runtime overhead.
			* We go through and store all the nodes in the root list in a fixed array
			* and iterate over this array.
			*/ 
			
			//Setup list of nodes to visit.
			vector<FibonacciNode *> nodes_to_visit;
			FibonacciNode *curr = maxPtr;
			FibonacciNode *start = curr;
			do{
				nodes_to_visit.push_back(curr);
				curr = curr->rightSibling;
			} while(curr != start);
			
			//Go over all the nodes in the nodes_to_visit array
			for (int i = 0; i < nodes_to_visit.size(); i++){
				curr = nodes_to_visit[i];
				
				//Ensure the rank list is big enough
				if (curr->degree >= rank_to_nodes.size()) rank_to_nodes.resize(curr->degree+1);

				// If it is the first node of its rank, just add it
				if (!rank_to_nodes[curr->degree]){
					rank_to_nodes[curr->degree] = curr;
					if (curr->count >= maxPtr->count) maxPtr = curr;
				}
				// Otherwise we merge the two trees. 
				else {
					while (rank_to_nodes[curr->degree]){					
						//Assign the new curr after linking. 
						curr = link(rank_to_nodes[curr->degree], curr);
						//Empty the previous rank
						rank_to_nodes[curr->degree - 1] = NULL;
						//Update min, making sure its a root pointer.
						if(curr->count >= maxPtr->count) maxPtr = curr;
						//If it needs to be resized.
						if (curr->degree >= rank_to_nodes.size()) rank_to_nodes.resize(curr->degree+1);
					}
					//Add the node to this new rank
					rank_to_nodes[curr->degree] = curr;
				}
			}
		}

		FibonacciNode * link(FibonacciNode *a, FibonacciNode *b){
		
			if (a == b) cout << "\n Warning: linking node to itself \n";
			
			//Remove the larger node from the root list
			FibonacciNode *min, *max;
			if (a->count > b->count){
				min = a;
				max = b;
			} else {
				min = b;
				max = a;
			}
			removeNode(max);
			
			//Make the larger node a child of the smaller 		
			min->nodeChild = mergeLists(min->nodeChild, max);
			max->nodeParent = min;
			
			//Increase the rank of the smaller node
			min->degree++;
			
			return min;
		}
		void increaseKey(int new_count, string hsTag){

			FibonacciNode *curr = hashTable[hsTag];

			curr->count = curr->count + new_count;

			hashTable.erase(hsTag);

			hashTable[hsTag] = curr;
			if (curr->nodeParent){
				if (curr->nodeParent->count < curr->count) 
					cutNode(curr);
			}
			
			//Make sure min is updated if necessary
			if (curr->count > maxPtr->count) maxPtr=curr;

		}

		void cutNode(FibonacciNode *to_cut){
			to_cut->isMarked = false;
			
			//If we hit a root, stop the cuts
			if (!to_cut->nodeParent) return;
			
			//If it was the node that its parent pointed to, change the parent's child pointer
			if (to_cut->nodeParent->nodeChild == to_cut){
				if (to_cut->rightSibling == to_cut) to_cut->nodeParent->nodeChild = NULL;
				else to_cut->nodeParent->nodeChild = to_cut->rightSibling;
			}
			
			//Remove the node from its list
			removeNode(to_cut);
			
			//Merge it with the root list
			maxPtr = mergeLists(maxPtr, to_cut);
			
			//Decrease parents degree
			to_cut->nodeParent->degree--;
			
			//Recursive call
			if (!to_cut->nodeParent->isMarked) to_cut->nodeParent->isMarked = true;
			else cutNode(to_cut->nodeParent);
			
			//Set parent to null since it is in root list
			to_cut->nodeParent = NULL;
		}
};

int main(int argc, char* argv[]) {
	
	FibonacciClass fibObj;
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
	    else if(hTag[0]!='#' && (hTag!="STOP" || hTag!="stop")){
	    	 stringstream convert(hTag);
	    	 convert>>topNTags;
	    	 type=2;
	    }
	    else if(hTag=="STOP" || hTag=="stop"){
	    	break;
	    }
	    if(type==1){
	     	fibObj.createNode(count, hTag);
	 	}
	 	else if(type==2){
	 		while(topNTags--){
	 			if(topNTags!=0){
	 				outfile<<fibObj.getMin().substr(1,fibObj.getMin().length())<<fibObj.getMinCount()<<",";
	 			}
	 			else{
	 				outfile<<fibObj.getMin().substr(1,fibObj.getMin().length())<<fibObj.getMinCount();
	 			}
	 			hashStringStack.push(fibObj.getMin());
	 			hashCountStack.push(fibObj.getMinCount());
	 			fibObj.removeMax();
	 		}
	 		
	 		//end of writng rmoveMax to output file.
	 		outfile<<endl;

	 		while(hashStringStack.size()>0){
	 			fibObj.createNode(hashCountStack.top(), hashStringStack.top());
	 			hashStringStack.pop();
	 			hashCountStack.pop();
	 		}
	 	}
	}
}