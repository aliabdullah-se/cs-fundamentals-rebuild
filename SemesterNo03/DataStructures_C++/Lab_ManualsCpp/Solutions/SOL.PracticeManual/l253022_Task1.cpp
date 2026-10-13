// Task 1: Union of two linked lists (sorted, distinct)
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

    // cuts the list in the middle and hands back the second half
    static Node *splitMiddle(Node *h)
    {
        Node *slow = h;
        Node *fast = h->next;
        while (fast && fast->next)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node *second = slow->next;
        slow->next = nullptr;
        return second;
    }

    static Node *mergeSorted(Node *a, Node *b)
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

    static Node *mergeSort(Node *h)
    {
        if (!h || !h->next)
            return h;
        Node *second = splitMiddle(h);
        return mergeSorted(mergeSort(h), mergeSort(second));
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

    void sort()
    {
        head = mergeSort(head);
        tail = head;
        while (tail && tail->next)
            tail = tail->next;
    }

    // only valid once the list is sorted
    void removeDuplicates()
    {
        Node *cur = head;
        while (cur && cur->next)
        {
            if (cur->data == cur->next->data)
            {
                Node *dup = cur->next;
                cur->next = dup->next;
                delete dup;
                size--;
            }
            else
            {
                cur = cur->next;
            }
        }
        tail = cur;
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

// copies every value of both lists into out, then sorts and drops repeats
void buildUnion(const LinkedList &a, const LinkedList &b, LinkedList &out)
{
    out.clear();
    for (Node *c = a.getHead(); c; c = c->next)
        out.append(c->data);
    for (Node *c = b.getHead(); c; c = c->next)
        out.append(c->data);
    out.sort();
    out.removeDuplicates();
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
        cout << "Union of two linked lists" << endl;
        cout << "1. Fill a list" << endl;
        cout << "2. Append one value to a list" << endl;
        cout << "3. Display lists" << endl;
        cout << "4. Compute union" << endl;
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
            buildUnion(a, b, result);
            cout << "Union: ";
            result.display();
            break;
        }
    }
    cout << "Bye" << endl;
    return 0;
}
