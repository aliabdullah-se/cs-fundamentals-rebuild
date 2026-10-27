#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class Node
{
public:
    int data;
    Node *next;
    Node *random; // can point to any node in the list, or to nullptr

    Node(int val) : data(val), next(nullptr), random(nullptr) {}
};

// The idea here is to avoid a hashmap completely. Instead we temporarily
// weave a clone node right after every original node, use that weave to
// figure out where each clone's random pointer should go, and then
// separate the two lists back apart. Since every step only touches a
// constant number of pointers per node, this ends up being O(n) time
// and O(1) extra space (not counting the new nodes we have to create
// anyway for the answer).

Node *cloneRandomList(Node *head)
{
    if (head == nullptr)
        return nullptr;

    // Step 1: insert a clone right  after each o riginal node.
    // A -> B -> C  becomes  A -> A' -> B -> B' -> C -> C'
    Node *curr = head;
    while (curr != nullptr)
    {
        Node *cloneNode = new Node(curr->data);
        cloneNode->next = curr->next;
        curr->next = cloneNode;
        curr = cloneNode->next;
    }

    // Step 2: fix the random pointers. Since curr's clone is just
    // curr->next, and curr->random's clone is curr->random->next,
    // we don't need to search for anything.
    curr = head;
    while (curr != nullptr)
    {
        if (curr->random != nullptr)
        {
            curr->next->random = curr->random->next;
        }
        curr = curr->next->next;
    }

    // Step 3: split the combined list back into the original list and
    // the clone list, restoring both to how they should look separately.
    curr = head;
    Node *cloneHead = head->next;
    Node *cloneCurr = cloneHead;

    while (curr != nullptr)
    {
        curr->next = curr->next->next;

        if (cloneCurr->next != nullptr)
        {
            cloneCurr->next = cloneCurr->next->next;
        }

        curr = curr->next;
        cloneCurr = cloneCurr->next;
    }

    return cloneHead;
}

// Just prints the list so we can visually check the clone looks right. // a visualization of linked list
void printList(Node *head, const string &label)
{
    cout << label << ": ";
    Node *curr = head;
    while (curr != nullptr)
    {
        cout << curr->data << "(";
        if (curr->random != nullptr)
        {
            cout << curr->random->data;
        }
        else
        {
            cout << "NULL";
        }
        cout << ")";
        if (curr->next != nullptr)
            cout << " -> ";
        curr = curr->next;
    }
    if (head == nullptr)
        cout << "(empty)";
    cout << endl;
}

// Walks both lists side by side and checks that the clone is a real,
// independent deep copy: no shared node addresses, matching data, and
// random pointers that mirror the original without pointing back into it.
void verifyClone(Node *origHead, Node *cloneHead)
{
    Node *o = origHead;
    Node *c = cloneHead;
    bool ok = true;
    int index = 0;

    while (o != nullptr && c != nullptr)
    {
        if (o == c)
        {
            cout << "  FAIL at index " << index << ": clone node shares address with original." << endl;
            ok = false;
        }

        if (o->data != c->data)
        {
            cout << "  FAIL at index " << index << ": data mismatch (" << o->data
                 << " vs " << c->data << ")." << endl;
            ok = false;
        }

        if ((o->random == nullptr) != (c->random == nullptr))
        {
            cout << "  FAIL at index " << index << ": random NULL-ness mismatch." << endl;
            ok = false;
        }
        else if (o->random != nullptr)
        {
            if (o->random->data != c->random->data)
            {
                cout << "  FAIL at index " << index << ": random target data mismatch." << endl;
                ok = false;
            }
            if (c->random == o->random)
            {
                cout << "  FAIL at index " << index << ": clone's random still points into original list." << endl;
                ok = false;
            }
        }

        o = o->next;
        c = c->next;
        index++;
    }

    if (o != nullptr || c != nullptr)
    {
        cout << "  FAIL: lists have different lengths." << endl;
        ok = false;
    }

    if (ok)
        cout << "  PASS: clone is a correct, fully independent deep copy." << endl;
}

// Cleans up a list node by node so we don't leak memory after testing.
void freeList(Node *head)
{
    while (head != nullptr)
    {
        Node *nxt = head->next;
        delete head;
        head = nxt;
    }
}

void test1_GeneralCase()
{
    cout << endl
         << " TEST 1: General case (5 nodes) " << endl;
    Node *n0 = new Node(1);
    Node *n1 = new Node(2);
    Node *n2 = new Node(3);
    Node *n3 = new Node(4);
    Node *n4 = new Node(5);

    n0->next = n1;
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = nullptr;

    n0->random = n2;
    n1->random = n0;
    n2->random = n4;
    n3->random = n3; // this one points to itself, on purpose
    n4->random = n1;

    printList(n0, "Original before clone");

    Node *clone = cloneRandomList(n0);

    printList(n0, "Original after clone (must be unchanged)");
    printList(clone, "Clone");
    verifyClone(n0, clone);

    freeList(n0);
    freeList(clone);
}

void test2_AllRandomNull()
{
    cout << endl
         << " TEST 2: All random = NULL " << endl;
    Node *n0 = new Node(10);
    Node *n1 = new Node(20);
    Node *n2 = new Node(30);
    n0->next = n1;
    n1->next = n2;
    n2->next = nullptr;
    // random pointers are left as nullptr from the constructor

    printList(n0, "Original before clone");
    Node *clone = cloneRandomList(n0);
    printList(n0, "Original after clone (must be unchanged)");
    printList(clone, "Clone");
    verifyClone(n0, clone);

    freeList(n0);
    freeList(clone);
}

void test3_RandomPointsToSelf()
{
    cout << endl
         << " TEST 3: Single node, random -> self " << endl;
    Node *n0 = new Node(99);
    n0->next = nullptr;
    n0->random = n0; // this node's random points back to itself

    printList(n0, "Original before clone");
    Node *clone = cloneRandomList(n0);
    printList(n0, "Original after clone (must be unchanged)");
    printList(clone, "Clone");

    // extra check just for this case: the clone's random should point
    // to itself too, not back to the original node
    if (clone->random == clone)
    {
        cout << "  PASS: clone's random correctly points to itself (not to original)." << endl;
    }
    else
    {
        cout << "  FAIL: clone's random does not correctly point to itself." << endl;
    }
    verifyClone(n0, clone);

    freeList(n0);
    freeList(clone);
}

void test4_EmptyList()
{
    cout << endl
         << " TEST 4: Empty list (head = NULL) " << endl;
    Node *head = nullptr;
    Node *clone = cloneRandomList(head);
    if (clone == nullptr)
    {
        cout << "  PASS: cloning an empty list correctly returns NULL." << endl;
    }
    else
    {
        cout << "  FAIL: expected NULL for an empty list." << endl;
    }
}

int main()
{

    ios::sync_with_stdio(false);
    cin.tie(NULL);

    //  The Final Time Complexity for the Solution Algorithm is :
    //  Time Complexity: O(N)  three separate passes over the list,
    //  each doing constant work per node.
    //  Space Complexity: O(1) extra no hash map or array is used;
    //  only a fixed number of pointer variables regardless of list size.

    test1_GeneralCase();
    test2_AllRandomNull();
    test3_RandomPointsToSelf();
    test4_EmptyList();


    return 0;

  
}