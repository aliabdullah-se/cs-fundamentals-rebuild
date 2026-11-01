#include <iostream>
using namespace std;

class BST
{
    class node
    {
    private:
        int data;
        node *left;
        node *right;

    public:
        node(int data)
        {
            this->data = data;
            left = NULL;
            right = NULL;
        }
    };
    node *root;
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}