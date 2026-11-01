#include <iostream>
using namespace std;

// array based stack implementation:

class stack
{
private:
    int size;
    int capacity;
    int top;
    int *arr;

public:
    stack()
    {
        capacity = 10;
        top = -1;
        arr = new int[capacity];
    }

    stack(int capacity)
    {
        this->capacity = capacity;
        top = -1;
        arr = new int[capacity];
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}