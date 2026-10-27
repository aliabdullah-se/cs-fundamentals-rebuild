class Iterator
{
    Node *ptr;

public:
    Iterator(Node *p = NULL)
    {
        ptr = p;
    }
    int &operator*()
    {
        return ptr->data;
        
    } // *it gives the data

    Iterator &operator++() // ++it moves to next node
    {
        ptr = ptr->next;
        return *this;
    }
    bool operator!=(const Iterator &other) const
    {
        return ptr != other.ptr;
    }
    bool operator==(const Iterator &other) const
    {
        return ptr == other.ptr;
    }
};

Iterator begin()
{
    return Iterator(head);
} // first node
Iterator end() 
{ 
    return Iterator(NULL); 
}
     // one PAST the last node

#include <iostream>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}