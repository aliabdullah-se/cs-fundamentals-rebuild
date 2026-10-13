// Task 5: Merge two sorted linked lists by splicing the existing nodes
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

const int MAX_LEN = 50;
const int MIN_VAL = -100;
const int MAX_VAL = 100;

class LinkedList
{
private:
    Node *head;
    Node *tail;
    int size;

    void recount()
    {
        size = 0;
        tail = nullptr;
        for (Node *c = head; c; c = c->next)
        {
            size++;
            tail = c;
        }
    }

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

    // hands the nodes over to the caller, this list becomes empty
    Node *release()
    {
        Node *h = head;
        head = tail = nullptr;
        size = 0;
        return h;
    }

    void adopt(Node *h)
    {
        clear();
        head = h;
        recount();
    }

    // smallest value the next appended element may have
    int lowestAllowed() const { return tail ? tail->data : MIN_VAL; }
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

// relinks the nodes of both lists, nothing is copied or allocated
Node *spliceMerge(Node *a, Node *b)
{
    Node dummy(0);
    Node *t = &dummy;
    while (a && b)
    {
        if (a->data <= b->data)
        {
            t->next = a;
            a = a->next;
        }
        else
        {
            t->next = b;
            b = b->next;
        }
        t = t->next;
    }
    t->next = a ? a : b;
    return dummy.next;
}

// values must be typed in sorted order, so the lower bound moves up as we go
void fillList(LinkedList &l)
{
    int n = readIntRange("How many elements (0 to 50): ", 0, MAX_LEN);
    l.clear();
    for (int i = 1; i <= n; i++)
    {
        cout << "Element " << i << " ";
        l.append(readIntRange("(non decreasing): ", l.lowestAllowed(), MAX_VAL));
    }
}

LinkedList &pickList(LinkedList &a, LinkedList &b)
{
    return readIntRange("Which list, 1 for List1 and 2 for List2: ", 1, 2) == 1 ? a : b;
}

int main()
{
    LinkedList a, b, merged;
    int choice = -1;
    while (choice != 0)
    {
        cout << endl;
        cout << "Merge two sorted linked lists" << endl;
        cout << "1. Fill a list" << endl;
        cout << "2. Append one value to a list" << endl;
        cout << "3. Display lists" << endl;
        cout << "4. Merge List1 and List2" << endl;
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
            l.append(readIntRange("Value (must not be smaller than the last one): ", l.lowestAllowed(), MAX_VAL));
            break;
        }
        case 3:
            cout << "List1: ";
            a.display();
            cout << "List2: ";
            b.display();
            cout << "Merged: ";
            merged.display();
            break;
        case 4:
            merged.adopt(spliceMerge(a.release(), b.release()));
            cout << "Merged: ";
            merged.display();
            cout << "List1 and List2 gave their nodes away, so they are empty now" << endl;
            break;
        }
    }
    cout << "Bye" << endl;
    return 0;
}
