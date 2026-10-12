// #include <iostream>
// using namespace std;

// class SinglyLinkedList
// {

//     class Node
//     {
//     public:
//         int data;
//         Node *next;
//         Node()
//         {
//             data = 0;
//             next = NULL;
//         }

//         Node(int data)
//         {
//             this->data = data;
//             next = NULL;
//         }
//     };

// private:
//     Node *head;
//     Node *tail;

// public:
//     SinglyLinkedList()
//     {
//         head = tail = NULL;
//     }

//     void push_front(int data) // O(1)
//     {
//         Node *newNode = new Node(data);
//         if (head == NULL)
//         {
//             head = tail = newNode;
//         }
//         else
//         {
//             newNode->next = head;
//             head = newNode;
//         }
//     }

//     void push_back(int data) // O(n)
//     {
//         Node *newNode = new Node(data);

//         if (head == NULL)
//         {
//             head = tail = newNode;
//         }
//         else
//         {
//             Node *temp = head;

//             while (temp->next != NULL)
//             {
//                 temp = temp->next;
//             }
//             temp->next = newNode;
//         }
//     }
//     void pop_front(int target)
//     {
//         if (head == NULL)
//         {
//             cout << "List is Empty " << endl;
//             return;
//         }
//         Node *temp = head;
//         head = head->next;
//         temp->next = NULL;

//         delete temp;
//     }

//     void pop_back()
//     {
//         Node *temp = head;
//         if(head == NULL)
//         {
//             cout<<"Empty Nothing to delete"<<endl;
//         }
//         else
//         {
//         while(temp->next->next!=NULL)
//         {
//             temp = temp -> next;

//         }
//          delete temp;

//         }
//     }

//     void print_list()
//     {
//         if (head == NULL)
//         {
//             cout << "List is Empty" << endl;
//         }
//         else
//         {
//             Node *temp = head;

//             while (temp != NULL)
//             {
//                 cout << temp->data << " -> ";
//                 temp = temp->next;
//             }
//         }
//         cout << " NULL" << endl;
//     }
// };
// int main()
// {

//     ios::sync_with_stdio(false);
//     cin.tie(NULL);

//     SinglyLinkedList li;
//     li.push_front(20);
//     li.push_front(30);
//     li.print_list();

//     return 0;
// }

// Linked List New Implementation:

// #include <iostream>
// using namespace std;

// class LinkedList
// {

//     class Node
//     {
//     public:
//         int data;
//         Node *next;

//         Node()
//         {
//             data = 0;
//             next = NULL;
//         }
//         Node(int data, Node *next = NULL)
//         {
//             this->data = data;
//             this->next = next;
//         }
//     };

//     Node *head;
//     Node *tail;

// public:
//     LinkedList()
//     {
//         head = tail = NULL;
//     }

//     void push_front(int data)
//     {
//         Node *newNode = new Node(data);
//         if (head == NULL)
//         {
//             head = tail = newNode;
//             return;
//         }
//         else{
//         newNode->next = head;
//         head = newNode;}
//     }

//     void push_back(int data)
//     {
//         Node *newNode = new Node(data);
//         if (head == NULL)
//         {
//             head = tail = newNode;
//             return;
//         }
//         else{
//         tail->next = newNode;
//         tail = newNode;
//         }
//     }

//     void pop_front()
//     {
//         if(head == NULL)
//         {
//             cout<<"LL is Empty \n"<<endl;
//         }

//     }

//     void print_list()
//     {
//         if (head == NULL)
//         {
//             cout << head->data;
//         }
//         Node *curr = head;
//         while (curr != tail)
//         {
//             cout << curr->data << " -> ";
//             curr = curr->next;
//         }
//         cout << "NULL" << endl;
//     }
// };

// int main()
// {
//     ios::sync_with_stdio(false);
//     cin.tie(NULL);

//     return 0;
// }

// Linked List Implementation From Absolute Scratch : (Again :( )

#include <iostream>
using namespace std;

class SinglyLinkedList // Singly Linked List Wrapper Implementation
{
public:
    class node // Node Class Wrapper Implementation
    {
    public:
        int data;
        node *next;

        node()
        {
            data = 0;
            next = NULL;
        }
        node(int data)
        {
            this->data = data;
            next = NULL;
        }
    };

    node *head;
    node *tail;

    SinglyLinkedList()
    {
        head = NULL;
        tail = NULL;
    }

    // This is a Push_Front Function;

    void push_front(int value) // O(1)
    {
        node *front = new node(value);

        if (head == NULL)
        {
            head = front;
            tail = front;
            return;
        }
        else
        {
            front->next = head;
            head = front;
        }
    }

    // Push back implemented but with only using the head pointer as a slider(temp)

    // void push_back_test(int value) // O(n)
    // {
    //   node* back = new node (value);

    //   if(head == NULL)
    //   {
    //     head = back;
    //     return;
    //   }
    //   else
    //   {
    //     node* temp = head;
    //     while (temp -> next != NULL)
    //     {
    //         temp = temp -> next;
    //     }
    //     temp -> next = back;
    //   }
    // }

    // Push back implementation:

    void push_back(int value) // O(1) assuming the tail ptr
    {
        node *rear = new node(value);

        if (head == NULL)
        {
            head = rear;
            tail = rear;
            return;
        }
        else
        {
            tail->next = rear;
            tail = rear;
        }
    }

    void pop_front()
    {
        if(head == NULL)
        {
            cout<<"List is Empty"<<endl;
            return;
        }
        else
        {
        node* temp = head;
        head = head -> next;
        temp -> next = NULL;  
        delete temp;

        }
    }

    void pop_back()
    {
        if(head == NULL)
        {
            cout<<"List is Empty"<<endl;
            return;
        }
        else
        {
           node* temp = head;
           while(temp->next != tail)
           {
            temp = temp -> next;
           }
           temp -> next = NULL;
           delete tail;
           tail = temp;
           
           
        }
    }

    void print_list()
    {
        node *temp = head;
        while (temp != NULL)
        {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL ";
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    SinglyLinkedList L1;

    L1.push_front(10);
    L1.push_back(20);
    L1.push_front(30);
    L1.push_front(40);
    L1.push_back(90);
    L1.push_back(100);
    L1.print_list();

    L1.pop_front();
    L1.pop_back();

    system("pause");
    return 0;
}
