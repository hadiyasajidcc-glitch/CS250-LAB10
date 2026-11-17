#include <iostream>
using namespace std;

const int MAX = 100;

struct Node {
    int serviceID;
    int monthlyCost;
    Node* left;
    Node* right;
    Node(int id, int cost) {
        serviceID = id;
        monthlyCost = cost;
        left = right = NULL;
    }
};

//BST is ordered by monthlyCost
Node* insert(Node* root, int id, int cost) {
    if (root == NULL) {
        return new Node(id, cost);
    }
    if (cost < root->monthlyCost) {
        root->left = insert(root->left, id, cost);
    } else if (cost > root->monthlyCost) {
        root->right = insert(root->right, id, cost);
    } else {
        cout << "Cost " << cost << " already exists, skipping service " << id << endl;
    }
    return root;
}

void printNodes(Node* arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << "S" << arr[i]->serviceID << "(" << arr[i]->monthlyCost << ")";
        if (i != size - 1) cout << " -> ";
    }
    cout << endl;
}

//---------- Part A ----------

Node* path[MAX];     //stack for the current path
int pathTop = 0;     //number of nodes in the path
int pathCount = 0;

//go down from one starting node and keep the path
void goDown(Node* node, int B, int sum) {
    if (node == NULL) return;

    path[pathTop] = node;      //push
    pathTop++;
    sum += node->monthlyCost;

    if (sum == B) {
        pathCount++;
        printNodes(path, pathTop);
    }

    //costs are positive so if sum is already >= B going deeper is useless
    if (sum < B) {
        goDown(node->left, B, sum);
        goDown(node->right, B, sum);
    }

    pathTop--;                 //pop
}

//try every node of the tree as the starting node
void findPaths(Node* root, int B) {
    if (root == NULL) return;

    pathTop = 0;
    goDown(root, B, 0);

    findPaths(root->left, B);
    findPaths(root->right, B);
}

//---------- Part B ----------

Node* nodeList[MAX];     //nodes in sorted order
int nodeListSize = 0;
Node* chosen[MAX];       //current subset
int chosenSize = 0;
int subsetCount = 0;

//inorder gives the nodes in increasing order of cost
void inorder(Node* root) {
    if (root == NULL) return;
    inorder(root->left);
    nodeList[nodeListSize] = root;
    nodeListSize++;
    inorder(root->right);
}

void findSubsets(int index, int B, int sum) {
    for (int i = index; i < nodeListSize; i++) {
        //list is sorted so if this one is too big everything after it
        //(the right side) is bigger too so stop here
        if (sum + nodeList[i]->monthlyCost > B) {
            break;
        }

        chosen[chosenSize] = nodeList[i];     //choose
        chosenSize++;

        if (sum + nodeList[i]->monthlyCost == B) {
            subsetCount++;
            printNodes(chosen, chosenSize);
        } else {
            findSubsets(i + 1, B, sum + nodeList[i]->monthlyCost);
        }

        chosenSize--;                         //undo
    }
}

void deleteTree(Node* root) {
    if (root == NULL) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    Node* root = NULL;
    int n, B;

    cout << "Enter number of services: ";
    cin >> n;
    if (n <= 0 || n > MAX) {
        cout << "Number of services must be between 1 and " << MAX << endl;
        return 0;
    }

    for (int i = 0; i < n; i++) {
        int id, cost;
        cout << "Enter serviceID and monthlyCost of service " << i + 1 << ": ";
        cin >> id >> cost;
        root = insert(root, id, cost);
    }

    cout << "Enter target budget B: ";
    cin >> B;

    cout << endl << "Part A - downward paths with sum B:" << endl;
    findPaths(root, B);
    if (pathCount == 0) {
        cout << "No path found." << endl;
    }

    cout << endl << "Part B - subsets with sum B:" << endl;
    inorder(root);
    findSubsets(0, B, 0);
    if (subsetCount == 0) {
        cout << "No subset found." << endl;
    }

    deleteTree(root);
    return 0;
}