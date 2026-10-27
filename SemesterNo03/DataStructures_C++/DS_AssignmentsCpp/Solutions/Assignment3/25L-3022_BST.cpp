#include <iostream>
#include <string>
#include <limits>
using namespace std;

class ShuttleBST {
private:
    struct Node {
        int departureTime;
        string destination;
        int seatsAvailable;
        Node* left;
        Node* right;

        Node(int time, const string& dest, int seats)
            : departureTime(time), destination(dest), seatsAvailable(seats),
              left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insertNode(Node* node, int time, const string& destination,
                     int seats, bool& inserted) {
        if (node == nullptr) {
            inserted = true;
            return new Node(time, destination, seats);
        }

        if (time < node->departureTime) {
            node->left = insertNode(node->left, time, destination, seats, inserted);
        } else if (time > node->departureTime) {
            node->right = insertNode(node->right, time, destination, seats, inserted);
        } else {
            inserted = false;
        }
        return node;
    }

    Node* findNode(int time) const {
        Node* current = root;
        while (current != nullptr) {
            if (time == current->departureTime) return current;
            current = (time < current->departureTime) ? current->left : current->right;
        }
        return nullptr;
    }

    Node* minimumNode(Node* node) const {
        if (node == nullptr) return nullptr;
        while (node->left != nullptr) node = node->left;
        return node;
    }

    Node* deleteNode(Node* node, int time, bool& removed) {
        if (node == nullptr) return nullptr;

        if (time < node->departureTime) {
            node->left = deleteNode(node->left, time, removed);
        } else if (time > node->departureTime) {
            node->right = deleteNode(node->right, time, removed);
        } else {
            removed = true;

            if (node->left == nullptr) {
                Node* replacement = node->right;
                delete node;
                return replacement;
            }
            if (node->right == nullptr) {
                Node* replacement = node->left;
                delete node;
                return replacement;
            }

            Node* successor = minimumNode(node->right);
            node->departureTime = successor->departureTime;
            node->destination = successor->destination;
            node->seatsAvailable = successor->seatsAvailable;

            bool successorRemoved = false;
            node->right = deleteNode(node->right, successor->departureTime, successorRemoved);
        }
        return node;
    }

    void printRecord(const Node* node) const {
        cout << "Time: " << node->departureTime
             << " minutes (" << formatTime(node->departureTime) << ")"
             << ", Destination: " << node->destination
             << ", Seats available: " << node->seatsAvailable << '\n';
    }

    void printInorder(Node* node, bool& printed) const {
        if (node == nullptr) return;
        printInorder(node->left, printed);
        printRecord(node);
        printed = true;
        printInorder(node->right, printed);
    }

    void printRange(Node* node, int start, int end, bool& printed) const {
        if (node == nullptr) return;

        if (node->departureTime > start)
            printRange(node->left, start, end, printed);

        if (node->departureTime >= start && node->departureTime <= end) {
            printRecord(node);
            printed = true;
        }

        if (node->departureTime < end)
            printRange(node->right, start, end, printed);
    }

    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    static bool validTime(int time) {
        return time >= 0 && time <= 1439;
    }

public:
    ShuttleBST() : root(nullptr) {}

    ~ShuttleBST() {
        destroy(root);
    }

    ShuttleBST(const ShuttleBST&) = delete;
    ShuttleBST& operator=(const ShuttleBST&) = delete;

    static string formatTime(int time) {
        int hours = time / 60;
        int minutes = time % 60;
        string result;
        result += (hours < 10 ? "0" : "") + to_string(hours);
        result += ":";
        result += (minutes < 10 ? "0" : "") + to_string(minutes);
        return result;
    }

    void scheduleDeparture(int time, const string& destination, int seats) {
        if (!validTime(time)) {
            cout << "Rejected: departure time must be from 0 to 1439.\n";
            return;
        }
        if (destination.empty()) {
            cout << "Rejected: destination cannot be empty.\n";
            return;
        }
        if (seats <= 0) {
            cout << "Rejected: seats must be positive.\n";
            return;
        }

        bool inserted = false;
        root = insertNode(root, time, destination, seats, inserted);
        if (inserted)
            cout << "Departure inserted successfully.\n";
        else
            cout << "Rejected: a departure already exists at that time.\n";
    }

    void findDeparture(int time) const {
        if (!validTime(time)) {
            cout << "Invalid departure time.\n";
            return;
        }
        Node* found = findNode(time);
        if (found == nullptr) cout << "No departure found at " << formatTime(time) << ".\n";
        else printRecord(found);
    }

    void cancelDeparture(int time) {
        if (!validTime(time)) {
            cout << "Invalid departure time.\n";
            return;
        }
        bool removed = false;
        root = deleteNode(root, time, removed);
        cout << (removed ? "Departure cancelled successfully.\n"
                         : "No departure found; tree unchanged.\n");
    }

    void rescheduleDeparture(int oldTime, int newTime) {
        if (!validTime(oldTime) || !validTime(newTime)) {
            cout << "Rejected: both times must be from 0 to 1439.\n";
            return;
        }

        Node* existing = findNode(oldTime);
        if (existing == nullptr) {
            cout << "Rejected: original departure does not exist.\n";
            return;
        }
        if (oldTime == newTime) {
            cout << "Departure already has that time; no change needed.\n";
            return;
        }
        if (findNode(newTime) != nullptr) {
            cout << "Rejected: the new time is already occupied; tree unchanged.\n";
            return;
        }

        string destination = existing->destination;
        int seats = existing->seatsAvailable;

        bool removed = false;
        root = deleteNode(root, oldTime, removed);
        if (removed) {
            bool inserted = false;
            root = insertNode(root, newTime, destination, seats, inserted);
            if (inserted) cout << "Departure rescheduled successfully.\n";
            else {
                // This branch should not occur because the new time was checked first.
                bool restore = false;
                root = insertNode(root, oldTime, destination, seats, restore);
                cout << "Rescheduling failed; original departure restored.\n";
            }
        }
    }

    void nextDepartureAfter(int time) const {
        Node* current = root;
        Node* candidate = nullptr;

        while (current != nullptr) {
            if (current->departureTime > time) {
                candidate = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (candidate == nullptr) cout << "No upcoming shuttle.\n";
        else printRecord(candidate);
    }

    void listDeparturesBetween(int start, int end) const {
        if (!validTime(start) || !validTime(end)) {
            cout << "Invalid time range: times must be from 0 to 1439.\n";
            return;
        }
        if (start > end) {
            cout << "Reversed interval: start must not exceed end.\n";
            return;
        }

        bool printed = false;
        printRange(root, start, end, printed);
        if (!printed) cout << "No departures in this interval.\n";
    }

    void boardPassengers(int time, int count) {
        if (!validTime(time)) {
            cout << "Invalid departure time.\n";
            return;
        }
        if (count <= 0) {
            cout << "Rejected: passenger count must be positive.\n";
            return;
        }

        Node* trip = findNode(time);
        if (trip == nullptr) {
            cout << "Rejected: departure not found.\n";
        } else if (count > trip->seatsAvailable) {
            cout << "Rejected: not enough seats available; record unchanged.\n";
        } else {
            trip->seatsAvailable -= count;
            cout << "Passengers boarded. Seats remaining: " << trip->seatsAvailable << ".\n";
        }
    }

    void printDailySchedule() const {
        if (root == nullptr) {
            cout << "The schedule is empty.\n";
            return;
        }
        bool printed = false;
        printInorder(root, printed);
    }
};

static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cout << "Invalid input. Please enter an integer.\n";
        clearInput();
    }
}

int main() {
    ShuttleBST scheduler;
    int choice;

    do {
        cout << "\n===== Campus Shuttle Dispatch Scheduler =====\n"
             << "1. Schedule departure\n"
             << "2. Search departure\n"
             << "3. Cancel departure\n"
             << "4. Reschedule departure\n"
             << "5. Find next departure after a time\n"
             << "6. List departures within a time range\n"
             << "7. Board passengers\n"
             << "8. Print daily schedule\n"
             << "0. Exit\n";

        choice = readInt("Enter choice: ");

        switch (choice) {
            case 1: {
                int time = readInt("Departure time (minutes after midnight, 0-1439): ");
                string destination;
                cout << "Destination (single word): ";
                cin >> destination;
                int seats = readInt("Number of seats: ");
                scheduler.scheduleDeparture(time, destination, seats);
                break;
            }
            case 2: {
                int time = readInt("Departure time to search: ");
                scheduler.findDeparture(time);
                break;
            }
            case 3: {
                int time = readInt("Departure time to cancel: ");
                scheduler.cancelDeparture(time);
                break;
            }
            case 4: {
                int oldTime = readInt("Current departure time: ");
                int newTime = readInt("New departure time: ");
                scheduler.rescheduleDeparture(oldTime, newTime);
                break;
            }
            case 5: {
                int time = readInt("Find next departure strictly after (minutes): ");
                scheduler.nextDepartureAfter(time);
                break;
            }
            case 6: {
                int start = readInt("Start time (minutes): ");
                int end = readInt("End time (minutes): ");
                scheduler.listDeparturesBetween(start, end);
                break;
            }
            case 7: {
                int time = readInt("Departure time: ");
                int count = readInt("Number of passengers boarding: ");
                scheduler.boardPassengers(time, count);
                break;
            }
            case 8:
                scheduler.printDailySchedule();
                break;
            case 0:
                cout << "Exiting scheduler. Goodbye!\n";
                break;
            default:
                cout << "Invalid menu choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
