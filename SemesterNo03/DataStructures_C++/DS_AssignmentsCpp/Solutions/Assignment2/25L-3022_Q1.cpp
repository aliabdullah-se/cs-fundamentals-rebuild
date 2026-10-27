// Q1: CodeSentry, Mini C++ file analyzer and integer interpreter

// Only iostream, fstream and string are used. Stack, Queue and the
// doubly linked symbol table are written by hand using nodes and pointers.

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// small character helpers so that we do not need cctype
bool isDigitChar(char c) { return c >= '0' && c <= '9'; }
bool isAlphaChar(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
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

// Custom Stack (linked nodes)
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

// Custom Queue (linked nodes, FIFO)
template <typename T>
class Queue
{
    struct Node
    {
        T data;
        Node *next;
        Node(const T &d) : data(d), next(nullptr) {}
    };
    Node *frontNode;
    Node *rearNode;
    int count;
    Queue(const Queue &) = delete;
    Queue &operator=(const Queue &) = delete;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}
    ~Queue()
    {
        T dummy;
        while (dequeue(dummy))
        {
        }
    }
    bool isEmpty() const { return frontNode == nullptr; }
    int size() const { return count; }
    void enqueue(const T &v)
    {
        Node *n = new Node(v);
        if (rearNode)
            rearNode->next = n;
        else
            frontNode = n;
        rearNode = n;
        count++;
    }
    bool dequeue(T &out)
    {
        if (isEmpty())
            return false;
        Node *t = frontNode;
        out = t->data;
        frontNode = t->next;
        if (frontNode == nullptr)
            rearNode = nullptr;
        delete t;
        count = count - 1;
        return true;
    }
};

// Basic data carriers
struct Statement
{
    int line = 0;
    string text = "";
    bool hasSemicolon = false;
};

enum TokenType
{
    TK_NUM,
    TK_ID,
    TK_OP,
    TK_LPAREN,
    TK_RPAREN,
    TK_ASSIGN,
    TK_SHIFT
};

struct Token
{
    TokenType type = TK_NUM;
    string text = "";
    int value = 0;
};

enum ErrorKind
{
    ERR_NONE,
    ERR_SYNTAX,
    ERR_SEMANTIC,
    ERR_RUNTIME
};

struct Failure
{
    ErrorKind kind = ERR_NONE;
    string message = "";
};

bool fail(Failure &f, ErrorKind k, const string &m)
{
    f.kind = k;
    f.message = m;
    return false;
}

string kindName(ErrorKind k)
{
    if (k == ERR_SYNTAX)
        return "Syntax error";
    if (k == ERR_SEMANTIC)
        return "Semantic error";
    if (k == ERR_RUNTIME)
        return "Runtime error";
    return "No error";
}

bool isKeyword(const string &s) { return s == "int" || s == "cout"; }

// Symbol table as a Doubly Linked List
struct SymbolNode
{
    string name;
    bool initialized;
    int value;
    int declLine;
    SymbolNode *prev;
    SymbolNode *next;
};

class SymbolTableDLL
{
    SymbolNode *head;
    SymbolNode *tail;
    int count;
    SymbolTableDLL(const SymbolTableDLL &) = delete;
    SymbolTableDLL &operator=(const SymbolTableDLL &) = delete;

    void printNode(const SymbolNode *p) const
    {
        cout << p->name << ", ";
        if (p->initialized)
            cout << p->value;
        else
            cout << "uninitialized";
        cout << ", declared on line " << p->declLine << "\n";
    }

public:
    SymbolTableDLL() : head(nullptr), tail(nullptr), count(0) {}
    ~SymbolTableDLL()
    {
        SymbolNode *p = head;
        while (p != nullptr)
        {
            SymbolNode *nxt = p->next;
            delete p;
            p = nxt;
        }
    }

    SymbolNode *find(const string &name) const
    {
        for (SymbolNode *p = head; p != nullptr; p = p->next)
            if (p->name == name)
                return p;
        return nullptr;
    }

    // returns the new node, or nullptr when the name already exists
    SymbolNode *declare(const string &name, int line)
    {
        if (find(name) != nullptr)
            return nullptr;
        SymbolNode *n = new SymbolNode;
        n->name = name;
        n->initialized = false;
        n->value = 0;
        n->declLine = line;
        n->next = nullptr;
        n->prev = tail;
        if (tail != nullptr)
            tail->next = n;
        else
            head = n;
        tail = n;
        count++;
        return n;
    }

    void printForward() const
    {
        if (head == nullptr)
        {
            cout << "(no variables)\n";
            return;
        }
        for (const SymbolNode *p = head; p != nullptr; p = p->next)
            printNode(p);
    }

    void printBackward() const
    {
        if (tail == nullptr)
        {
            cout << "(no variables)\n";
            return;
        }
        for (const SymbolNode *p = tail; p != nullptr; p = p->prev)
            printNode(p);
    }
};

// Tokenizer: turns the text of one statement into a queue of tokens
bool tokenize(const string &s, Queue<Token> &out, Failure &f)
{
    size_t i = 0;
    while (i < s.size())
    {
        char c = s[i];
        if (isSpaceChar(c))
        {
            i++;
            continue;
        }
        Token t;
        if (isDigitChar(c))
        {
            size_t j = i;
            while (j < s.size() && (isDigitChar(s[j]) || isAlphaChar(s[j])))
                j++;
            string word = s.substr(i, j - i);
            int v = 0;
            for (size_t k = 0; k < word.size(); k++)
            {
                if (!isDigitChar(word[k]))
                    return fail(f, ERR_SYNTAX, "malformed identifier or number '" + word + "'");
                v = v * 10 + (word[k] - '0');
            }
            t.type = TK_NUM;
            t.text = word;
            t.value = v;
            i = j;
        }
        else if (isAlphaChar(c))
        {
            size_t j = i;
            while (j < s.size() && (isDigitChar(s[j]) || isAlphaChar(s[j])))
                j++;
            t.type = TK_ID;
            t.text = s.substr(i, j - i);
            i = j;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        {
            t.type = TK_OP;
            t.text = string(1, c);
            i++;
        }
        else if (c == '(')
        {
            t.type = TK_LPAREN;
            t.text = "(";
            i++;
        }
        else if (c == ')')
        {
            t.type = TK_RPAREN;
            t.text = ")";
            i++;
        }
        else if (c == '=')
        {
            t.type = TK_ASSIGN;
            t.text = "=";
            i++;
        }
        else if (c == '<')
        {
            if (i + 1 < s.size() && s[i + 1] == '<')
            {
                t.type = TK_SHIFT;
                t.text = "<<";
                i += 2;
            }
            else
            {
                return fail(f, ERR_SYNTAX, "single '<' is not supported, expected '<<'");
            }
        }
        else
        {
            return fail(f, ERR_SYNTAX, string("unsupported character '") + c + "'");
        }
        out.enqueue(t);
    }
    return true;
}

// Expression engine: infix to postfix (operator stack), then
// postfix evaluation (value stack). 'n' is used for unary minus.
int precedenceOf(char op)
{
    if (op == 'n')
        return 3;
    if (op == '*' || op == '/' || op == '%')
        return 2;
    return 1;
}

class ExpressionEngine
{
public:
    static bool toPostfix(Queue<Token> &infix, const SymbolTableDLL &table,
                          Queue<Token> &postfix, Failure &f)
    {
        Stack<Token> ops;
        bool expectOperand = true;
        Token t;
        while (infix.dequeue(t))
        {
            if (t.type == TK_NUM)
            {
                if (!expectOperand)
                    return fail(f, ERR_SYNTAX, "missing operator before '" + t.text + "'");
                postfix.enqueue(t);
                expectOperand = false;
            }
            else if (t.type == TK_ID)
            {
                if (!expectOperand)
                    return fail(f, ERR_SYNTAX, "missing operator before '" + t.text + "'");
                if (isKeyword(t.text))
                    return fail(f, ERR_SYNTAX, "keyword '" + t.text + "' cannot be used inside an expression");
                SymbolNode *s = table.find(t.text);
                if (s == nullptr)
                    return fail(f, ERR_SEMANTIC, "variable '" + t.text + "' is used before declaration");
                if (!s->initialized)
                    return fail(f, ERR_SEMANTIC, "variable '" + t.text + "' is declared but uninitialized");
                Token v;
                v.type = TK_NUM;
                v.text = t.text;
                v.value = s->value;
                postfix.enqueue(v);
                expectOperand = false;
            }
            else if (t.type == TK_OP)
            {
                char c = t.text[0];
                if (expectOperand)
                {
                    if (c == '-')
                    {
                        Token u = t;
                        u.text = "n";
                        ops.push(u);
                    }
                    else if (c == '+')
                    {
                        // unary plus does nothing
                    }
                    else
                    {
                        return fail(f, ERR_SYNTAX, string("operator '") + c + "' has no left operand");
                    }
                }
                else
                {
                    Token top;
                    while (ops.peek(top) && top.type == TK_OP && precedenceOf(top.text[0]) >= precedenceOf(c))
                    {
                        ops.pop(top);
                        postfix.enqueue(top);
                    }
                    ops.push(t);
                    expectOperand = true;
                }
            }
            else if (t.type == TK_LPAREN)
            {
                if (!expectOperand)
                    return fail(f, ERR_SYNTAX, "missing operator before '('");
                ops.push(t);
            }
            else if (t.type == TK_RPAREN)
            {
                if (expectOperand)
                    return fail(f, ERR_SYNTAX, "missing operand before ')'");
                Token top;
                bool found = false;
                while (ops.pop(top))
                {
                    if (top.type == TK_LPAREN)
                    {
                        found = true;
                        break;
                    }
                    postfix.enqueue(top);
                }
                if (!found)
                    return fail(f, ERR_SYNTAX, "unmatched ')' in expression");
                expectOperand = false;
            }
            else
            {
                return fail(f, ERR_SYNTAX, "unexpected '" + t.text + "' inside expression");
            }
        }
        if (expectOperand)
            return fail(f, ERR_SYNTAX, "expression is empty or ends without an operand");
        Token top;
        while (ops.pop(top))
        {
            if (top.type == TK_LPAREN)
                return fail(f, ERR_SYNTAX, "unmatched '(' in expression");
            postfix.enqueue(top);
        }
        return true;
    }

    static bool evalPostfix(Queue<Token> &postfix, int &result, Failure &f)
    {
        Stack<int> vals;
        Token t;
        while (postfix.dequeue(t))
        {
            if (t.type == TK_NUM)
            {
                vals.push(t.value);
                continue;
            }
            char c = t.text[0];
            if (c == 'n')
            {
                int a;
                if (!vals.pop(a))
                    return fail(f, ERR_SYNTAX, "malformed expression");
                vals.push(0 - a);
                continue;
            }
            int a, b;
            if (!vals.pop(b) || !vals.pop(a))
                return fail(f, ERR_SYNTAX, "malformed expression");
            if (c == '+')
                vals.push(a + b);
            else if (c == '-')
                vals.push(a - b);
            else if (c == '*')
                vals.push(a * b);
            else if (c == '/')
            {
                if (b == 0)
                    return fail(f, ERR_RUNTIME, "division by zero");
                vals.push(a / b);
            }
            else if (c == '%')
            {
                if (b == 0)
                    return fail(f, ERR_RUNTIME, "modulo by zero");
                vals.push(a % b);
            }
        }
        int r;
        if (!vals.pop(r) || !vals.isEmpty())
            return fail(f, ERR_SYNTAX, "malformed expression");
        result = r;
        return true;
    }

    static bool evaluate(Queue<Token> &infix, const SymbolTableDLL &table, int &result, Failure &f)
    {
        Queue<Token> postfix;
        if (!toPostfix(infix, table, postfix, f))
            return false;
        return evalPostfix(postfix, result, f);
    }
};

// Statement validation and execution
bool executeStatement(const Statement &st, SymbolTableDLL &table, Queue<int> &output, Failure &f)
{
    if (!st.hasSemicolon)
        return fail(f, ERR_SYNTAX, "missing semicolon at the end of the statement");
    if (st.text.empty())
        return fail(f, ERR_SYNTAX, "empty statement is not supported");

    Queue<Token> tokens;
    if (!tokenize(st.text, tokens, f))
        return false;

    Token first;
    if (!tokens.dequeue(first))
        return fail(f, ERR_SYNTAX, "empty statement is not supported");

    // int x;   or   int x = expression;
    if (first.type == TK_ID && first.text == "int")
    {
        Token name;
        if (!tokens.dequeue(name) || name.type != TK_ID || isKeyword(name.text))
            return fail(f, ERR_SYNTAX, "expected a valid variable name after int");
        Token next;
        if (!tokens.dequeue(next))
        {
            if (table.declare(name.text, st.line) == nullptr)
                return fail(f, ERR_SEMANTIC, "duplicate declaration of '" + name.text + "'");
            return true;
        }
        if (next.type != TK_ASSIGN)
            return fail(f, ERR_SYNTAX, "expected '=' or end of statement after the variable name");
        SymbolNode *sym = table.declare(name.text, st.line);
        if (sym == nullptr)
            return fail(f, ERR_SEMANTIC, "duplicate declaration of '" + name.text + "'");
        int value;
        if (!ExpressionEngine::evaluate(tokens, table, value, f))
            return false;
        sym->value = value;
        sym->initialized = true;
        return true;
    }

    // cout << expression;
    if (first.type == TK_ID && first.text == "cout")
    {
        Token shift;
        if (!tokens.dequeue(shift) || shift.type != TK_SHIFT)
            return fail(f, ERR_SYNTAX, "expected '<<' after cout");
        int value;
        if (!ExpressionEngine::evaluate(tokens, table, value, f))
            return false;
        output.enqueue(value);
        return true;
    }

    // x = expression;
    if (first.type == TK_ID && !isKeyword(first.text))
    {
        Token eq;
        if (!tokens.dequeue(eq) || eq.type != TK_ASSIGN)
            return fail(f, ERR_SYNTAX, "unsupported statement");
        SymbolNode *sym = table.find(first.text);
        if (sym == nullptr)
            return fail(f, ERR_SEMANTIC, "variable '" + first.text + "' is used before declaration");
        int value;
        if (!ExpressionEngine::evaluate(tokens, table, value, f))
            return false;
        sym->value = value;
        sym->initialized = true;
        return true;
    }

    return fail(f, ERR_SYNTAX, "unsupported statement");
}

// Reading the file: comment removal, delimiter scan, statement queue
struct DelimInfo
{
    char ch = ' ';
    int line = 0;
    int col = 0;
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

string placeText(int line, int col)
{
    return "Line " + to_string(line) + ", Column " + to_string(col);
}

// returns false and fills msg on the first delimiter problem in this line
bool scanDelimiters(const string &s, int lineNo, Stack<DelimInfo> &st, string &msg)
{
    for (size_t k = 0; k < s.size(); k++)
    {
        char c = s[k];
        int col = (int)k + 1;
        if (isOpener(c))
        {
            DelimInfo d;
            d.ch = c;
            d.line = lineNo;
            d.col = col;
            st.push(d);
        }
        else if (isCloser(c))
        {
            DelimInfo top;
            if (!st.pop(top))
            {
                msg = placeText(lineNo, col) + ": closing '" + string(1, c) +
                      "' found but there is no matching opener";
                return false;
            }
            char expected = closerFor(top.ch);
            if (c != expected)
            {
                msg = placeText(lineNo, col) + ": found '" + string(1, c) + "' but expected '" +
                      string(1, expected) + "' to close '" + string(1, top.ch) + "' opened at " +
                      placeText(top.line, top.col);
                return false;
            }
        }
    }
    return true;
}

// split one cleaned line into statements. A statement ends at ';'
// Braces end a piece too, but they are not statements themselves.
void splitIntoStatements(const string &s, int lineNo, Queue<Statement> &q)
{
    string piece = "";
    for (size_t k = 0; k < s.size(); k++)
    {
        char c = s[k];
        if (c == ';')
        {
            Statement st;
            st.line = lineNo;
            st.text = trimText(piece);
            st.hasSemicolon = true;
            q.enqueue(st);
            piece = "";
        }
        else if (c == '{' || c == '}')
        {
            string t = trimText(piece);
            if (!t.empty())
            {
                Statement st;
                st.line = lineNo;
                st.text = t;
                st.hasSemicolon = false;
                q.enqueue(st);
            }
            piece = "";
        }
        else
        {
            piece += c;
        }
    }
    string t = trimText(piece);
    if (!t.empty())
    {
        Statement st;
        st.line = lineNo;
        st.text = t;
        st.hasSemicolon = false;
        q.enqueue(st);
    }
}

// returns false only when the file cannot be opened
bool loadSource(const string &fname, Queue<Statement> &q, bool &structuralOk, string &structMsg)
{
    ifstream in(fname.c_str());
    if (!in.is_open())
        return false;

    Stack<DelimInfo> delims;
    structuralOk = true;
    structMsg = "";
    string raw;
    int lineNo = 0;

    while (getline(in, raw))
    {
        lineNo++;
        size_t pos = raw.find("//");
        if (pos != string::npos)
            raw = raw.substr(0, pos);
        if (trimText(raw).empty())
            continue;

        string msg;
        if (!scanDelimiters(raw, lineNo, delims, msg))
        {
            structuralOk = false;
            structMsg = msg;
            break;
        }
        splitIntoStatements(raw, lineNo, q);
    }

    if (structuralOk && !delims.isEmpty())
    {
        DelimInfo top;
        delims.peek(top);
        structuralOk = false;
        structMsg = placeText(top.line, top.col) + ": '" + string(1, top.ch) +
                    "' was never closed, expected '" + string(1, closerFor(top.ch)) +
                    "' before the end of the file";
    }
    in.close();
    return true;
}

// Report
void analyzeFile(const string &fname)
{
    Queue<Statement> pending;
    bool structuralOk = true;
    string structMsg;

    cout << "\nFILE: " << fname << "\n";
    if (!loadSource(fname, pending, structuralOk, structMsg))
    {
        cout << "Could not open the file '" << fname << "'. Check the name and path.\n";
        return;
    }

    if (!structuralOk)
    {
        cout << "STRUCTURAL CHECK: FAIL\n";
        cout << structMsg << "\n";
        cout << "Execution skipped because of the structural error.\n";
        cout << "FINAL STATUS: FAILED\n";
        return;
    }
    cout << "STRUCTURAL CHECK: PASS\n";

    SymbolTableDLL table;
    Queue<int> output;
    bool success = true;

    cout << "EXECUTION TRACE\n";
    if (pending.isEmpty())
        cout << "No executable statements found.\n";

    Statement st;
    while (pending.dequeue(st))
    {
        Failure f;
        string shown = st.text + (st.hasSemicolon ? ";" : "");
        if (executeStatement(st, table, output, f))
        {
            cout << "Line " << st.line << ": " << shown << " => OK\n";
        }
        else
        {
            cout << "Line " << st.line << ": " << shown << " => ERROR (" << kindName(f.kind)
                 << "): " << f.message << "\n";
            success = false;
            if (pending.size() > 0)
                cout << "Execution stopped, " << pending.size() << " statement(s) were not run.\n";
            break;
        }
    }

    cout << "PROGRAM OUTPUT\n";
    if (output.isEmpty())
        cout << "(no output)\n";
    int v;
    while (output.dequeue(v))
        cout << v << "\n";

    cout << "SYMBOL TABLE FORWARD\n";
    table.printForward();
    cout << "SYMBOL TABLE BACKWARD\n";
    table.printBackward();

    cout << "FINAL STATUS: " << (success ? "SUCCESS" : "FAILED") << "\n";
}

void showRules()
{
    cout << "\nMini C++ rules supported by CodeSentry\n";
    cout << "1. int name;\n";
    cout << "2. int name = expression;\n";
    cout << "3. name = expression;\n";
    cout << "4. cout << expression;\n";
    cout << "5. { and } on their own or around statements, // for comments\n";
    cout << "Expressions use integers, variables, ( ), + - * / %\n";
    cout << "Every statement must end with a semicolon.\n";
}

// reads a whole line, returns false when the input has ended
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
        cout << "\nCodeSentry Menu\n";
        cout << "1. Analyze a Mini C++ file\n";
        cout << "2. Show supported rules\n";
        cout << "3. Exit\n";
        if (!readLine("Enter choice: ", input))
            break;

        if (input == "1")
        {
            string fname;
            if (!readLine("Enter source file name: ", fname))
                break;
            if (fname.empty())
            {
                cout << "File name cannot be empty.\n";
                continue;
            }
            analyzeFile(fname);
        }
        else if (input == "2")
        {
            showRules();
        }
        else if (input == "3")
        {
            cout << "Goodbye.\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Please enter 1, 2 or 3.\n";
        }
    }
    return 0;
}
