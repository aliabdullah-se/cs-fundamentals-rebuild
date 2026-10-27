// Q2: CodeArena, circular coding challenge tournament

// Only iostream, fstream and string are used. Stack, challenge Queue,
// circular player ring and the match history list are all hand written.
//
// Complexity note (when the needed current and previous pointers are known):
//   Stack push and pop               O(1)
//   Queue enqueue and dequeue        O(1)
//   DLL append                       O(1) because a tail pointer is kept
//   DLL full traversal               O(n)
//   CLL insertion at the end         O(1) because a tail pointer is kept
//   CLL removal of a known node      O(1) when its previous node is known

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool isDigitChar(char c) { return c >= '0' && c <= '9'; }
bool isSpaceChar(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }

string trimText(const string &s)
{
    size_t a = 0, b = s.size();
    while (a < b && isSpaceChar(s[a]))
        a++;
    while (b > a && isSpaceChar(s[b - 1]))
        b = b - 1;
    return s.substr(a, b - a);
}

// reads an optional sign followed by digits, nothing else allowed
bool parseInteger(const string &text, long long &out)
{
    string s = trimText(text);
    size_t i = 0;
    bool negative = false;
    if (i < s.size() && (s[i] == '+' || s[i] == '-'))
    {
        negative = (s[i] == '-');
        i++;
    }
    if (i >= s.size())
        return false;
    if (s.size() - i > 18)
        return false;
    long long v = 0;
    for (; i < s.size(); i++)
    {
        if (!isDigitChar(s[i]))
            return false;
        v = v * 10 + (s[i] - '0');
    }
    out = negative ? 0 - v : v;
    return true;
}

// Custom Stack
template <typename T>
class Stack
{
    struct Node
    {
        T data;
        Node *next;
        Node(const T &d, Node *n) : data(d), next(n) {}
    };
    Node *topNode;
    int count;
    Stack(const Stack &) = delete;
    Stack &operator=(const Stack &) = delete;

public:
    Stack() : topNode(nullptr), count(0) {}
    ~Stack()
    {
        T dummy;
        while (pop(dummy))
        {
        }
    }
    bool isEmpty() const { return topNode == nullptr; }
    int size() const { return count; }
    void push(const T &v)
    {
        topNode = new Node(v, topNode);
        count++;
    }
    bool pop(T &out)
    {
        if (isEmpty())
            return false;
        Node *t = topNode;
        out = t->data;
        topNode = t->next;
        delete t;
        count = count - 1;
        return true;
    }
    bool peek(T &out) const
    {
        if (isEmpty())
            return false;
        out = topNode->data;
        return true;
    }
};

// Expression engine: parenthesis check, infix to postfix, postfix eval
struct ParenInfo
{
    char ch = ' ';
    int pos = 0;
};

bool isOpener(char c) { return c == '(' || c == '[' || c == '{'; }
bool isCloser(char c) { return c == ')' || c == ']' || c == '}'; }
char closerFor(char c)
{
    if (c == '(')
        return ')';
    if (c == '[')
        return ']';
    return '}';
}
bool isOperator(char c) { return c == '+' || c == '-' || c == '*' || c == '/' || c == '%'; }
int precedenceOf(char op) { return (op == '*' || op == '/' || op == '%') ? 2 : 1; }

class ExpressionEngine
{
public:
    // Step 1: delimiter validation with a stack
    static bool validateParens(const string &e, string &reason)
    {
        Stack<ParenInfo> st;
        for (size_t i = 0; i < e.size(); i++)
        {
            char c = e[i];
            int pos = (int)i + 1;
            if (isOpener(c))
            {
                ParenInfo p;
                p.ch = c;
                p.pos = pos;
                st.push(p);
            }
            else if (isCloser(c))
            {
                ParenInfo top;
                if (!st.pop(top))
                {
                    reason = "extra closing '" + string(1, c) + "' at position " + to_string(pos) +
                             " has no matching opener";
                    return false;
                }
                if (closerFor(top.ch) != c)
                {
                    reason = "parenthesis mismatch at position " + to_string(pos) + ", found '" +
                             string(1, c) + "' but expected '" + string(1, closerFor(top.ch)) +
                             "' to close '" + string(1, top.ch) + "' from position " + to_string(top.pos);
                    return false;
                }
            }
        }
        if (!st.isEmpty())
        {
            ParenInfo top;
            st.peek(top);
            reason = "'" + string(1, top.ch) + "' at position " + to_string(top.pos) +
                     " is never closed, expected '" + string(1, closerFor(top.ch)) + "'";
            return false;
        }
        return true;
    }

    // Step 2: infix to postfix using an operator stack.
    // The postfix text keeps tokens separated by single spaces.
    static bool toPostfix(const string &e, string &postfix, string &reason)
    {
        Stack<char> ops;
        postfix = "";
        bool expectOperand = true;
        size_t i = 0;
        while (i < e.size())
        {
            char c = e[i];
            int pos = (int)i + 1;
            if (isSpaceChar(c))
            {
                i++;
                continue;
            }

            if (isDigitChar(c))
            {
                if (!expectOperand)
                {
                    reason = "missing operator before the number at position " + to_string(pos);
                    return false;
                }
                size_t start = i;
                while (i < e.size() && isDigitChar(e[i]))
                {
                    postfix += e[i];
                    i++;
                }
                if (i - start > 18)
                {
                    reason = "number at position " + to_string(pos) + " is too large";
                    return false;
                }
                postfix += ' ';
                expectOperand = false;
            }
            else if (c == '(')
            {
                if (!expectOperand)
                {
                    reason = "missing operator before '(' at position " + to_string(pos);
                    return false;
                }
                ops.push(c);
                i++;
            }
            else if (c == ')')
            {
                if (expectOperand)
                {
                    reason = "missing operand before ')' at position " + to_string(pos);
                    return false;
                }
                char top;
                bool found = false;
                while (ops.pop(top))
                {
                    if (top == '(')
                    {
                        found = true;
                        break;
                    }
                    postfix += top;
                    postfix += ' ';
                }
                if (!found)
                {
                    reason = "extra ')' at position " + to_string(pos);
                    return false;
                }
                expectOperand = false;
                i++;
            }
            else if (isOperator(c))
            {
                if (expectOperand)
                {
                    reason = "operator '" + string(1, c) + "' at position " + to_string(pos) +
                             " has no left operand";
                    return false;
                }
                char top;
                while (ops.peek(top) && top != '(' && precedenceOf(top) >= precedenceOf(c))
                {
                    ops.pop(top);
                    postfix += top;
                    postfix += ' ';
                }
                ops.push(c);
                expectOperand = true;
                i++;
            }
            else
            {
                reason = "unsupported character '" + string(1, c) + "' at position " + to_string(pos);
                return false;
            }
        }
        if (expectOperand)
        {
            reason = "expression is empty or ends without an operand";
            return false;
        }
        char top;
        while (ops.pop(top))
        {
            if (top == '(')
            {
                reason = "unclosed '(' in expression";
                return false;
            }
            postfix += top;
            postfix += ' ';
        }
        return true;
    }

    // Step 3: evaluate the postfix text with a value stack
    static bool evalPostfix(const string &p, long long &result, string &reason)
    {
        Stack<long long> vals;
        size_t i = 0;
        while (i < p.size())
        {
            char c = p[i];
            if (isSpaceChar(c))
            {
                i++;
                continue;
            }
            if (isDigitChar(c))
            {
                long long n = 0;
                while (i < p.size() && isDigitChar(p[i]))
                {
                    n = n * 10 + (p[i] - '0');
                    i++;
                }
                vals.push(n);
                continue;
            }
            long long a, b;
            if (!vals.pop(b) || !vals.pop(a))
            {
                reason = "malformed operator and operand sequence";
                return false;
            }
            if (c == '+')
                vals.push(a + b);
            else if (c == '-')
                vals.push(a - b);
            else if (c == '*')
                vals.push(a * b);
            else if (c == '/')
            {
                if (b == 0)
                {
                    reason = "division by zero";
                    return false;
                }
                vals.push(a / b);
            }
            else if (c == '%')
            {
                if (b == 0)
                {
                    reason = "modulo by zero";
                    return false;
                }
                vals.push(a % b);
            }
            i++;
        }
        long long r;
        if (!vals.pop(r) || !vals.isEmpty())
        {
            reason = "malformed operator and operand sequence";
            return false;
        }
        result = r;
        return true;
    }

    static bool evaluate(const string &e, long long &result, string &reason)
    {
        if (!validateParens(e, reason))
            return false;
        string postfix;
        if (!toPostfix(e, postfix, reason))
            return false;
        return evalPostfix(postfix, result, reason);
    }
};

// Challenge queue (FIFO)
struct ChallengeNode
{
    string id;
    string expression;
    long long submitted;
    ChallengeNode *next;
    ChallengeNode() : id(""), expression(""), submitted(0), next(nullptr) {}
};

class ChallengeQueue
{
    ChallengeNode *frontNode;
    ChallengeNode *rearNode;
    int count;
    ChallengeQueue(const ChallengeQueue &) = delete;
    ChallengeQueue &operator=(const ChallengeQueue &) = delete;

public:
    ChallengeQueue() : frontNode(nullptr), rearNode(nullptr), count(0) {}
    ~ChallengeQueue()
    {
        ChallengeNode tmp;
        while (dequeue(tmp))
        {
        }
    }
    bool isEmpty() const { return frontNode == nullptr; }
    int size() const { return count; }

    void enqueue(const string &id, const string &expr, long long answer)
    {
        ChallengeNode *n = new ChallengeNode;
        n->id = id;
        n->expression = expr;
        n->submitted = answer;
        if (rearNode != nullptr)
            rearNode->next = n;
        else
            frontNode = n;
        rearNode = n;
        count++;
    }

    bool dequeue(ChallengeNode &out)
    {
        if (isEmpty())
            return false;
        ChallengeNode *t = frontNode;
        out.id = t->id;
        out.expression = t->expression;
        out.submitted = t->submitted;
        out.next = nullptr;
        frontNode = t->next;
        if (frontNode == nullptr)
            rearNode = nullptr;
        delete t;
        count = count - 1;
        return true;
    }
};

// Circular Linked List of players
struct PlayerNode
{
    string id;
    string name;
    int score;
    int strikes;
    PlayerNode *next;
};

class PlayerRing
{
    PlayerNode *head;
    PlayerNode *tail;
    int count;
    PlayerRing(const PlayerRing &) = delete;
    PlayerRing &operator=(const PlayerRing &) = delete;

public:
    PlayerRing() : head(nullptr), tail(nullptr), count(0) {}
    ~PlayerRing()
    {
        // go around the circle exactly count times
        PlayerNode *p = head;
        for (int i = 0; i < count; i++)
        {
            PlayerNode *nxt = p->next;
            delete p;
            p = nxt;
        }
    }

    bool isEmpty() const { return head == nullptr; }
    int size() const { return count; }
    PlayerNode *getHead() const { return head; }
    PlayerNode *getTail() const { return tail; }

    bool hasId(const string &id) const
    {
        PlayerNode *p = head;
        for (int i = 0; i < count; i++)
        {
            if (p->id == id)
                return true;
            p = p->next;
        }
        return false;
    }

    // insert at the end, the new node always links back to head
    void insertAtEnd(const string &id, const string &name)
    {
        PlayerNode *n = new PlayerNode;
        n->id = id;
        n->name = name;
        n->score = 0;
        n->strikes = 0;
        if (head == nullptr)
        {
            head = tail = n;
            n->next = n;
        }
        else
        {
            n->next = head;
            tail->next = n;
            tail = n;
        }
        count++;
    }

    // Removes target, prev must be the node just before target.
    // Returns the successor of target (nullptr if the ring becomes empty).
    PlayerNode *removeNode(PlayerNode *prev, PlayerNode *target)
    {
        if (target == nullptr || head == nullptr)
            return nullptr;

        if (target->next == target)
        { // only node left
            delete target;
            head = tail = nullptr;
            count = 0;
            return nullptr;
        }

        PlayerNode *successor = target->next;
        prev->next = successor;
        if (target == head)
            head = successor; // head removed
        if (target == tail)
            tail = prev; // tail removed
        delete target;
        count = count - 1;
        return successor;
    }

    void printRing() const
    {
        if (head == nullptr)
        {
            cout << "EMPTY\n";
            return;
        }
        PlayerNode *p = head;
        for (int i = 0; i < count; i++)
        {
            cout << p->id << " => ";
            p = p->next;
        }
        cout << "(back to " << head->id << ")\n";
    }
};

// Match history as a Doubly Linked List
struct HistoryNode
{
    int turn;
    string playerId;
    string playerName;
    string challengeId;
    string expression;
    bool valid;
    long long computed;
    long long submitted;
    string outcome;
    int score;
    int strikes;
    HistoryNode *prev;
    HistoryNode *next;
};

class MatchHistoryDLL
{
    HistoryNode *head;
    HistoryNode *tail;
    int count;
    MatchHistoryDLL(const MatchHistoryDLL &) = delete;
    MatchHistoryDLL &operator=(const MatchHistoryDLL &) = delete;

    void printNode(const HistoryNode *h) const
    {
        cout << "Turn " << h->turn << " | " << h->playerId << " " << h->playerName << " | "
             << h->challengeId << " | expr=" << h->expression << " | result=";
        if (h->valid)
            cout << h->computed;
        else
            cout << "INVALID";
        cout << " | submitted=" << h->submitted << " | " << h->outcome
             << " | score=" << h->score << " strikes=" << h->strikes << "\n";
    }

public:
    MatchHistoryDLL() : head(nullptr), tail(nullptr), count(0) {}
    ~MatchHistoryDLL()
    {
        HistoryNode *p = head;
        while (p != nullptr)
        {
            HistoryNode *nxt = p->next;
            delete p;
            p = nxt;
        }
    }

    void append(int turn, const PlayerNode *pl, const ChallengeNode &ch, bool valid,
                long long computed, const string &outcome)
    {
        HistoryNode *n = new HistoryNode;
        n->turn = turn;
        n->playerId = pl->id;
        n->playerName = pl->name;
        n->challengeId = ch.id;
        n->expression = ch.expression;
        n->valid = valid;
        n->computed = computed;
        n->submitted = ch.submitted;
        n->outcome = outcome;
        n->score = pl->score;
        n->strikes = pl->strikes;
        n->next = nullptr;
        n->prev = tail;
        if (tail != nullptr)
            tail->next = n;
        else
            head = n;
        tail = n;
        count++;
    }

    void printForward() const
    {
        if (head == nullptr)
        {
            cout << "(no history)\n";
            return;
        }
        for (const HistoryNode *p = head; p != nullptr; p = p->next)
            printNode(p);
    }

    void printBackward() const
    {
        if (tail == nullptr)
        {
            cout << "(no history)\n";
            return;
        }
        for (const HistoryNode *p = tail; p != nullptr; p = p->prev)
            printNode(p);
    }
};

// Phase A: reading the two input files
bool loadPlayers(const string &fname, PlayerRing &ring, int &rejected)
{
    ifstream in(fname.c_str());
    if (!in.is_open())
        return false;
    string line;
    int lineNo = 0;
    while (getline(in, line))
    {
        lineNo++;
        line = trimText(line);
        if (line.empty())
            continue;

        size_t bar = line.find('|');
        if (bar == string::npos || line.find('|', bar + 1) != string::npos)
        {
            cout << "Players file line " << lineNo << " rejected: expected format ID|Name\n";
            rejected++;
            continue;
        }
        string id = trimText(line.substr(0, bar));
        string name = trimText(line.substr(bar + 1));
        if (id.empty() || name.empty())
        {
            cout << "Players file line " << lineNo << " rejected: player ID or name is empty\n";
            rejected++;
            continue;
        }
        if (ring.hasId(id))
        {
            cout << "Players file line " << lineNo << " rejected: duplicate player ID " << id << "\n";
            rejected++;
            continue;
        }
        ring.insertAtEnd(id, name);
    }
    in.close();
    return true;
}

bool loadChallenges(const string &fname, ChallengeQueue &deck, int &rejected)
{
    ifstream in(fname.c_str());
    if (!in.is_open())
        return false;
    string line;
    int lineNo = 0;
    while (getline(in, line))
    {
        lineNo++;
        line = trimText(line);
        if (line.empty())
            continue;

        size_t b1 = line.find('|');
        size_t b2 = (b1 == string::npos) ? string::npos : line.find('|', b1 + 1);
        size_t b3 = (b2 == string::npos) ? string::npos : line.find('|', b2 + 1);
        if (b1 == string::npos || b2 == string::npos || b3 != string::npos)
        {
            cout << "Challenges file line " << lineNo
                 << " rejected: expected format ID|expression|answer\n";
            rejected++;
            continue;
        }
        string id = trimText(line.substr(0, b1));
        string expr = trimText(line.substr(b1 + 1, b2 - b1 - 1));
        string ansText = line.substr(b2 + 1);
        long long answer;
        if (id.empty() || expr.empty())
        {
            cout << "Challenges file line " << lineNo << " rejected: challenge ID or expression is empty\n";
            rejected++;
            continue;
        }
        if (!parseInteger(ansText, answer))
        {
            cout << "Challenges file line " << lineNo << " rejected: submitted answer '"
                 << trimText(ansText) << "' is not a valid integer\n";
            rejected++;
            continue;
        }
        deck.enqueue(id, expr, answer);
    }
    in.close();
    return true;
}

// Phases B, C and D: the tournament itself
void playTournament(PlayerRing &ring, ChallengeQueue &deck, MatchHistoryDLL &history)
{
    PlayerNode *current = ring.getHead();
    PlayerNode *prev = ring.getTail(); // node just before current
    int turn = 0;

    while (!deck.isEmpty() && !ring.isEmpty())
    {
        turn++;
        ChallengeNode card;
        deck.dequeue(card);

        long long result = 0;
        string reason;
        bool valid = ExpressionEngine::evaluate(card.expression, result, reason);
        string outcome;
        bool eliminated = false;

        if (!valid)
        {
            outcome = "INVALID_CHALLENGE";
        }
        else if (result == card.submitted)
        {
            current->score += 10;
            outcome = "CORRECT";
        }
        else
        {
            current->strikes++;
            if (current->strikes >= 2)
            {
                outcome = "ELIMINATED";
                eliminated = true;
            }
            else
            {
                outcome = "WRONG";
            }
        }

        history.append(turn, current, card, valid, result, outcome);

        cout << "Turn " << turn << " | " << current->id << " " << current->name << " | " << card.id
             << " | result=";
        if (valid)
            cout << result;
        else
            cout << "INVALID";
        cout << " | submitted=" << card.submitted << " | " << outcome
             << " | score=" << current->score << " strikes=" << current->strikes << "\n";
        if (!valid)
            cout << "  Challenge " << card.id << " is invalid: " << reason << "\n";

        if (eliminated)
        {
            cout << "  " << current->id << " " << current->name << " is eliminated\n";
            current = ring.removeNode(prev, current);
            // prev stays the same, it still sits right before the successor.
            // When the ring is empty both pointers become null.
            if (current == nullptr)
                prev = nullptr;
        }
        else
        {
            prev = current;
            current = current->next;
        }

        cout << "Active ring: ";
        ring.printRing();
    }
}

void runSimulation(const string &playersFile, const string &challengesFile)
{
    PlayerRing ring;
    ChallengeQueue deck;
    MatchHistoryDLL history;
    int badPlayers = 0, badChallenges = 0;

    if (!loadPlayers(playersFile, ring, badPlayers))
    {
        cout << "Could not open the players file '" << playersFile << "'.\n";
        return;
    }
    if (!loadChallenges(challengesFile, deck, badChallenges))
    {
        cout << "Could not open the challenges file '" << challengesFile << "'.\n";
        return;
    }

    cout << "\nLOAD REPORT\n";
    cout << "Players loaded: " << ring.size() << "\n";
    cout << "Challenges queued: " << deck.size() << "\n";
    if (badPlayers > 0 || badChallenges > 0)
        cout << "Rejected records: " << badPlayers << " player line(s), " << badChallenges
             << " challenge line(s)\n";
    cout << "Initial ring: ";
    ring.printRing();

    cout << "\nTURN TRACE\n";
    if (ring.isEmpty())
        cout << "No players are available, so no turns can be played.\n";
    else if (deck.isEmpty())
        cout << "There are no challenges, so no turns can be played.\n";
    playTournament(ring, deck, history);

    int leftover = deck.size();
    cout << "\nFINAL ACTIVE RING\n";
    ring.printRing();
    if (leftover > 0)
        cout << "Unprocessed challenges left in the queue: " << leftover << "\n";

    cout << "\nMATCH HISTORY FORWARD\n";
    history.printForward();
    cout << "\nMATCH HISTORY BACKWARD\n";
    history.printBackward();

    cout << "\nFINAL STATUS: ";
    if (ring.isEmpty())
        cout << "NO_ACTIVE_PLAYERS\n";
    else
        cout << "CHALLENGES_EXHAUSTED\n";
}

void showFormatHelp()
{
    cout << "\nInput file formats\n";
    cout << "players file: one line per team as ID|Name, for example P01|Alpha\n";
    cout << "challenges file: ID|expression|answer, for example C01|(8+4)*2|24\n";
    cout << "Blank lines are ignored. Player IDs must be unique.\n";
    cout << "Rules: correct answer gives 10 points, wrong answer gives a strike,\n";
    cout << "two strikes eliminate a team, invalid cards never change score or strikes.\n";
}

void showComplexity()
{
    cout << "\nComplexity note\n";
    cout << "Stack push and pop: O(1)\n";
    cout << "Queue enqueue and dequeue: O(1)\n";
    cout << "DLL append: O(1), full traversal: O(n)\n";
    cout << "CLL insertion at the end: O(1) using the tail pointer\n";
    cout << "CLL removal with current and previous known: O(1)\n";
}

bool readLine(const string &prompt, string &out)
{
    cout << prompt;
    if (!getline(cin, out))
        return false;
    out = trimText(out);
    return true;
}

int main()
{
    string input;
    while (true)
    {
        cout << "\nCodeArena Menu\n";
        cout << "1. Run tournament from files\n";
        cout << "2. Show input file format\n";
        cout << "3. Show complexity note\n";
        cout << "4. Exit\n";
        if (!readLine("Enter choice: ", input))
            break;

        if (input == "1")
        {
            string pf, cf;
            if (!readLine("Players file (press Enter for players.txt): ", pf))
                break;
            if (!readLine("Challenges file (press Enter for challenges.txt): ", cf))
                break;
            if (pf.empty())
                pf = "players.txt";
            if (cf.empty())
                cf = "challenges.txt";
            runSimulation(pf, cf);
        }
        else if (input == "2")
        {
            showFormatHelp();
        }
        else if (input == "3")
        {
            showComplexity();
        }
        else if (input == "4")
        {
            cout << "Goodbye.\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Please enter 1, 2, 3 or 4.\n";
        }
    }
    return 0;
}
