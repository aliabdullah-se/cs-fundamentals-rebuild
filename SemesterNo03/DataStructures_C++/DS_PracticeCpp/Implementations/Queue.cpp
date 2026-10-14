// #include <iostream>
// using namespace std;

// class Queue
// {
// private: // Using Arrays........ based implementations
//     int *arr;
//     int size;
//     int front, rear;
//     int count;

// public:
//     Queue(int size = 5)
//     {
//         this -> size = size;
//         arr = new int [size];
//         front = rear = -1;
//         count = 0;
//     }

// bool isFull()
// {
//     return count == size;
// }

// bool isEmpty()
// {
//     return count == 0;
// }

// void enqueue(int val)
// {
//     if(isFull())
//     {
//         cerr<<"Queue Overflow"<<endl;
//         return;
//     }
//     if(front == -1)
//     {
//         front = 0;
//     }
//     rear = (rear + 1) % size;
//     arr[rear] = val;
//     count++;
// }

// void dequeue()
// {
//     if(isEmpty())
//     {
//         cerr<<"Queue UnderFlow"<<endl;
//         return;
//     }
//     front = (front + 1) % size;
//     count--;
// }
// void Print() const
// {
//     if(isEmpty())
//     {
//         cerr<<"Queue Underflow"<<endl;
//         return;
//     }
//     cout<<"Queue : [";
//     int index = front;
//     for(int i = 1; i<count; i++)
//     {
//       cout<< arr[index]<<" , ";
//       index = (index + 1) % size;
//     }
//     cout<< arr[index]<< "]"<<endl;
// }
// ~Queue()
// {
//     delete [] arr;
// }

// };

// int main()
// {
//     ios::sync_with_stdio(false);
//     cin.tie(NULL);

//     return 0;
// }

//

// #include <iostream>
// using namespace std;

// class Queue
// {
// private:
//     int *arr;
//     int size, front, rear;
//     int count;

// public:
//     Queue(int size = 5)
//     {
//         this->size = size;
//         arr = new int[size];
//         front = rear = -1;
//         count = 0;
//     }

//     bool isEmpty() const
//     {
//         if (count == 0)
//             return 1;
//         return 0;
//     }

//     bool isFull()
//     {
//         if(count == size)
//         return true;
//         return false;
//     }

//     void enqueue(int data)
//     {
//         if(isFull())
//         {
//             cerr<<"Queue OverFlow"<<endl;
//             return;
//         }
//         if(front == -1)
//         {
//             front = 0;
//         }
//         rear = (rear + 1) % size;
//         arr[rear] = data;
//         count ++;
//     }

//     void dequeue()
//     {
//         if(isEmpty())
//         {
//             cerr<<"Queue Underflow"<<endl;
//             return;
//         }
//         front = (front + 1) % size;
//         count --;
//     }

//     void print() const
//     {
//       if(isEmpty())
//       {
//         cerr<<"Queue UnderFlow"<<endl;
//         return;
//       }
//       cout<<"Queue [";
//       int index = front;
//       for(int i = 1; i < count; i++)
//       {
//         cout<<arr[index] <<" , ";
//         index = (index + 1)%size;
//       }
//        cout<<arr[index] <<"]"<<endl;
//     }

//     ~Queue()
//     {
//         delete[] arr;
//     }
// };

// int main()
// {
//     Queue q1;
//     ios::sync_with_stdio(false);
//     cin.tie(NULL);

//     return 0;
// }




// LinkedList Implementation
#include <iostream>
using namespace std;

class Queue
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

        Node(int data, Node *next = NULL)
        {
            this->data = data;
            this->next = next;
        }
    };

    Node *front;
    Node *rear;

public:
    Queue()
    {
        front = rear = NULL;
    }
    bool isEmpty() const
    {
        return front == NULL;
    }

    void enqueue(int data)
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
    void dequeue()
    {
        if (isEmpty())
        {
            cerr << "Queue UnderFlow" << endl;
            return;
        }
        if (front == rear)
        {
            delete front;
            front = rear = NULL;
            return;
        }
        Node *temp = front;
        front = front->next;
        delete temp;
    }

    void display() const
    {
        if (isEmpty())
        {
            cerr << "Queue UnderFlow" << endl;
            return;
        }
        Node *temp = front;
        cout << "Queue [";
        while (temp != rear)
        {
            cout << temp->data << " , ";
            temp = temp->next;
        }
        cout << temp->data << "]\n"; 
    }
};

int main()
{
    Queue q1;
    
    ios::sync_with_stdio(false);
    cin.tie(NULL);
   
    return 0;
}