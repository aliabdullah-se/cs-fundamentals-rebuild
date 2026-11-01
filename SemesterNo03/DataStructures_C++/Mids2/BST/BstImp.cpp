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
    node *root;
    // Traversals:

    //1-preOrder Traversal:
    void preOrder(node* temp)
    {
        if(temp == NULL)
        return;
        cout << temp->data;
        inOrder(temp -> left);
        inOrder(temp -> right);
    }
    //2-inOrder Traversal:
    void inOrder(node* temp)
    {
        if(temp == NULL)
        return;
        inOrder(temp -> left);
        cout << temp->data;
        inOrder(temp -> right);
    }
    //3-PostOrder Traversal:
    void postOrder(node* temp)
    {
        if(temp == NULL)
        return;
        inOrder(temp -> left);
        inOrder(temp -> right);
        cout << temp->data;
    }

};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}