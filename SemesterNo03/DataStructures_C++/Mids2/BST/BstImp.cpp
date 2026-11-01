#include <iostream>
using namespace std;

class BST
{
    // All the Nodes in the BST:
    class node
    {
    public:
        int data;
        node *left;
        node *right;

        node()
        {
            data = 0;
            left = right = NULL;
        }

        node(int data)
        {
            this->data = data;
            left = NULL;
            right = NULL;
        }
    };

    // BST ROOT node:

    node *root;
    public:
    BST()
    {
        root = NULL;
    }   
    // Traversals:

    // 1-preOrder Traversal:
    void preOrder(node *temp)
    {
        if (temp == NULL)
            return;
        cout << temp->data;
        inOrder(temp->left);
        inOrder(temp->right);
    }
    // 2-inOrder Traversal:
    void inOrder(node *temp)
    {
        if (temp == NULL)
            return;
        inOrder(temp->left);
        cout << temp->data;
        inOrder(temp->right);
    }
    // 3-PostOrder Traversal:
    void postOrder(node *temp)
    {
        if (temp == NULL)
            return;
        inOrder(temp->left);
        inOrder(temp->right);
        cout << temp->data;
    }

    void printBST()
    {
        if(root == NULL)
        {
            cerr<<"Tree is Empty.. Working on an empty branch"<<endl;
        }
    }
};

int main()
{
    BST tree;

    tree.printBST();
    
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}