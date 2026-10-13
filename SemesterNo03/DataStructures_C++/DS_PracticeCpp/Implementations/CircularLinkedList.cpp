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
        node* next; 
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

   node* head;
   node* tail;

   SinglyCircularLinkedList()
   {
    head = tail = NULL;
   }

   //push from front function:

   void push_front(string value)
   {
    node*newNode = new node(value);

      if(head == NULL)
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

   void push_back(string Value){
    
   }

    
    // Printing the list:
      void print_list()
      {
        node* temp = head;
        while(temp != NULL)
        {
            cout << temp->data <<" -> ";
            temp = temp->next; 
        }
        cout<<"NULL"<<endl;
      }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
   //object of LL:

   SinglyCircularLinkedList SCL;

   // Test Cases to A Singly circular LinkedList:
   //1. Creation by Push_front:
   SCL.push_front("Ali");
   SCL.push_front("Ahmed");
   SCL.push_front("Abdullah");
   SCL.print_list();

    
    system("pause");
    return 0;
}