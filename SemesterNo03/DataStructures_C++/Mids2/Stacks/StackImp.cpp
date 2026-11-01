// #include <iostream>
// #include <stack> //Later on implementation
// #include <string>
// using namespace std;

// // array based stack implementation:

// class Stack
// {
// private:
//     int capacity;
//     int top;
//     int *arr;

// public:
//     Stack()
//     {
//         capacity = 10;
//         top = -1;
//         arr = new int[capacity];
//     }

//     Stack(int capacity)
//     {
//         this->capacity = capacity;
//         top = -1;
//         arr = new int[capacity];
//     }

//     bool isFull()
//     {
//         return ((top + 1) == capacity);
//     }

//     bool isEmpty()
//     {
//         return (top == -1);
//     }

//     void push(int value)
//     {
//         if (!isFull())
//         {
//             top++;
//             arr[top] = value;
//         }
//         else
//         {
//             cerr << "Stack Overflow" << endl;
//         }
//     }

//     int pop()
//     {
//         if (isEmpty())
//         {
//             cerr << "Stack Underflow";
//             return;
//         }
//         else
//         {
//             int temp = arr[top];
//             top--;
//             return temp;
//         }
//     }

//     int top()
//     {
//         if (!isEmpty())
//             return arr[top];
//         else
//         {
//             cout << "Stack Underflow" << endl;
//             return -1;
//         }
//     }

//     int update(int index, int value)
//     {

//     }

//     void print()
//     {
//         for (int i = 0; i <= top; i++)
//         {
//             cout << *(arr + i) << " ";
//         }
//     }
// };

// int main()
// {
//     Stack s;
//     s.push(10);
//     s.push(20);
//     s.push(30);
//     s.print();
//     // s.pop();
//     // s.pop();
//     // s.pop();

//     cout << endl;
//     cout << s.peek();

//     ios::sync_with_stdio(false);
//     cin.tie(NULL);

//     return 0;
// }


