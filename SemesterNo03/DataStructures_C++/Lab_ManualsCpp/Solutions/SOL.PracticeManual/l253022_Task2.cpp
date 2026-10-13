// Task 2: Difference of two linked lists (A - B)
// Roll no: l253022

#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node *next;
    Node(int d) : data(d), next(nullptr) {}
};

// asks again until the user types a whole number
int readInt(const char *prompt)
{
    int value;
    cout << prompt;
    while (!(cin >> value))
    {
        if (cin.eof())
            return 0;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please type a whole number: ";
    }
    cin.ignore(1000, '\n');
    return value;
}

// same thing, but only for places where the manual gives a limit
int readIntRange(const char *prompt, int lo, int hi)
{
    int value = readInt(prompt);
    while ((value < lo || value > hi) && !cin.eof())
    {
        cout << "Value must be between " << lo << " and " << hi << endl;
        value = readInt(prompt);
    }
    if (value < lo || value > hi)
        return lo;
    return value;
}

const int MAX_LEN = 10000;

class LinkedList
{
private:
    Node *head;
    Node *tail;
    int size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~LinkedList() { clear(); }
    LinkedList(const LinkedList &) = delete;
    LinkedList &operator=(const LinkedList &) = delete;

    void append(int v)
    {
        Node *n = new Node(v);
        if (!head)
            head = tail = n;
        else
        {
            tail->next = n;
            tail = n;
        }
        size++;
    }

    void clear()
    {
        while (head)
        {
            Node *t = head;
            head = head->next;
            delete t;
        }
        tail = nullptr;
        size = 0;
    }

    bool contains(int v) const
    {
        for (Node *c = head; c; c = c->next)
            if (c->data == v)
                return true;
        return false;
    }

    Node *getHead() const { return head; }
    int getSize() const { return size; }

    void display() const
    {
        if (!head)
        {
            cout << "(empty)" << endl;
            return;
        }
        for (Node *c = head; c; c = c->next)
            cout << c->data << (c->next ? " -> " : "");
        cout << endl;
    }
};

// keeps A's order and each value shows up only once in the result
void buildDifference(const LinkedList &a, const LinkedList &b, LinkedList &out)
{
    out.clear();
    for (Node *c = a.getHead(); c; c = c->next)
    {
        if (!b.contains(c->data) && !out.contains(c->data))
            out.append(c->data);
    }
}

void fillList(LinkedList &l)
{
    int n = readIntRange("How many elements (0 to 10000): ", 0, MAX_LEN);
    l.clear();
    for (int i = 1; i <= n; i++)
    {
        cout << "Element " << i << ": ";
        l.append(readInt(""));
    }
}

LinkedList &pickList(LinkedList &a, LinkedList &b)
{
    return readIntRange("Which list, 1 for A and 2 for B: ", 1, 2) == 1 ? a : b;
}

int main()
{
    LinkedList a, b, result;
    int choice = -1;
    while (choice != 0)
    {
        cout << endl;
        cout << "Difference of two linked lists (A - B)" << endl;
        cout << "1. Fill a list" << endl;
        cout << "2. Append one value to a list" << endl;
        cout << "3. Display lists" << endl;
        cout << "4. Compute A - B" << endl;
        cout << "0. Exit" << endl;
        choice = readIntRange("Choice: ", 0, 4);
        switch (choice)
        {
        case 1:
            fillList(pickList(a, b));
            break;
        case 2:
        {
            LinkedList &l = pickList(a, b);
            if (l.getSize() >= MAX_LEN)
            {
                cout << "List is full, max 10000 nodes" << endl;
                break;
            }
            l.append(readInt("Value: "));
            break;
        }
        case 3:
            cout << "A: ";
            a.display();
            cout << "B: ";
            b.display();
            break;
        case 4:
            buildDifference(a, b, result);
            cout << "A - B: ";
            result.display();
            break;
        }
    }
    cout << "Bye" << endl;
    return 0;
}
