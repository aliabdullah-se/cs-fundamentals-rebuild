
#include <iostream>
#include <string>

using namespace std;

// ---------------------------------------------------------------
// Input helpers
// ---------------------------------------------------------------

// reads a whole number, asks again if the typing is not a number
int readNumber(const string &prompt)
{
    int value;
    cout << prompt;
    while (!(cin >> value))
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "That was not a number, try again: ";
    }
    cin.ignore(1000, '\n'); // drop the rest of the line
    return value;
}

// same as above but the number must also sit between low and high
int readInt(const string &prompt, int low, int high)
{
    int value = readNumber(prompt);
    while (value < low || value > high)
    {
        cout << "Please enter a number between " << low << " and " << high << "." << endl;
        value = readNumber(prompt);
    }
    return value;
}

// reads a full line of text and refuses an empty answer
string readText(const string &prompt)
{
    string text;
    cout << prompt;
    getline(cin, text);
    while (text.empty())
    {
        cout << "This field cannot be empty, try again: ";
        getline(cin, text);
    }
    return text;
}

// ---------------------------------------------------------------
// Task 2: circular array queue (template so every task can reuse it)
// ---------------------------------------------------------------

template <typename T>
class CircularQueue
{
private:
    T *data;
    int capacity;
    int frontIndex;
    int count;

    // copying a raw array owner is unsafe, so we switch it off
    CircularQueue(const CircularQueue &);
    CircularQueue &operator=(const CircularQueue &);

public:
    CircularQueue(int cap) : capacity(cap), frontIndex(0), count(0)
    {
        data = new T[capacity];
    }

    ~CircularQueue()
    {
        delete[] data;
    }

    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == capacity; }
    int getSize() const { return count; }
    int getCapacity() const { return capacity; }

    // insert at the rear, the modulo makes the rear wrap around
    bool enqueue(const T &value)
    {
        if (isFull())
        {
            return false;
        }
        int rear = (frontIndex + count) % capacity;
        data[rear] = value;
        count++;
        return true;
    }

    // remove from the front, nothing gets shifted
    bool dequeue(T &removed)
    {
        if (isEmpty())
        {
            return false;
        }
        removed = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return true;
    }

    // look at the front element without removing it
    bool peek(T &frontValue) const
    {
        if (isEmpty())
        {
            return false;
        }
        frontValue = data[frontIndex];
        return true;
    }

    // gives the element sitting i places behind the front, handy for custom printing
    bool getAt(int i, T &value) const
    {
        if (i < 0 || i >= count)
        {
            return false;
        }
        value = data[(frontIndex + i) % capacity];
        return true;
    }

    // prints from front to rear on one line
    void display() const
    {
        if (isEmpty())
        {
            cout << "(empty)";
            return;
        }
        for (int i = 0; i < count; i++)
        {
            cout << data[(frontIndex + i) % capacity];
            if (i != count - 1)
            {
                cout << "  ";
            }
        }
    }
};

// ---------------------------------------------------------------
// Task 1: Cafeteria token system
// ---------------------------------------------------------------

struct Student
{
    int token;
    string name;
    string order;

    Student() : token(0), name(""), order("") {}
};

ostream &operator<<(ostream &out, const Student &s)
{
    out << "Token #" << s.token << " (" << s.name << ")";
    return out;
}

void cafeteriaTask()
{
    cout << endl
         << "Cafeteria Token System" << endl;
    int capacity = readInt("How many students can wait in line at once? ", 1, 100000);

    CircularQueue<Student> line(capacity);
    int nextToken = 1;
    int choice;

    do
    {
        cout << endl
             << "==== Cafeteria Token Menu ====" << endl;
        cout << "1. Issue New Token" << endl;
        cout << "2. Serve Next Student" << endl;
        cout << "3. Show Next in Line (Peek)" << endl;
        cout << "4. Show Waiting Queue" << endl;
        cout << "5. Count Waiting Students" << endl;
        cout << "6. Exit" << endl;
        choice = readInt("Enter choice: ", 1, 6);

        switch (choice)
        {
        case 1:
        {
            if (line.isFull())
            {
                cout << "The line is full right now, serve someone first." << endl;
                break;
            }
            Student s;
            s.name = readText("Name: ");
            s.order = readText("Order: ");
            s.token = nextToken;
            line.enqueue(s);
            nextToken++;
            cout << "Token #" << s.token << " issued." << endl;
            break;
        }
        case 2:
        {
            Student served;
            if (line.dequeue(served))
            {
                cout << "Serving " << served << ": " << served.order << endl;
            }
            else
            {
                cout << "No students waiting" << endl;
            }
            break;
        }
        case 3:
        {
            Student next;
            if (line.peek(next))
            {
                cout << "Next in line: " << next << endl;
            }
            else
            {
                cout << "No students waiting" << endl;
            }
            break;
        }
        case 4:
        {
            if (line.isEmpty())
            {
                cout << "No students waiting" << endl;
                break;
            }
            cout << "Waiting from front to rear: " << endl;
            for (int i = 0; i < line.getSize(); i++)
            {
                Student s;
                line.getAt(i, s);
                cout << (i + 1) << ". " << s << ": " << s.order << endl;
            }
            break;
        }
        case 5:
            cout << "Students waiting: " << line.getSize() << endl;
            break;
        case 6:
            cout << "Closing the counter, back to the main menu." << endl;
            break;
        }
    } while (choice != 6);
}

// ---------------------------------------------------------------
// Task 2 demo: try every queue operation by hand
// ---------------------------------------------------------------

void queueDemoTask()
{
    cout << endl
         << "Circular Queue Implementation" << endl;
    int capacity = readInt("Enter the capacity of the queue: ", 1, 100000);

    CircularQueue<int> q(capacity);
    int choice;

    do
    {
        cout << endl
             << "---- Queue Operations ----" << endl;
        cout << "1. Enqueue" << endl;
        cout << "2. Dequeue" << endl;
        cout << "3. Peek" << endl;
        cout << "4. Check isEmpty and isFull" << endl;
        cout << "5. Display" << endl;
        cout << "6. Get size" << endl;
        cout << "7. Back to main menu" << endl;
        choice = readInt("Enter choice: ", 1, 7);

        switch (choice)
        {
        case 1:
        {
            if (q.isFull())
            {
                cout << "Queue is full, cannot enqueue." << endl;
                break;
            }
            int value = readNumber("Value to insert: ");
            q.enqueue(value);
            cout << value << " added at the rear." << endl;
            break;
        }
        case 2:
        {
            int removed;
            if (q.dequeue(removed))
            {
                cout << removed << " removed from the front." << endl;
            }
            else
            {
                cout << "Queue is empty, nothing to remove." << endl;
            }
            break;
        }
        case 3:
        {
            int frontValue;
            if (q.peek(frontValue))
            {
                cout << "Front element is " << frontValue << endl;
            }
            else
            {
                cout << "Queue is empty." << endl;
            }
            break;
        }
        case 4:
            cout << "isEmpty: " << (q.isEmpty() ? "yes" : "no") << endl;
            cout << "isFull: " << (q.isFull() ? "yes" : "no") << endl;
            break;
        case 5:
            cout << "Front to rear: ";
            q.display();
            cout << endl;
            break;
        case 6:
            cout << "Elements stored: " << q.getSize() << " out of " << q.getCapacity() << endl;
            break;
        case 7:
            cout << "Going back." << endl;
            break;
        }
    } while (choice != 7);
}

// ---------------------------------------------------------------
// Task 3: Josephus problem
// ---------------------------------------------------------------

void josephusTask()
{
    cout << endl
         << "The Josephus Problem" << endl;
    int n = readInt("Enter number of people (n, at least 1): ", 1, 100000);
    int k = readInt("Enter the counting step (k, at least 1): ", 1, 1000000);

    CircularQueue<int> circle(n);
    for (int i = 1; i <= n; i++)
    {
        circle.enqueue(i);
    }

    cout << endl
         << "Starting circle: ";
    circle.display();
    cout << endl
         << endl;

    int round = 1;
    int eliminatedCount = 0;

    while (circle.getSize() > 1)
    {
        cout << "Round " << round << endl;
        cout << "  Queue at start: ";
        circle.display();
        cout << endl;

        // k-1 people go from the front to the rear, then the next one is eliminated
        int skips = k - 1;

        cout << "  Skipped (moved to rear): ";
        if (skips == 0)
        {
            cout << "nobody";
        }
        for (int i = 0; i < skips; i++)
        {
            int person;
            circle.dequeue(person);
            circle.enqueue(person);
            cout << person;
            if (i != skips - 1)
            {
                cout << ", ";
            }
        }
        cout << endl;

        int out;
        circle.dequeue(out);
        eliminatedCount++;
        cout << "  Eliminated (out #" << eliminatedCount << "): " << out << endl;

        cout << "  Queue after round: ";
        circle.display();
        cout << endl
             << endl;

        round++;
    }

    int survivor;
    circle.peek(survivor);
    cout << "Survivor = " << survivor << endl;
}

// ---------------------------------------------------------------
// Task 5: reverse a queue
// ---------------------------------------------------------------

// pull the front out, reverse whatever is left, then put it at the rear
// the recursion stack plays the role of a helper stack here
void reverseQueue(CircularQueue<int> &q)
{
    if (q.isEmpty())
    {
        return;
    }
    int frontValue;
    q.dequeue(frontValue);
    reverseQueue(q);
    q.enqueue(frontValue);
}

void reverseTask()
{
    cout << endl
         << "Reverse a Queue" << endl;
    int count = readInt("How many elements? ", 1, 100000);

    CircularQueue<int> q(count);
    for (int i = 0; i < count; i++)
    {
        int value = readNumber("Element " + to_string(i + 1) + ": ");
        q.enqueue(value);
    }

    cout << "Original queue: ";
    q.display();
    cout << endl;

    reverseQueue(q);

    cout << "Reversed queue: ";
    q.display();
    cout << endl;
}

// ---------------------------------------------------------------
// Task 6: binary numbers from 1 to n using a queue
// ---------------------------------------------------------------

void binaryTask()
{
    cout << endl
         << "Binary Numbers Using a Queue" << endl;
    int n = readInt("Enter n: ", 1, 100000);

    // at most n + 1 strings are ever waiting, two extra slots keeps it safe
    CircularQueue<string> q(n + 2);
    q.enqueue("1");

    cout << "[";
    for (int i = 1; i <= n; i++)
    {
        string current;
        q.dequeue(current);

        cout << "\"" << current << "\"";
        if (i != n)
        {
            cout << ", ";
        }

        // every binary number spawns two children by appending 0 and 1
        q.enqueue(current + "0");
        q.enqueue(current + "1");
    }
    cout << "]" << endl;
}

// ---------------------------------------------------------------
// Main menu
// ---------------------------------------------------------------

int main()
{
    int choice;

    do
    {
        cout << endl
             << "######## Lab 05 Queue Tasks ########" << endl;
        cout << "1. Task 1: Cafeteria Token System" << endl;
        cout << "2. Task 2: Circular Queue Implementation" << endl;
        cout << "3. Task 3: The Josephus Problem" << endl;
        cout << "5. Task 5: Reverse a Queue" << endl;
        cout << "6. Task 6: Generate Binary Numbers" << endl;
        cout << "0. Exit" << endl;
        choice = readInt("Enter your choice: ", 0, 6);

        switch (choice)
        {
        case 1:
            cafeteriaTask();
            break;
        case 2:
            queueDemoTask();
            break;
        case 3:
            josephusTask();
            break;
        case 5:
            reverseTask();
            break;
        case 6:
            binaryTask();
            break;
        case 0:
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "The manual has no Task " << choice << ", pick one from the list." << endl;
            break;
        }
    } while (choice != 0);

    return 0;
}
