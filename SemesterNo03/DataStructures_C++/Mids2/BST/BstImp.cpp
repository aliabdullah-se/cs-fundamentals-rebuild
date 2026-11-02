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
    node *minNode(node *min)
    {
        while (min && min->left)
        {
            min = min->left;
        }
        return min;
    }

    node *add(node *temp, int data)
    {
        if (temp == NULL)
        {
            return new node(data);
        }
        else if (temp->data < data)
        {
            temp->right = add(temp->right, data);
        }
        else if (temp->data > data)
        {
            temp->left = add(temp->left, data);
        }
        else
        {
            cout << "Duplicate Branch... Error!!" << endl;
        }
        return temp;
    }

    // delete
    node *remove(node *temp, int data)
    {
        if (temp == NULL)
        {
            return new node(data);
        }
        else if (temp->data < data)
        {
            temp->right = add(temp->right, data);
        }
        else if (temp->data > data)
        {
            temp->left = add(temp->left, data);
        }
        else
        {
            if (temp->left == NULL || temp->right == NULL)
            {
                node *dummy;
                if (temp->left != NULL)
                {
                    dummy = temp->left;
                }
                else
                {
                    dummy = temp->right;
                }
                delete temp;
                return dummy;
            }
            else
            {
                node *succ = minNode(temp->right);
                temp->data = succ->data;
                temp->right = remove(temp->right, data);
            }
        }
        return temp;
    }
    // search
    bool search(node *temp, int key)
    {
        if (temp == NULL)
        {
            return false;
        }
        else if (temp->data == key)
        {
            return true;
        }
        else if (key > temp->data)
        {
            return search(temp->right, key);
        }
        else
        {
            return search(temp->left, key);
        }
    }

    // 2-inOrder Traversal:
    void inOrder(node *temp)
    {
        if (temp == NULL)
            return;
        inOrder(temp->left);
        cout << temp->data << " , ";
        inOrder(temp->right);
    }

public:
    BST()
    {
        root = NULL;
    }
    // Add Method:
    void add(int data)
    {
        root = add(root, data);
    }
    // remove
    void remove(int data)
    {
        root = remove(root, data);
    }
    // search method
    bool search(int key)
    {
        return search(root, key);
    }

    void update(int oldData, int newData)
    {
        if (search(oldData))
        {
            remove(oldData);
            add(newData);
        }
        else
        {
            cout << "Data Not Found" << endl;
        }
    }

    // Traversals:

    // 1-preOrder Traversal:
    void preOrder(node *temp)
    {
        if (temp == NULL)
            return;
        cout << temp->data;
        preOrder(temp->left);
        preOrder(temp->right);
    }

    // 3-PostOrder Traversal:
    void postOrder(node *temp)
    {
        if (temp == NULL)
            return;
        postOrder(temp->left);
        postOrder(temp->right);
        cout << temp->data;
    }

    void printBST()
    {
        if (root == NULL)
        {
            cerr << "Tree is Empty.. Working on an empty branch" << endl;
        }
        else
        {
            inOrder(root);
        }
    }
};

int main()
{
    BST tree;
    tree.add(10);
    tree.add(30);
    tree.add(50);
    tree.add(70);
    tree.add(90);
    tree.remove(90);
    tree.printBST();

    ios::sync_with_stdio(false);
    cin.tie(NULL);

    system("pause");
    return 0;
}