// =====================================================================
//  DATA STRUCTURES MIDTERM 1 - REFERENCE IMPLEMENTATIONS (plain C++, no STL)
//  Style matches your own code: nested Node class, NULL, head/tail pointers.
//
//  THE 5 RULES BEHIND EVERY FUNCTION IN THIS FILE
//   1. Empty list first:        if (head == NULL) ...
//   2. Single node second:      if (head == tail) ...      (all pop functions)
//   3. Link first, move after:  connect the new node BEFORE moving head/tail,
//                               re-link the neighbours BEFORE deleting a node.
//   4. Where does the loop stop?
//        while (temp != NULL)         -> visits every node, ends PAST the last
//        while (temp->next != NULL)   -> ends ON the last node
//        while (temp->next != tail)   -> ends on the node BEFORE tail
//   5. After every change ask: is head still right? is tail still right?
//
//  CONTENTS
//   1. SinglyLinkedList (+ reverse, middle, iterator)
//   2. DoublyLinkedList (a Deque is just this with push/pop at both ends)
//   3. CircularLinkedList
//   4. ArrayStack and LinkedStack
//   5. ArrayQueue (circular) and LinkedQueue
//   6. Stack applications: balanced brackets, infix->postfix, postfix eval
// =====================================================================
#include <iostream>
using namespace std;
 
// =====================================================================
// 1. SINGLY LINKED LIST
// =====================================================================
class SinglyLinkedList
{
    class Node
    {
    public:
        int data;
        Node *next;
 
        Node()
        {
            data = 0;
            next = NULL;
        }
        Node(int data)
        {
            this->data = data;
            next = NULL;
        }
    };
 
    Node *head;
    Node *tail;
 
public:
    // Iterator: a small object that behaves like a pointer into the list
    class Iterator
    {
        Node *ptr;
 
    public:
        Iterator(Node *p = NULL) { ptr = p; }
        int &operator*() { return ptr->data; }                          // *it gives the data
        Iterator &operator++()                                          // ++it moves to next node
        {
            ptr = ptr->next;
            return *this;
        }
        bool operator!=(const Iterator &other) const { return ptr != other.ptr; }
        bool operator==(const Iterator &other) const { return ptr == other.ptr; }
    };
 
    Iterator begin() { return Iterator(head); }   // first node
    Iterator end() { return Iterator(NULL); }     // one PAST the last node
 
    SinglyLinkedList() { head = tail = NULL; }
 
    void push_front(int value) // O(1)
    {
        Node *newNode = new Node(value);
        if (head == NULL)
        {
            head = tail = newNode;
        }
        else
        {
            newNode->next = head; // link first
            head = newNode;       // move after
        }
    }
 
    void push_back(int value) // O(1) because of tail
    {
        Node *newNode = new Node(value);
        if (head == NULL)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode; // link first
            tail = newNode;       // move after
        }
    }
 
    void pop_front() // O(1)
    {
        if (head == NULL)
        {
            cout << "List is empty" << endl;
            return;
        }
        Node *temp = head;
        if (head == tail) // single node
            head = tail = NULL;
        else
            head = head->next;
        delete temp;
    }
 
    void pop_back() // O(n): must reach the node BEFORE tail
    {
        if (head == NULL)
        {
            cout << "List is empty" << endl;
            return;
        }
        if (head == tail) // single node
        {
            delete head;
            head = tail = NULL;
            return;
        }
        Node *temp = head;
        while (temp->next != tail)
            temp = temp->next;
        delete tail;
        tail = temp;
        tail->next = NULL;
    }
 
    // pos is 1-indexed: the new node BECOMES position pos (1 = front)
    void insertAt(int value, int pos) // O(n)
    {
        if (pos < 1)
        {
            cout << "Invalid position" << endl;
            return;
        }
        if (pos == 1)
        {
            push_front(value); // also handles the empty list
            return;
        }
        Node *temp = head;
        for (int i = 1; i < pos - 1 && temp != NULL; i++) // stop ON node pos-1
            temp = temp->next;
        if (temp == NULL)
        {
            cout << "Invalid position" << endl;
            return;
        }
        if (temp == tail) // inserting after the last node = push_back
        {
            push_back(value);
            return;
        }
        Node *newNode = new Node(value);
        newNode->next = temp->next; // link first (new node -> the rest)
        temp->next = newNode;       // then (before -> new node)
    }
 
    void remove(int key) // delete the first node holding key, O(n)
    {
        if (head == NULL)
        {
            cout << "List is empty" << endl;
            return;
        }
        if (head->data == key)
        {
            pop_front();
            return;
        }
        Node *prev = head;
        Node *curr = head->next;
        while (curr != NULL && curr->data != key)
        {
            prev = curr;
            curr = curr->next;
        }
        if (curr == NULL)
        {
            cout << "Key not found" << endl;
            return;
        }
        prev->next = curr->next; // bypass curr
        if (curr == tail)
            tail = prev; // deleted the last node -> tail moves back
        delete curr;
    }
 
    int search(int key) // returns POSITION (1 = first node), -1 if absent. O(n)
    {
        int pos = 1;
        Node *temp = head;
        while (temp != NULL)
        {
            if (temp->data == key)
                return pos;
            temp = temp->next;
            pos++;
        }
        return -1;
    }
 
    void reverse() // O(n) time, O(1) space - THE classic
    {
        Node *prev = NULL;
        Node *curr = head;
        tail = head; // old head becomes the new tail
        while (curr != NULL)
        {
            Node *nxt = curr->next; // 1. save the rest of the list
            curr->next = prev;      // 2. flip this arrow backwards
            prev = curr;            // 3. advance prev
            curr = nxt;             // 4. advance curr
        }
        head = prev; // prev ends on the old last node
    }
 
    int middle() // slow/fast pointers, O(n). Even length -> second middle
    {
        if (head == NULL)
            return -1;
        Node *slow = head;
        Node *fast = head;
        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;       // 1 step
            fast = fast->next->next; // 2 steps
        }
        return slow->data;
    }
 
    void print_list()
    {
        Node *temp = head;
        while (temp != NULL)
        {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl; // printed ONCE, after the loop
    }
 
    ~SinglyLinkedList()
    {
        while (head != NULL)
            pop_front();
    }
};
 
// =====================================================================
// 2. DOUBLY LINKED LIST   (every node has prev AND next)
// =====================================================================
class DoublyLinkedList
{
    class Node
    {
    public:
        int data;
        Node *next;
        Node *prev;
 
        Node()
        {
            data = 0;
            next = prev = NULL;
        }
        Node(int data)
        {
            this->data = data;
            next = prev = NULL;
        }
    };
 
    Node *head;
    Node *tail;
 
public:
    DoublyLinkedList() { head = tail = NULL; }
 
    void push_front(int value) // O(1)
    {
        Node *newNode = new Node(value);
        if (head == NULL)
        {
            head = tail = newNode;
        }
        else
        {
            newNode->next = head; // new -> old head
            head->prev = newNode; // old head <- new
            head = newNode;
        }
    }
 
    void push_back(int value) // O(1)
    {
        Node *newNode = new Node(value);
        if (head == NULL)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode; // old tail -> new
            newNode->prev = tail; // old tail <- new
            tail = newNode;
        }
    }
 
    void pop_front() // O(1)
    {
        if (head == NULL)
        {
            cout << "List is empty" << endl;
            return;
        }
        Node *temp = head;
        if (head == tail)
        {
            head = tail = NULL;
        }
        else
        {
            head = head->next;
            head->prev = NULL; // new head has nothing behind it
        }
        delete temp;
    }
 
    void pop_back() // O(1) - no traversal, tail->prev already knows the way
    {
        if (head == NULL)
        {
            cout << "List is empty" << endl;
            return;
        }
        Node *temp = tail;
        if (head == tail)
        {
            head = tail = NULL;
        }
        else
        {
            tail = tail->prev;
            tail->next = NULL; // new tail has nothing ahead of it
        }
        delete temp;
    }
 
    void remove(int key) // O(n) to find, O(1) to unlink
    {
        Node *temp = head;
        while (temp != NULL && temp->data != key)
            temp = temp->next;
        if (temp == NULL)
        {
            cout << "Key not found" << endl;
            return;
        }
        if (temp == head)
        {
            pop_front();
            return;
        }
        if (temp == tail)
        {
            pop_back();
            return;
        }
        temp->prev->next = temp->next; // node before skips temp
        temp->next->prev = temp->prev; // node after skips temp
        delete temp;
    }
 
    void print_forward()
    {
        Node *temp = head;
        cout << "NULL <-> ";
        while (temp != NULL)
        {
            cout << temp->data << " <-> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }
 
    void print_backward()
    {
        Node *temp = tail;
        cout << "NULL <-> ";
        while (temp != NULL)
        {
            cout << temp->data << " <-> ";
            temp = temp->prev;
        }
        cout << "NULL" << endl;
    }
 
    ~DoublyLinkedList()
    {
        while (head != NULL)
            pop_front();
    }
};
 
// =====================================================================
// 3. CIRCULAR LINKED LIST   (invariant: tail->next == head, ALWAYS)
// =====================================================================
class CircularLinkedList
{
    class Node
    {
    public:
        int data;
        Node *next;
 
        Node()
        {
            data = 0;
            next = NULL;
        }
        Node(int data)
        {
            this->data = data;
            next = NULL;
        }
    };
 
    Node *head;
    Node *tail;
 
public:
    CircularLinkedList() { head = tail = NULL; }
 
    void push_front(int value) // O(1)
    {
        Node *newNode = new Node(value);
        if (head == NULL)
        {
            head = tail = newNode;
            newNode->next = head; // a single node points to itself
        }
        else
        {
            newNode->next = head;
            head = newNode;
            tail->next = head; // repair the circle
        }
    }
 
    void push_back(int value) // O(1)
    {
        Node *newNode = new Node(value);
        if (head == NULL)
        {
            head = tail = newNode;
            newNode->next = head;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
            tail->next = head; // repair the circle
        }
    }
 
    void pop_front() // O(1)
    {
        if (head == NULL)
        {
            cout << "List is empty" << endl;
            return;
        }
        Node *temp = head;
        if (head == tail)
        {
            head = tail = NULL;
        }
        else
        {
            head = head->next;
            tail->next = head; // repair the circle
        }
        delete temp;
    }
 
    void pop_back() // O(n): need the node BEFORE tail
    {
        if (head == NULL)
        {
            cout << "List is empty" << endl;
            return;
        }
        if (head == tail)
        {
            delete head;
            head = tail = NULL;
            return;
        }
        Node *temp = head;
        while (temp->next != tail)
            temp = temp->next;
        delete tail;
        tail = temp;
        tail->next = head; // repair the circle
    }
 
    void print_list()
    {
        if (head == NULL)
        {
            cout << "List is empty" << endl;
            return;
        }
        Node *temp = head;
        do // do-while: a plain while (temp != head) would stop before printing anything
        {
            cout << temp->data << " -> ";
            temp = temp->next;
        } while (temp != head);
        cout << "(back to head)" << endl;
    }
 
    ~CircularLinkedList()
    {
        while (head != NULL)
            pop_front();
    }
};
 
// =====================================================================
// 4a. STACK - ARRAY BASED   (LIFO: last in, first out)
// =====================================================================
class ArrayStack
{
    int *arr;
    int capacity;
    int topIndex; // index of the top element, -1 when empty
 
public:
    ArrayStack(int capacity = 5)
    {
        this->capacity = capacity;
        arr = new int[capacity];
        topIndex = -1;
    }
 
    bool isEmpty() const { return topIndex == -1; }
    bool isFull() const { return topIndex == capacity - 1; }
 
    void push(int value) // O(1)
    {
        if (isFull())
        {
            cout << "Stack Overflow" << endl;
            return;
        }
        arr[++topIndex] = value; // move up first, then store
    }
 
    void pop() // O(1)
    {
        if (isEmpty())
        {
            cout << "Stack Underflow" << endl;
            return;
        }
        topIndex--; // the old value is simply ignored from now on
    }
 
    int peek() const // O(1)
    {
        if (isEmpty())
        {
            cout << "Stack is empty" << endl;
            return -1;
        }
        return arr[topIndex];
    }
 
    void display() const // top -> bottom
    {
        cout << "Stack (top first): ";
        for (int i = topIndex; i >= 0; i--)
            cout << arr[i] << " ";
        cout << endl;
    }
 
    ~ArrayStack() { delete[] arr; }
};
 
// =====================================================================
// 4b. STACK - LINKED LIST BASED   (push = push_front, pop = pop_front)
// =====================================================================
class LinkedStack
{
    class Node
    {
    public:
        int data;
        Node *next;
 
        Node(int data)
        {
            this->data = data;
            next = NULL;
        }
    };
 
    Node *top;
 
public:
    LinkedStack() { top = NULL; }
 
    bool isEmpty() const { return top == NULL; }
 
    void push(int value) // O(1)
    {
        Node *newNode = new Node(value);
        newNode->next = top; // link first
        top = newNode;       // move after
    }
 
    void pop() // O(1)
    {
        if (isEmpty())
        {
            cout << "Stack Underflow" << endl;
            return;
        }
        Node *temp = top;
        top = top->next;
        delete temp;
    }
 
    int peek() const // O(1)
    {
        if (isEmpty())
        {
            cout << "Stack is empty" << endl;
            return -1;
        }
        return top->data;
    }
 
    void display() const // top -> bottom
    {
        cout << "Stack (top first): ";
        Node *temp = top;
        while (temp != NULL)
        {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
 
    ~LinkedStack()
    {
        while (!isEmpty())
            pop();
    }
};
 
// =====================================================================
// 5a. QUEUE - CIRCULAR ARRAY   (FIFO: first in, first out)
// =====================================================================
class ArrayQueue
{
    int *arr;
    int size, front, rear;
    int count; // number of stored elements: tells full from empty
 
public:
    ArrayQueue(int size = 5)
    {
        this->size = size;
        arr = new int[size];
        front = rear = -1;
        count = 0;
    }
 
    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == size; }
 
    void enqueue(int data) // O(1)
    {
        if (isFull())
        {
            cout << "Queue Overflow" << endl;
            return;
        }
        if (front == -1)
            front = 0;
        rear = (rear + 1) % size; // the % wraps rear back to index 0
        arr[rear] = data;
        count++;
    }
 
    void dequeue() // O(1)
    {
        if (isEmpty())
        {
            cout << "Queue Underflow" << endl;
            return;
        }
        front = (front + 1) % size;
        count--;
    }
 
    int peek() const // front element
    {
        if (isEmpty())
        {
            cout << "Queue is empty" << endl;
            return -1;
        }
        return arr[front];
    }
 
    void display() const
    {
        if (isEmpty())
        {
            cout << "Queue is empty" << endl;
            return;
        }
        cout << "Queue [";
        int index = front;
        for (int i = 1; i < count; i++)
        {
            cout << arr[index] << " , ";
            index = (index + 1) % size;
        }
        cout << arr[index] << "]" << endl;
    }
 
    ~ArrayQueue() { delete[] arr; }
};
 
// =====================================================================
// 5b. QUEUE - LINKED LIST   (enqueue = push_back, dequeue = pop_front)
// =====================================================================
class LinkedQueue
{
    class Node
    {
    public:
        int data;
        Node *next;
 
        Node(int data)
        {
            this->data = data;
            next = NULL;
        }
    };
 
    Node *front;
    Node *rear;
 
public:
    LinkedQueue() { front = rear = NULL; }
 
    bool isEmpty() const { return front == NULL; }
 
    void enqueue(int data) // O(1)
    {
        Node *newNode = new Node(data);
        if (isEmpty())
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }
 
    void dequeue() // O(1)
    {
        if (isEmpty())
        {
            cout << "Queue Underflow" << endl;
            return;
        }
        Node *temp = front;
        if (front == rear)
            front = rear = NULL; // queue became empty: BOTH pointers reset
        else
            front = front->next;
        delete temp;
    }
 
    int peek() const
    {
        if (isEmpty())
        {
            cout << "Queue is empty" << endl;
            return -1;
        }
        return front->data;
    }
 
    void display() const
    {
        if (isEmpty())
        {
            cout << "Queue is empty" << endl;
            return;
        }
        Node *temp = front;
        cout << "Queue [";
        while (temp != rear)
        {
            cout << temp->data << " , ";
            temp = temp->next;
        }
        cout << temp->data << "]" << endl;
    }
 
    ~LinkedQueue()
    {
        while (!isEmpty())
            dequeue();
    }
};
 
// =====================================================================
// 6. STACK APPLICATIONS  (they reuse ArrayStack above)
// =====================================================================
 
// Balanced brackets: push every opener; every closer must match the top.
bool isBalanced(const char *s)
{
    ArrayStack st(100);
    for (int i = 0; s[i] != '\0'; i++)
    {
        char c = s[i];
        if (c == '(' || c == '{' || c == '[')
        {
            st.push(c);
        }
        else if (c == ')' || c == '}' || c == ']')
        {
            if (st.isEmpty())
                return false; // closer with nothing to match
            char open = st.peek();
            if ((c == ')' && open != '(') ||
                (c == '}' && open != '{') ||
                (c == ']' && open != '['))
                return false; // wrong kind of opener
            st.pop();
        }
    }
    return st.isEmpty(); // leftover openers = not balanced
}
 
int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0; // '(' is lowest, so no operator ever pops it
}
 
// Infix -> postfix. Operands go straight out; operators wait on the stack
// until something of equal or lower priority arrives.
void infixToPostfix(const char *infix, char *postfix)
{
    ArrayStack st(100);
    int k = 0; // write position in postfix
    for (int i = 0; infix[i] != '\0'; i++)
    {
        char c = infix[i];
        if (c == ' ')
            continue;
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            postfix[k++] = c; // operand
        }
        else if (c == '(')
        {
            st.push(c);
        }
        else if (c == ')')
        {
            while (!st.isEmpty() && st.peek() != '(')
            {
                postfix[k++] = st.peek();
                st.pop();
            }
            st.pop(); // discard the '('
        }
        else // operator
        {
            while (!st.isEmpty() && precedence(st.peek()) >= precedence(c))
            {
                postfix[k++] = st.peek();
                st.pop();
            }
            st.push(c);
        }
    }
    while (!st.isEmpty()) // flush what is left
    {
        postfix[k++] = st.peek();
        st.pop();
    }
    postfix[k] = '\0';
}
 
// Postfix evaluation (single-digit operands): operand -> push;
// operator -> pop two, apply, push the result.
int evaluatePostfix(const char *exp)
{
    ArrayStack st(100);
    for (int i = 0; exp[i] != '\0'; i++)
    {
        char c = exp[i];
        if (c >= '0' && c <= '9')
        {
            st.push(c - '0');
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            int b = st.peek(); // the SECOND operand comes off first
            st.pop();
            int a = st.peek();
            st.pop();
            if (c == '+')
                st.push(a + b);
            else if (c == '-')
                st.push(a - b);
            else if (c == '*')
                st.push(a * b);
            else
                st.push(a / b);
        }
    }
    return st.peek();
}
 
// =====================================================================
// TESTS
// =====================================================================
int main()
{
    cout << "===== SINGLY LINKED LIST =====" << endl;
    SinglyLinkedList s;
    s.push_front(10);
    s.push_back(20);
    s.push_front(30);
    s.push_back(40);
    s.print_list(); // 30 10 20 40
    s.insertAt(99, 3);
    s.print_list(); // 30 10 99 20 40
    s.insertAt(77, 6); // after the last node = append
    s.push_back(5);    // proves tail was updated
    s.print_list();    // 30 10 99 20 40 77 5
    s.insertAt(1, 20); // invalid
    s.pop_front();
    s.pop_back();
    s.print_list(); // 10 99 20 40 77
    s.remove(20);
    s.remove(77); // removing the tail
    s.push_back(8);
    s.print_list(); // 10 99 40 8
    cout << "search(40) = " << s.search(40) << ", search(5) = " << s.search(5) << endl;
    s.reverse();
    s.push_back(1);
    s.print_list(); // 8 40 99 10 1
    cout << "middle = " << s.middle() << endl; // 99
    cout << "iterator: ";
    for (SinglyLinkedList::Iterator it = s.begin(); it != s.end(); ++it)
        cout << *it << " ";
    cout << endl;
 
    cout << "\n===== DOUBLY LINKED LIST =====" << endl;
    DoublyLinkedList d;
    for (int i = 1; i <= 5; i++)
        d.push_back(i);
    d.push_front(0);
    d.print_forward();  // 0 1 2 3 4 5
    d.print_backward(); // 5 4 3 2 1 0
    d.remove(3);
    d.pop_front();
    d.pop_back();
    d.print_forward();  // 1 2 4
    d.print_backward(); // 4 2 1
 
    cout << "\n===== CIRCULAR LINKED LIST =====" << endl;
    CircularLinkedList c;
    c.push_back(1);
    c.push_back(2);
    c.push_back(3);
    c.push_front(0);
    c.print_list(); // 0 1 2 3
    c.pop_front();
    c.pop_back();
    c.print_list(); // 1 2
    c.pop_back();
    c.pop_back();
    c.print_list(); // empty
 
    cout << "\n===== STACKS =====" << endl;
    ArrayStack as(3);
    as.push(1);
    as.push(2);
    as.push(3);
    as.push(4); // overflow
    as.display();
    as.pop();
    cout << "peek = " << as.peek() << endl; // 2
    LinkedStack ls;
    ls.push(1);
    ls.push(2);
    ls.push(3);
    ls.pop();
    ls.display(); // 2 1
    cout << "peek = " << ls.peek() << endl; // 2
 
    cout << "\n===== QUEUES =====" << endl;
    ArrayQueue aq(5);
    for (int i = 1; i <= 6; i++) // the 6th overflows
        aq.enqueue(i);
    aq.dequeue();
    aq.dequeue();
    aq.enqueue(6);
    aq.enqueue(7); // wraps around
    aq.display();  // 3 4 5 6 7
    LinkedQueue lq;
    for (int i = 1; i <= 4; i++)
        lq.enqueue(i);
    lq.dequeue();
    lq.display(); // 2 3 4
    cout << "peek = " << lq.peek() << endl; // 2
 
    cout << "\n===== STACK APPLICATIONS =====" << endl;
    cout << "{[()]}  balanced? " << isBalanced("{[()]}") << endl; // 1
    cout << "{[(])}  balanced? " << isBalanced("{[(])}") << endl; // 0
    cout << "((      balanced? " << isBalanced("((") << endl;     // 0
    char post[100];
    infixToPostfix("A+B*C", post);
    cout << "A+B*C     -> " << post << endl; // ABC*+
    infixToPostfix("(A+B)*C", post);
    cout << "(A+B)*C   -> " << post << endl; // AB+C*
    infixToPostfix("A*B+C/D", post);
    cout << "A*B+C/D   -> " << post << endl; // AB*CD/+
    cout << "23*54*+9- = " << evaluatePostfix("23*54*+9-") << endl; // 17
 
    return 0;
}