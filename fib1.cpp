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
		void createNode(int hshCount, string hshTag){
			//check whether the tag is already there in hashtable or not...
			if(hashTable.find(hshTag) == hashTable.end()){
				FibonacciNode *newNode = new FibonacciNode(hshCount, hshTag);
				if(!maxPtr){
					maxPtr = newNode;
				}
				else{
					if(!maxPtr && !newNode){
						maxPtr= NULL;
					}
					else if(!maxPtr){
						maxPtr= newNode;
					}
					FibonacciNode *swapNode;
					swapNode = maxPtr->rightSibling;
					maxPtr->rightSibling = newNode->rightSibling;
					newNode->rightSibling = swapNode;
					maxPtr->rightSibling->leftSibling = maxPtr;
					newNode->rightSibling->leftSibling = newNode;
					maxPtr= (maxPtr->count > newNode->count)? maxPtr: newNode;
				}
				hashTable[hshTag]= newNode;
				noOfNodes++;
			}
			else{
				increaseKey(hshCount, hshTag);
			}
		}


		FibonacciNode * remNode(FibonacciNode * node){
			FibonacciNode * ret = node->rightSibling;
			if (ret == node){
				return NULL;
			}
			node->leftSibling->rightSibling = node->rightSibling;
			node->rightSibling->leftSibling = node->leftSibling;
			node->leftSibling = node;
			node->rightSibling = node;
			return ret;
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
		int removeMax(){
			if(!maxPtr){
				cout<<"E456mpty heap"<<endl;
				return 0;
			}
			FibonacciNode *delNode = maxPtr;
			noOfNodes--;
			hashTable.erase(maxPtr->hashTag);
			
			maxPtr = remNode(maxPtr);

			if(delNode -> nodeChild){
				FibonacciNode *tnode  = delNode->nodeChild;
				FibonacciNode *start = tnode;
				// do{
					tnode->nodeParent = NULL;
					tnode = tnode->rightSibling;
				// } while(tnode != start);
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
			pairWiseCombine();		
			return delNode->count;
		}
		
		void pairWiseCombine(){
			
			if (maxPtr->rightSibling == maxPtr) {
				return;
			}
			vector<FibonacciNode *> toCheck;
			FibonacciNode *curr = maxPtr;
			FibonacciNode *start = curr;
			// do{
				toCheck.push_back(curr);
				curr = curr->rightSibling;
			// } while(curr != start);
			while(curr != start){
				toCheck.push_back(curr);
				curr = curr->rightSibling;
			}

			vector<FibonacciNode *> degrees (int(log2(noOfNodes)) + 1);
			for (int i = 0; i < toCheck.size(); i++){
				curr = toCheck[i];
				if (curr->degree >= degrees.size()){
					degrees.resize(curr->degree+1);	
				} 
				if (!degrees[curr->degree]){
					degrees[curr->degree] = curr;
					if (curr->count >= maxPtr->count){
						maxPtr = curr;	
					} 
				}
				else {
					while (degrees[curr->degree]){					
						
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
			}
		}
		void increaseKey(int new_count, string hsTag){

			FibonacciNode *curr = hashTable[hsTag];
			
			curr->count = curr->count + new_count;
			hashTable.erase(hsTag);
			hashTable[hsTag] = curr;
			
			if (curr->nodeParent && (curr->nodeParent->count < curr->count)){
					childCut(curr);
			}
			if (curr->count > maxPtr->count){
				maxPtr=curr;	
			} 
		}
		void childCut(FibonacciNode *node){

			if (!node->nodeParent) {
				return;
			}
			node->isMarked =false;

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