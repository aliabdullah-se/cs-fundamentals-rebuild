#include <iostream>
using namespace std;

class BST
{
    class node
    {
        public:
        int data;
        node *left;
        node *right;

        node(int data)
        {
            this->data = data;
            left = NULL;
            right = NULL;
        }
    };
    node *root;
    // Traversals:
    //1-inOrder Traversal:

    void inOrder(node* temp)
    {
        if(temp == NULL)
        return;
        inOrder(temp -> left);
        cout << temp->data;
        inOrder(temp -> right);
    }
    //2-PreOrder Traversal:
    void inOrder(node* temp)
    {
        if(temp == NULL)
        return;
        inOrder(temp -> left);
        cout << temp->data;
        inOrder(temp -> right);
    }
    //PostOrder Traversal:
    void inOrder(node* temp)
    {
        if(temp == NULL)
        return;
        inOrder(temp -> left);
        cout << temp->data;
        inOrder(temp -> right);
    }

};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}