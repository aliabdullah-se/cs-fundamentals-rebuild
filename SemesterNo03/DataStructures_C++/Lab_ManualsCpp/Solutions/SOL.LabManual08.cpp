#include <iostream>
#include <queue>
#include <string>
using namespace std;

template <typename T>
struct Node {
    T data;
    int height;
    Node<T>* left;
    Node<T>* right;

    Node(T value) : data(value), height(0), left(nullptr), right(nullptr) {}
};

template <typename T>
class AVL {
private:
    Node<T>* root;

    int height(Node<T>* node) const {
        return node == nullptr ? -1 : node->height;
    }

    void updateHeight(Node<T>* node) {
        if (node != nullptr) {
            int leftHeight = height(node->left);
            int rightHeight = height(node->right);
            node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
        }
    }

    int getBalanceFactor(Node<T>* node) const {
        if (node == nullptr) {
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    Node<T>* rotateRight(Node<T>* node) {
        Node<T>* newRoot = node->left;
        Node<T>* movedSubtree = newRoot->right;

        newRoot->right = node;
        node->left = movedSubtree;

        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    Node<T>* rotateLeft(Node<T>* node) {
        Node<T>* newRoot = node->right;
        Node<T>* movedSubtree = newRoot->left;

        newRoot->left = node;
        node->right = movedSubtree;

        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    Node<T>* rebalance(Node<T>* node) {
        if (node == nullptr) {
            return node;
        }

        updateHeight(node);
        int balance = getBalanceFactor(node);

        if (balance > 1) {
            if (getBalanceFactor(node->left) < 0) {
                cout << "Rotation: LR at " << node->data << endl;
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            cout << "Rotation: LL at " << node->data << endl;
            return rotateRight(node);
        }

        if (balance < -1) {
            if (getBalanceFactor(node->right) > 0) {
                cout << "Rotation: RL at " << node->data << endl;
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            cout << "Rotation: RR at " << node->data << endl;
            return rotateLeft(node);
        }

        return node;
    }

    Node<T>* insert(Node<T>* node, T data) {
        if (node == nullptr) {
            return new Node<T>(data);
        }

        if (data < node->data) {
            node->left = insert(node->left, data);
        } else if (node->data < data) {
            node->right = insert(node->right, data);
        } else {
            return node;
        }

        return rebalance(node);
    }

    Node<T>* findMinNode(Node<T>* node) const {
        if (node == nullptr) {
            return nullptr;
        }
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node<T>* findMaxNode(Node<T>* node) const {
        if (node == nullptr) {
            return nullptr;
        }
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }

    Node<T>* remove(Node<T>* node, T data) {
        if (node == nullptr) {
            return nullptr;
        }

        if (data < node->data) {
            node->left = remove(node->left, data);
        } else if (node->data < data) {
            node->right = remove(node->right, data);
        } else {
            if (node->left == nullptr || node->right == nullptr) {
                Node<T>* child = node->left != nullptr ? node->left : node->right;
                delete node;
                return child;
            }

            Node<T>* successor = findMinNode(node->right);
            node->data = successor->data;
            node->right = remove(node->right, successor->data);
        }

        return rebalance(node);
    }

    void inorder(Node<T>* node) const {
        if (node == nullptr) return;
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }

    void preorder(Node<T>* node) const {
        if (node == nullptr) return;
        cout << node->data << " ";
        preorder(node->left);
        preorder(node->right);
    }

    void postorder(Node<T>* node) const {
        if (node == nullptr) return;
        postorder(node->left);
        postorder(node->right);
        cout << node->data << " ";
    }

    void printSideways(Node<T>* node, int depth) const {
        if (node == nullptr) return;
        printSideways(node->right, depth + 1);

        for (int i = 0; i < depth * 4; i++) {
            cout << " ";
        }

        int balance = getBalanceFactor(node);
        cout << node->data << " [h" << node->height << " bf";
        if (balance > 0) cout << "+";
        cout << balance << "]" << endl;

        printSideways(node->left, depth + 1);
    }

    bool checkBalanced(Node<T>* node) const {
        if (node == nullptr) return true;

        int balance = getBalanceFactor(node);
        if (balance < -1 || balance > 1) return false;

        return checkBalanced(node->left) && checkBalanced(node->right);
    }

    void clear(Node<T>* node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    int countNodes(Node<T>* node) const {
        if (node == nullptr) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    void listInRange(Node<T>* node, T low, T high) const {
        if (node == nullptr) return;

        if (low < node->data) listInRange(node->left, low, high);
        if (!(node->data < low) && !(high < node->data)) cout << node->data << " ";
        if (node->data < high) listInRange(node->right, low, high);
    }

public:
    AVL() : root(nullptr) {}

    ~AVL() {
        clear(root);
    }

    void insert(T data) {
        root = insert(root, data);
    }

    void remove(T data) {
        root = remove(root, data);
    }

    Node<T>* search(T data) const {
        Node<T>* current = root;
        while (current != nullptr) {
            if (data < current->data) {
                current = current->left;
            } else if (current->data < data) {
                current = current->right;
            } else {
                return current;
            }
        }
        return nullptr;
    }

    T findMin() const {
        Node<T>* node = findMinNode(root);
        if (node == nullptr) {
            cout << "Tree is empty." << endl;
            return T();
        }
        return node->data;
    }

    T findMax() const {
        Node<T>* node = findMaxNode(root);
        if (node == nullptr) {
            cout << "Tree is empty." << endl;
            return T();
        }
        return node->data;
    }

    void inorder() const {
        inorder(root);
        cout << endl;
    }

    void preorder() const {
        preorder(root);
        cout << endl;
    }

    void postorder() const {
        postorder(root);
        cout << endl;
    }

    void levelOrder() const {
        if (root == nullptr) {
            cout << "Tree is empty." << endl;
            return;
        }

        queue<Node<T>*> nodes;
        nodes.push(root);

        while (!nodes.empty()) {
            Node<T>* current = nodes.front();
            nodes.pop();
            cout << current->data << " ";

            if (current->left != nullptr) nodes.push(current->left);
            if (current->right != nullptr) nodes.push(current->right);
        }
        cout << endl;
    }

    void printTree() const {
        if (root == nullptr) {
            cout << "Tree is empty." << endl;
            return;
        }
        printSideways(root, 0);
    }

    bool isBalanced() const {
        return checkBalanced(root);
    }

    void clear() {
        clear(root);
        root = nullptr;
    }

    int size() const {
        return countNodes(root);
    }

    int treeHeight() const {
        return height(root);
    }

    void listInRange(T low, T high) const {
        if (high < low) {
            cout << "Invalid range." << endl;
            return;
        }
        listInRange(root, low, high);
        cout << endl;
    }

    void showStrongestAndWeakestWarriors() const {
        Node<T>* strongest = nullptr;
        Node<T>* weakest = nullptr;
        findWarriorExtremes(root, strongest, weakest);
        if (strongest == nullptr) {
            cout << "Tree is empty." << endl;
            return;
        }
        cout << "Strongest: " << strongest->data << endl;
        cout << "Weakest: " << weakest->data << endl;
    }

private:
    void findWarriorExtremes(Node<T>* node, Node<T>*& strongest, Node<T>*& weakest) const {
        if (node == nullptr) return;
        if (strongest == nullptr || node->data.chiPower > strongest->data.chiPower) strongest = node;
        if (weakest == nullptr || node->data.chiPower < weakest->data.chiPower) weakest = node;
        findWarriorExtremes(node->left, strongest, weakest);
        findWarriorExtremes(node->right, strongest, weakest);
    }
};

class Warrior {
public:
    int id;
    string name;
    int chiPower;

    Warrior(int warriorID = 0, string warriorName = "", int power = 0)
        : id(warriorID), name(warriorName), chiPower(power) {}

    bool operator<(const Warrior& other) const {
        return id < other.id;
    }

    bool operator>(const Warrior& other) const {
        return id > other.id;
    }

    bool operator==(const Warrior& other) const {
        return id == other.id;
    }
};

ostream& operator<<(ostream& output, const Warrior& warrior) {
    output << "ID: " << warrior.id << ", Name: " << warrior.name
           << ", Chi Power: " << warrior.chiPower;
    return output;
}

void findStrongestAndWeakest(Node<Warrior>* node, Warrior*& strongest, Warrior*& weakest) {
    if (node == nullptr) return;

    if (strongest == nullptr || node->data.chiPower > strongest->chiPower) {
        strongest = &(node->data);
    }
    if (weakest == nullptr || node->data.chiPower < weakest->chiPower) {
        weakest = &(node->data);
    }

    findStrongestAndWeakest(node->left, strongest, weakest);
    findStrongestAndWeakest(node->right, strongest, weakest);
}

template <typename T>
void printTreeStats(const AVL<T>& tree) {
    cout << "Number of nodes: " << tree.size() << endl;
    cout << "Tree height: " << tree.treeHeight() << endl;
    cout << "Balanced: " << (tree.isBalanced() ? "Yes" : "No") << endl;
}

int main() {
    AVL<int> tree;
    int choice;
    int value;

    cout << "AVL Tree Tasks 2 and 3" << endl;

    do {
        cout << endl;
        cout << "1. Insert integer" << endl;
        cout << "2. Remove integer" << endl;
        cout << "3. Search integer" << endl;
        cout << "4. Find minimum" << endl;
        cout << "5. Find maximum" << endl;
        cout << "6. Inorder traversal" << endl;
        cout << "7. Preorder traversal" << endl;
        cout << "8. Postorder traversal" << endl;
        cout << "9. Level-order traversal" << endl;
        cout << "10. Print tree" << endl;
        cout << "11. Check balance" << endl;
        cout << "12. Clear tree" << endl;
        cout << "13. List keys in range" << endl;
        cout << "14. Run example: 50, 40, 30, 60, 70, 55" << endl;
        cout << "15. Open Kung Fu Warrior menu (Task 4)" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter value: ";
            cin >> value;
            tree.insert(value);
        } else if (choice == 2) {
            cout << "Enter value to remove: ";
            cin >> value;
            tree.remove(value);
        } else if (choice == 3) {
            cout << "Enter value to search: ";
            cin >> value;
            Node<int>* found = tree.search(value);
            if (found != nullptr) cout << "Found: " << found->data << endl;
            else cout << "Value not found." << endl;
        } else if (choice == 4) {
            if (tree.size() > 0) cout << "Minimum: " << tree.findMin() << endl;
            else cout << "Tree is empty." << endl;
        } else if (choice == 5) {
            if (tree.size() > 0) cout << "Maximum: " << tree.findMax() << endl;
            else cout << "Tree is empty." << endl;
        } else if (choice == 6) {
            tree.inorder();
        } else if (choice == 7) {
            tree.preorder();
        } else if (choice == 8) {
            tree.postorder();
        } else if (choice == 9) {
            tree.levelOrder();
        } else if (choice == 10) {
            tree.printTree();
        } else if (choice == 11) {
            cout << "Balanced: " << (tree.isBalanced() ? "Yes" : "No") << endl;
        } else if (choice == 12) {
            tree.clear();
            cout << "Tree cleared." << endl;
        } else if (choice == 13) {
            int low, high;
            cout << "Enter lower and upper bounds: ";
            cin >> low >> high;
            tree.listInRange(low, high);
        } else if (choice == 14) {
            tree.clear();
            int values[] = {50, 40, 30, 60, 70, 55};
            for (int i = 0; i < 6; i++) tree.insert(values[i]);
            cout << "Inorder: ";
            tree.inorder();
            cout << "Preorder: ";
            tree.preorder();
            cout << "Tree:" << endl;
            tree.printTree();
        } else if (choice == 15) {
            AVL<Warrior> warriors;
            int warriorChoice;
            do {
                cout << endl;
                cout << "Kung Fu Balance Tree" << endl;
                cout << "1. Add warrior" << endl;
                cout << "2. Remove warrior by ID" << endl;
                cout << "3. Find warrior by ID" << endl;
                cout << "4. List warriors in ID order" << endl;
                cout << "5. Strongest and weakest warrior" << endl;
                cout << "6. Show tree stats" << endl;
                cout << "7. Print warrior tree" << endl;
                cout << "8. List warriors in ID range" << endl;
                cout << "0. Return to main menu" << endl;
                cout << "Enter choice: ";
                cin >> warriorChoice;

                if (warriorChoice == 1) {
                    int id, power;
                    string name;
                    cout << "Enter warrior ID: ";
                    cin >> id;
                    if (warriors.search(Warrior(id, "", 0)) != nullptr) {
                        cout << "A warrior with this ID already exists." << endl;
                    } else {
                        cout << "Enter warrior name (one word): ";
                        cin >> name;
                        cout << "Enter chi power: ";
                        cin >> power;
                        warriors.insert(Warrior(id, name, power));
                    }
                } else if (warriorChoice == 2) {
                    int id;
                    cout << "Enter ID to remove: ";
                    cin >> id;
                    if (warriors.search(Warrior(id, "", 0)) != nullptr) {
                        warriors.remove(Warrior(id, "", 0));
                        cout << "Warrior removed." << endl;
                    } else {
                        cout << "Warrior not found." << endl;
                    }
                } else if (warriorChoice == 3) {
                    int id;
                    cout << "Enter ID to find: ";
                    cin >> id;
                    Node<Warrior>* found = warriors.search(Warrior(id, "", 0));
                    if (found != nullptr) cout << found->data << endl;
                    else cout << "Warrior not found." << endl;
                } else if (warriorChoice == 4) {
                    warriors.inorder();
                } else if (warriorChoice == 5) {
                    warriors.showStrongestAndWeakestWarriors();
                } else if (warriorChoice == 6) {
                    printTreeStats(warriors);
                } else if (warriorChoice == 7) {
                    warriors.printTree();
                } else if (warriorChoice == 8) {
                    int low, high;
                    cout << "Enter minimum and maximum ID: ";
                    cin >> low >> high;
                    warriors.listInRange(Warrior(low, "", 0), Warrior(high, "", 0));
                } else if (warriorChoice != 0) {
                    cout << "Invalid choice." << endl;
                }
            } while (warriorChoice != 0);
        } else if (choice != 0) {
            cout << "Invalid choice." << endl;
        }
    } while (choice != 0);

    cout << "Program ended." << endl;
    return 0;
}
