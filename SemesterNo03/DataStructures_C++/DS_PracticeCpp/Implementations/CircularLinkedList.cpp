// This is a program to implement the circular linked list:
#include <iostream>
using namespace std;

class SinglyCircularLinkedList
{
public:
    class node
    {
    public:
        string data;
        node *next;
        node()
        {
            data = "";
            next = NULL;
        }
        node(string data)
        {
            this->data = data;
            next = NULL;
        }
    };

    node *head;
    node *tail;

    SinglyCircularLinkedList()
    {
        head = tail = NULL;
    }

    // push from front function:

    void push_front(string value)
    {
        node *newNode = new node(value);

        if (head == NULL)
        {
            head = tail = newNode;
            return;
        }
        else
        {
            newNode->next = head;
            head = newNode;
        }
    }

    // Push Back Function

    void push_back(string value)
    {
        node *newNode = new node(value);
        if (head == NULL)
        {
            head = tail = newNode;
            return;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Pop_front function implementation:

    void pop_front()
    {
        if (head == NULL)
        {
            cout << "LinkedList is Empty Nothing to Delete" << endl;
            return;
        }
        if (head == tail)
        {
            delete head;
            head = tail = NULL;
            return;
        }
        node *temp = head;
        head = head->next;
        temp->next = NULL;
        delete temp;
    }

    // Pop_back function implementation:

    void pop_back()
    {
        if (head == NULL)
        {
            cout << "LinkedList is Empty Nothing to Delete" << endl;
            return;
        }
        if (head == tail)
        {
            delete head;
            head = tail = NULL;
            return;
        }
        node *temp = head;
        while (temp->next != tail)
        {
            temp = temp->next;
        }
        delete tail;
        tail = temp;
        tail->next = NULL;
    }

    //To Be implemented in University;

    void insertAt(string value, int pos);
    int search_list(string value);
    

    // Printing the list:
    void print_list()
    {
        node *temp = head;
        while (temp != NULL)
        {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    // object of LL:

    SinglyCircularLinkedList SCL;

    // Test Cases to A Singly circular LinkedList:
    // 1. Creation by Push_front:
    cout << "Push Front: " << endl;

    SCL.push_front("Ali");
    SCL.push_front("Ahmed");
    SCL.push_front("Abdullah");
    SCL.print_list();

    cout << endl;
    cout << "Push Back: " << endl;

    SCL.push_back("Hiba");
    SCL.push_back("Tahreem");
    SCL.push_back("Sana");
    SCL.print_list();

    cout << endl;
    cout << "Pop front: " << endl;
    SCL.pop_front();
    SCL.print_list();

    cout << endl;
    cout << "Pop back: " << endl;
    SCL.pop_back();
    SCL.print_list();

    cout << endl;
    cout << "Insert At Index: " << endl;
    SCL.insertAt("Amir" , 3);
    SCL.print_list();
    
    cout << endl;
    
    int searchedValue = SCL.search_list("Ali");
    if(searchedValue == -1)
    {
        cout<<"Name Not Found"<<endl;
    }
    else
    {
        cout<<"Name is Found"<<endl;
    }



    system("pause");
    return 0;
}