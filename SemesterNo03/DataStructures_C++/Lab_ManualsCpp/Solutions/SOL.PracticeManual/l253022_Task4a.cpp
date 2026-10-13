// Task 4: Create an intersection of two linked lists
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
    Node *joinPoint; // first node this list shares with another list, if any

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0), joinPoint(nullptr) {}
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

    // stops at the shared part so the other list's nodes are not freed twice
    void clear()
    {
        while (head && head != joinPoint)
        {
            Node *t = head;
            head = head->next;
            delete t;
        }
        head = tail = nullptr;
        joinPoint = nullptr;
        size = 0;
    }

    // links the end of this list to an existing node, no new nodes are made
    void attachTo(Node *target)
    {
        if (!head)
            head = target;
        else
            tail->next = target;
        joinPoint = target;
        size = 0;
        for (Node *c = head; c; c = c->next)
            size++;
    }

    bool isEmpty() const { return head == nullptr; }
    bool isJoined() const { return joinPoint != nullptr; }
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

// B joins A at the k-th node of A (1 based), returns false if A is too short
bool makeIntersection(LinkedList &a, LinkedList &b, int k)
{
    if (k < 1 || b.isJoined())
        return false;
    Node *kth = a.getHead();
    for (int i = 1; i < k && kth; i++)
        kth = kth->next;
    if (!kth)
        return false;
    b.attachTo(kth);
    return true;
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

void resetBoth(LinkedList &a, LinkedList &b)
{
    b.clear(); // B first, since it may be borrowing nodes from A
    a.clear();
}

int main()
{
    LinkedList a, b;
    int choice = -1;
    while (choice != 0)
    {
        cout << endl;
        cout << "Create an intersection of two linked lists" << endl;
        cout << "1. Fill a list" << endl;
        cout << "2. Append one value to a list" << endl;
        cout << "3. Display lists" << endl;
        cout << "4. Make B join A at the k-th node" << endl;
        cout << "5. Reset both lists" << endl;
        cout << "0. Exit" << endl;
        choice = readIntRange("Choice: ", 0, 5);
        switch (choice)
        {
        case 1:
            if (b.isJoined())
            {
                cout << "The lists are joined, reset them first" << endl;
                break;
            }
            fillList(pickList(a, b));
            break;
        case 2:
        {
            if (b.isJoined())
            {
                cout << "The lists are joined, reset them first" << endl;
                break;
            }
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
        {
            if (b.isJoined())
            {
                cout << "B is already joined to A, reset first" << endl;
                break;
            }
            int k = readInt("Enter k: ");
            if (k < 1)
            {
                cout << "k must be at least 1" << endl;
                break;
            }
            if (makeIntersection(a, b, k))
            {
                cout << "Intersection created" << endl;
                cout << "A: ";
                a.display();
                cout << "B: ";
                b.display();
            }
            else
            {
                cout << "Could not create it, A has fewer than " << k << " nodes" << endl;
            }
            break;
        }
        case 5:
            resetBoth(a, b);
            cout << "Both lists are empty now" << endl;
            break;
        }
    }
    cout << "Bye" << endl;
    return 0;
}
