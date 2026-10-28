// File: 25L-3022.cpp
// Data Structures Lab 08 - Tasks 2, 3, 4 and 5
// Tasks 0 and 1 are paper-based and are not included here.

#include <iostream>
#include <stack>
#include <queue>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) {
        data = value;
        left = NULL;
        right = NULL;
    }
};

class BST {
private:
    Node* root;

    void clearTree(Node* node) {
        if (node == NULL) {
            return;
        }

        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

public:
    BST() {
        root = NULL;
    }

    ~BST() {
        clearTree(root);
    }

    Node* getRoot() {
        return root;
    }

    // Task 2: Insert
    void insert(int value) {
        Node* newNode = new Node(value);

        if (root == NULL) {
            root = newNode;
            return;
        }

        Node* current = root;
        Node* parent = NULL;

        while (current != NULL) {
            parent = current;

            if (value < current->data) {
                current = current->left;
            }
            else if (value > current->data) {
                current = current->right;
            }
            else {
                delete newNode;
                return; // Ignore duplicate values
            }
        }

        if (value < parent->data) {
            parent->left = newNode;
        }
        else {
            parent->right = newNode;
        }
    }

    // Task 2: Search
    bool search(int value) {
        Node* current = root;

        while (current != NULL) {
            if (value == current->data) {
                return true;
            }
            else if (value < current->data) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        return false;
    }

    // Task 2: Remove a value
    void removeValue(int value) {
        Node* current = root;
        Node* parent = NULL;

        while (current != NULL && current->data != value) {
            parent = current;

            if (value < current->data) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        if (current == NULL) {
            cout << value << " was not found." << endl;
            return;
        }

        // If the node has two children, copy its inorder successor.
        if (current->left != NULL && current->right != NULL) {
            Node* successorParent = current;
            Node* successor = current->right;

            while (successor->left != NULL) {
                successorParent = successor;
                successor = successor->left;
            }

            current->data = successor->data;
            current = successor;
            parent = successorParent;
        }

        // The node now has zero or one child.
        Node* child = NULL;

        if (current->left != NULL) {
            child = current->left;
        }
        else {
            child = current->right;
        }

        if (parent == NULL) {
            root = child;
        }
        else if (parent->left == current) {
            parent->left = child;
        }
        else {
            parent->right = child;
        }

        delete current;
    }

    // Task 2: Minimum
    int findMin() {
        if (root == NULL) {
            return -1;
        }

        Node* current = root;

        while (current->left != NULL) {
            current = current->left;
        }

        return current->data;
    }

    // Task 2: Maximum
    int findMax() {
        if (root == NULL) {
            return -1;
        }

        Node* current = root;

        while (current->right != NULL) {
            current = current->right;
        }

        return current->data;
    }

    // Task 2: Inorder traversal
    void inorder() {
        stack<Node*> nodes;
        Node* current = root;

        while (current != NULL || !nodes.empty()) {
            while (current != NULL) {
                nodes.push(current);
                current = current->left;
            }

            current = nodes.top();
            nodes.pop();

            cout << current->data << " ";
            current = current->right;
        }

        cout << endl;
    }

    // Task 2: Preorder traversal
    void preorder() {
        if (root == NULL) {
            cout << "Tree is empty." << endl;
            return;
        }

        stack<Node*> nodes;
        nodes.push(root);

        while (!nodes.empty()) {
            Node* current = nodes.top();
            nodes.pop();

            cout << current->data << " ";

            if (current->right != NULL) {
                nodes.push(current->right);
            }

            if (current->left != NULL) {
                nodes.push(current->left);
            }
        }

        cout << endl;
    }

    // Task 2: Postorder traversal using two stacks
    void postorder() {
        if (root == NULL) {
            cout << "Tree is empty." << endl;
            return;
        }

        stack<Node*> first;
        stack<Node*> second;

        first.push(root);

        while (!first.empty()) {
            Node* current = first.top();
            first.pop();

            second.push(current);

            if (current->left != NULL) {
                first.push(current->left);
            }

            if (current->right != NULL) {
                first.push(current->right);
            }
        }

        while (!second.empty()) {
            cout << second.top()->data << " ";
            second.pop();
        }

        cout << endl;
    }

    // Task 2: Level-order traversal
    void levelOrder() {
        if (root == NULL) {
            cout << "Tree is empty." << endl;
            return;
        }

        queue<Node*> nodes;
        nodes.push(root);

        while (!nodes.empty()) {
            Node* current = nodes.front();
            nodes.pop();

            cout << current->data << " ";

            if (current->left != NULL) {
                nodes.push(current->left);
            }

            if (current->right != NULL) {
                nodes.push(current->right);
            }
        }

        cout << endl;
    }

    // Task 3: Return the kth smallest value.
    // Returns -1 if k is invalid or the tree has fewer than k nodes.
    int kthSmallest(int k) {
        if (k <= 0) {
            return -1;
        }

        stack<Node*> nodes;
        Node* current = root;
        int count = 0;

        while (current != NULL || !nodes.empty()) {
            while (current != NULL) {
                nodes.push(current);
                current = current->left;
            }

            current = nodes.top();
            nodes.pop();
            count++;

            if (count == k) {
                return current->data;
            }

            current = current->right;
        }

        return -1;
    }

    // Task 4: Validate the BST ordering rule.
    // Every node must be greater than its lower bound and
    // smaller than its upper bound.
    bool validateFrom(Node* node, long long lower, long long upper) {
        if (node == NULL) {
            return true;
        }

        if (node->data <= lower || node->data >= upper) {
            return false;
        }

        bool leftValid = validateFrom(node->left, lower, node->data);
        bool rightValid = validateFrom(node->right, node->data, upper);

        if (leftValid == false || rightValid == false) {
            return false;
        }

        return true;
    }

    bool isValidBST(Node* node) {
        return validateFrom(node, -2147483649LL, 2147483648LL);
    }

    // Task 5: Check whether a value exists in a given tree.
    bool contains(Node* node, int value) {
        Node* current = node;

        while (current != NULL) {
            if (current->data == value) {
                return true;
            }
            else if (value < current->data) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        return false;
    }

    // Task 5: Lowest common ancestor.
    // Returns NULL if either key is missing.
    Node* lowestCommonAncestor(int a, int b) {
        if (contains(root, a) == false || contains(root, b) == false) {
            return NULL;
        }

        Node* current = root;

        while (current != NULL) {
            if (a < current->data && b < current->data) {
                current = current->left;
            }
            else if (a > current->data && b > current->data) {
                current = current->right;
            }
            else {
                return current;
            }
        }

        return NULL;
    }

    // Task 2 demonstration
    void runTask2() {
        cout << endl << "--- Task 2: BST Operations and Traversals ---" << endl;

        insert(45);
        insert(25);
        insert(65);
        insert(15);
        insert(35);
        insert(55);
        insert(75);
        insert(30);

        cout << "Inorder: ";
        inorder();

        cout << "Preorder: ";
        preorder();

        cout << "Postorder: ";
        postorder();

        cout << "Level order: ";
        levelOrder();

        if (search(35)) {
            cout << "Search 35: Found" << endl;
        }
        else {
            cout << "Search 35: Not found" << endl;
        }

        if (search(90)) {
            cout << "Search 90: Found" << endl;
        }
        else {
            cout << "Search 90: Not found" << endl;
        }

        cout << "Minimum: " << findMin() << endl;
        cout << "Maximum: " << findMax() << endl;

        cout << "Deleting 15..." << endl;
        removeValue(15);
        cout << "Inorder after deleting 15: ";
        inorder();

        cout << "Deleting 35..." << endl;
        removeValue(35);
        cout << "Inorder after deleting 35: ";
        inorder();

        cout << "Deleting 45..." << endl;
        removeValue(45);
        cout << "Inorder after deleting 45: ";
        inorder();
    }

    // Task 3 demonstration
    void runTask3() {
        cout << endl << "--- Task 3: Kth Smallest Element ---" << endl;

        insert(45);
        insert(25);
        insert(65);
        insert(15);
        insert(35);
        insert(55);
        insert(75);
        insert(30);

        int k;
        cout << "Enter k: ";
        cin >> k;

        int answer = kthSmallest(k);

        if (answer == -1) {
            cout << "Invalid k: no such element." << endl;
        }
        else {
            cout << k << "th smallest element: " << answer << endl;
        }
    }

    // Task 5 demonstration
    void runTask5() {
        cout << endl << "--- Task 5: Lowest Common Ancestor ---" << endl;

        insert(45);
        insert(25);
        insert(65);
        insert(15);
        insert(35);
        insert(55);
        insert(75);
        insert(30);

        int a, b;
        cout << "Enter the first key: ";
        cin >> a;
        cout << "Enter the second key: ";
        cin >> b;

        Node* answer = lowestCommonAncestor(a, b);

        if (answer == NULL) {
            cout << "LCA cannot be found because at least one key is missing."
                 << endl;
        }
        else {
            cout << "Lowest common ancestor: " << answer->data << endl;
        }
    }
};

// Task 4 demonstration uses three small trees.
void runTask4() {
    cout << endl << "--- Task 4: Validate a BST ---" << endl;

    BST checker;

    // Valid tree
    Node* valid = new Node(40);
    valid->left = new Node(20);
    valid->right = new Node(60);

    // Invalid: 55 is in the left subtree of 40.
    Node* invalidOne = new Node(40);
    invalidOne->left = new Node(20);
    invalidOne->left->right = new Node(55);

    // Invalid: 35 is in the right subtree of 40.
    Node* invalidTwo = new Node(40);
    invalidTwo->right = new Node(60);
    invalidTwo->right->left = new Node(35);

    if (checker.isValidBST(valid)) {
        cout << "Tree 1: Valid BST" << endl;
    }
    else {
        cout << "Tree 1: Invalid BST" << endl;
    }

    if (checker.isValidBST(invalidOne)) {
        cout << "Tree 2: Valid BST" << endl;
    }
    else {
        cout << "Tree 2: Invalid BST" << endl;
    }

    if (checker.isValidBST(invalidTwo)) {
        cout << "Tree 3: Valid BST" << endl;
    }
    else {
        cout << "Tree 3: Invalid BST" << endl;
    }

    // Release the manually created trees.
    // This helper is local to the demonstration.
    // Delete nodes in child-first order.
    delete valid->left;
    delete valid->right;
    delete valid;

    delete invalidOne->left->right;
    delete invalidOne->left;
    delete invalidOne;

    delete invalidTwo->right->left;
    delete invalidTwo->right;
    delete invalidTwo;
}

int main() {
    int choice;

    do {
        cout << endl;
        cout << "========== DATA STRUCTURES LAB 08 ==========" << endl;
        cout << "Tasks 0 and 1 are paper-based." << endl;
        cout << "Choose a question to run:" << endl;
        cout << "2. BST operations and traversals" << endl;
        cout << "3. Kth smallest element" << endl;
        cout << "4. Validate a BST" << endl;
        cout << "5. Lowest common ancestor" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Exiting." << endl;
            break;
        }

        if (choice == 2) {
            BST tree;
            tree.runTask2();
        }
        else if (choice == 3) {
            BST tree;
            tree.runTask3();
        }
        else if (choice == 4) {
            runTask4();
        }
        else if (choice == 5) {
            BST tree;
            tree.runTask5();
        }
        else if (choice == 0) {
            cout << "Program ended." << endl;
        }
        else {
            cout << "Invalid choice. Select 2, 3, 4, 5 or 0." << endl;
        }

    } while (choice != 0);

    return 0;
}
