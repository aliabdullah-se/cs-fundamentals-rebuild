#include <iostream>
#include <string>
using namespace std;

// formats a double to 2 decimal places by hand since iomanip isn't allowed
string fmt2(double v)
{
    bool isNegative = false;
    if (v < 0)
    {
        isNegative = true;
        v = -v;
    }
    long long scaledVal = (long long)(v * 100.0 + 0.5);
    long long wholePart = scaledVal / 100;
    long long fracPart = scaledVal % 100;
    string out = "";
    if (isNegative)
        out += "-";
    out += to_string(wholePart);
    out += ".";
    if (fracPart < 10)
        out += "0";
    out += to_string(fracPart);
    return out;
}

// input helpers, do-while validated with cin.fail() / cin.ignore()

int readInt(const string &prompt)
{
    int num;
    bool ok;
    do
    {
        cout << prompt;
        cin >> num;
        if (cin.fail())
        {
            cout << "Invalid input, please enter a whole number." << endl;
            cin.clear();
            cin.ignore(1000000, '\n');
            ok = false;
        }
        else
        {
            cin.ignore(1000000, '\n');
            ok = true;
        }
    } while (!ok);
    return num;
}

int readIntInRange(const string &prompt, int lo, int hi)
{
    int num;
    bool ok;
    do
    {
        num = readInt(prompt);
        if (num < lo || num > hi)
        {
            cout << "Value must be between " << lo << " and " << hi << endl;
            ok = false;
        }
        else
        {
            ok = true;
        }
    } while (!ok);
    return num;
}

double readDouble(const string &prompt)
{
    double num;
    bool ok;
    do
    {
        cout << prompt;
        cin >> num;
        if (cin.fail())
        {
            cout << "Invalid input, please enter a number." << endl;
            cin.clear();
            cin.ignore(1000000, '\n');
            ok = false;
        }
        else
        {
            cin.ignore(1000000, '\n');
            ok = true;
        }
    } while (!ok);
    return num;
}

string readLine(const string &prompt)
{
    string line;
    bool ok;
    do
    {
        cout << prompt;
        getline(cin, line);
        if (line.length() == 0)
        {
            cout << "Input cannot be empty." << endl;
            ok = false;
        }
        else
        {
            ok = true;
        }
    } while (!ok);
    return line;
}

//  ArrayList, our own dynamic array so no STL vector is touched

template <typename T>
class ArrayList
{
private:
    T *arr;
    int cap;
    int len;

    void grow(int newCap)
    {
        T *fresh = new T[newCap];
        for (int i = 0; i < len; i++)
            fresh[i] = arr[i];
        delete[] arr;
        arr = fresh;
        cap = newCap;
    }

public:
    ArrayList(int startCap = 4)
    {
        cap = startCap;
        if (cap < 1)
            cap = 1;
        len = 0;
        arr = new T[cap];
    }

    ~ArrayList() { delete[] arr; }

    ArrayList(const ArrayList &other)
    {
        cap = other.cap;
        len = other.len;
        arr = new T[cap];
        for (int i = 0; i < len; i++)
            arr[i] = other.arr[i];
    }

    ArrayList &operator=(const ArrayList &other)
    {
        if (this != &other)
        {
            delete[] arr;
            cap = other.cap;
            len = other.len;
            arr = new T[cap];
            for (int i = 0; i < len; i++)
                arr[i] = other.arr[i];
        }
        return *this;
    }

    // doubles the capacity when full, so this is O(1) amortised
    void add(const T &item)
    {
        if (len == cap)
            grow(cap * 2);
        arr[len++] = item;
    }

    // shifts everything after the index one spot left, O(n)
    void removeAt(int index)
    {
        if (index < 0 || index >= len)
            return;
        for (int i = index; i < len - 1; i++)
            arr[i] = arr[i + 1];
        len--;
    }

    T &get(int index) { return arr[index]; }
    const T &get(int index) const { return arr[index]; }
    void set(int index, const T &val) { arr[index] = val; }

    int getSize() const { return len; }
    bool isEmpty() const { return len == 0; }
};

// Candidate

enum class Status
{
    ACTIVE,
    SELECTED,
    WITHDRAWN
};

string statusToString(Status s)
{
    if (s == Status::ACTIVE)
        return "ACTIVE";
    if (s == Status::SELECTED)
        return "SELECTED";
    if (s == Status::WITHDRAWN)
        return "WITHDRAWN";
    return "UNKNOWN";
}

class Candidate
{
public:
    int id;
    string name;
    string university;
    double cgpa;
    int experience;
    double techScore;
    double interviewScore;
    ArrayList<string> skills;
    Status status;

    Candidate()
        : id(-1), cgpa(0), experience(0), techScore(0),
          interviewScore(0), skills(4), status(Status::ACTIVE) {}

    Candidate(int id_, string name_, string uni_, double cgpa_, int exp_,
              double tech_, double interview_, ArrayList<string> skills_)
        : id(id_), name(name_), university(uni_), cgpa(cgpa_),
          experience(exp_), techScore(tech_), interviewScore(interview_),
          skills(skills_), status(Status::ACTIVE) {}

    // O(k), k being how many skills this candidate carries
    bool hasSkill(const string &skill) const
    {
        for (int j = 0; j < skills.getSize(); j++)
        {
            if (skills.get(j) == skill)
                return true;
        }
        return false;
    }

    // counts how many of the candidate's skills are on our fixed relevant list
    // O(k * R) but R is constant so really just O(k)
    int relevantSkillCount() const
    {
        static const string relevant[] = {
            "C++", "Python", "Machine Learning", "Deep Learning", "Java",
            "Data Structures", "SQL", "TensorFlow", "PyTorch", "NLP",
            "Computer Vision", "Statistics"};
        int relevantTotal = 12;
        int matchCount = 0;
        for (int outerIdx = 0; outerIdx < skills.getSize(); outerIdx++)
        {
            for (int innerIdx = 0; innerIdx < relevantTotal; innerIdx++)
            {
                if (skills.get(outerIdx) == relevant[innerIdx])
                {
                    matchCount++;
                    break;
                }
            }
        }
        return matchCount;
    }

    // weighted sum, O(1)
    double finalScore() const
    {
        double cgpaPercent = (cgpa / 4.0) * 100.0;
        return 0.40 * cgpaPercent + 0.35 * techScore + 0.25 * interviewScore;
    }

    void print() const
    {
        cout << "  [" << id << "] " << name << " | " << university
             << " | CGPA: " << fmt2(cgpa)
             << " | Exp: " << experience << "y"
             << " | Tech: " << fmt2(techScore)
             << " | Interview: " << fmt2(interviewScore)
             << " | Skills: ";
        for (int j = 0; j < skills.getSize(); j++)
        {
            cout << skills.get(j);
            if (j != skills.getSize() - 1)
                cout << ", ";
        }
        cout << " | Status: " << statusToString(status) << endl;
    }
};

//  Stage, one node of the singly linked list

class Stage
{
public:
    string stageName;
    ArrayList<Candidate> candidates;
    Stage *next;

    Stage(const string &name) : stageName(name), candidates(8), next(NULL) {}
};

//  RecruitmentPipeline

class RecruitmentPipeline
{
private:
    Stage *head;
    ArrayList<Candidate> exitedCandidates; // people who withdrew, kept outside the pipeline

    // small helper used only inside checkPipelineIntegrity
    struct DuplicateRecord
    {
        int candidateId;
        ArrayList<string> stagesFound;
        DuplicateRecord() : candidateId(-1), stagesFound(4) {}
        DuplicateRecord(int id_) : candidateId(id_), stagesFound(4) {}
    };

public:
    RecruitmentPipeline() : head(NULL), exitedCandidates(8) {}

    ~RecruitmentPipeline()
    {
        Stage *walker = head;
        while (walker)
        {
            Stage *nxt = walker->next;
            delete walker;
            walker = nxt;
        }
    }

    // walks to the tail and attaches, O(S) where S is the number of stages
    void addStageAtEnd(const string &name)
    {
        Stage *fresh = new Stage(name);
        if (!head)
        {
            head = fresh;
            return;
        }
        Stage *trav = head;
        while (trav->next)
            trav = trav->next;
        trav->next = fresh;
    }

    // O(S)
    Stage *findStage(const string &name)
    {
        Stage *runner = head;
        while (runner)
        {
            if (runner->stageName == name)
                return runner;
            runner = runner->next;
        }
        return NULL;
    }

    // O(S), used so we can unlink a stage sitting in the middle
    Stage *findPrevStage(const string &name)
    {
        if (!head)
            return NULL;
        if (head->stageName == name)
            return NULL;
        Stage *runner = head;
        while (runner->next)
        {
            if (runner->next->stageName == name)
                return runner;
            runner = runner->next;
        }
        return NULL;
    }

    // finds afterStageName then splices a fresh stage right after it, O(S)
    void insertStage(const string &newStageName, const string &afterStageName)
    {
        Stage *anchor = findStage(afterStageName);
        if (!anchor)
        {
            cout << "ERROR: Stage \"" << afterStageName << "\" does not exist." << endl;
            return;
        }
        if (findStage(newStageName))
        {
            cout << "ERROR: Stage \"" << newStageName << "\" already exists." << endl;
            return;
        }
        Stage *freshStage = new Stage(newStageName);
        freshStage->next = anchor->next;
        anchor->next = freshStage;
        cout << "Stage \"" << newStageName << "\" inserted after \"" << afterStageName << "\"." << endl;
    }

    // only allowed to disappear if it's holding zero candidates, O(S)
    void removeStage(const string &stageName)
    {
        Stage *victim = findStage(stageName);
        if (!victim)
        {
            cout << "ERROR: Stage \"" << stageName << "\" not found." << endl;
            return;
        }
        if (!victim->candidates.isEmpty())
        {
            cout << "REJECTED: Stage \"" << stageName << "\" still has "
                 << victim->candidates.getSize() << " candidate(s)." << endl;
            return;
        }
        if (victim == head)
        {
            head = head->next;
        }
        else
        {
            Stage *before = findPrevStage(stageName);
            before->next = victim->next;
        }
        delete victim;
        cout << "Stage \"" << stageName << "\" removed." << endl;
    }

    // flips every next pointer, the ArrayLists inside each stage stay untouched, O(S)
    void reversePipeline()
    {
        Stage *prevNode = NULL;
        Stage *currNode = head;
        while (currNode)
        {
            Stage *nextNode = currNode->next;
            currNode->next = prevNode;
            prevNode = currNode;
            currNode = nextNode;
        }
        head = prevNode;
        cout << "Pipeline order reversed." << endl;
    }

    // Floyd's tortoise and hare, in-place, no extra ArrayList used, O(S) time, O(1) space
    bool hasCycle()
    {
        Stage *slow = head;
        Stage *fast = head;
        while (fast && fast->next)
        {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast)
                return true;
        }
        return false;
    }

    // hunts through every stage's list until the id turns up, worst case O(N), N being total candidates
    Stage *locateCandidate(int id, int &idxOut)
    {
        Stage *hunter = head;
        while (hunter)
        {
            for (int pos = 0; pos < hunter->candidates.getSize(); pos++)
            {
                if (hunter->candidates.get(pos).id == id)
                {
                    idxOut = pos;
                    return hunter;
                }
            }
            hunter = hunter->next;
        }
        idxOut = -1;
        return NULL;
    }

    // O(W), W being how many people have withdrawn so far
    int findExitedIndex(int id)
    {
        for (int k = 0; k < exitedCandidates.getSize(); k++)
        {
            if (exitedCandidates.get(k).id == id)
                return k;
        }
        return -1;
    }

    // checks the id isn't already taken anywhere, active or withdrawn, before adding, O(N)
    bool addCandidate(const string &stageName, const Candidate &c)
    {
        int idx;
        Stage *existing = locateCandidate(c.id, idx);
        if (existing != NULL || findExitedIndex(c.id) != -1)
        {
            cout << "ERROR: Candidate ID " << c.id << " already exists." << endl;
            return false;
        }
        Stage *landing = findStage(stageName);
        if (!landing)
        {
            cout << "ERROR: Stage \"" << stageName << "\" not found." << endl;
            return false;
        }
        landing->candidates.add(c);
        cout << "Candidate " << c.id << " added to \"" << stageName << "\"." << endl;
        return true;
    }

    // no hash map allowed, so a small ArrayList tracks (id, which stages it showed up in)
    // using plain linear search, worst case O(N^2)
    bool checkPipelineIntegrity()
    {
        ArrayList<DuplicateRecord> tracker(16);
        Stage *scanner = head;
        while (scanner)
        {
            for (int m = 0; m < scanner->candidates.getSize(); m++)
            {
                int cid = scanner->candidates.get(m).id;
                int hitIndex = -1;
                for (int n = 0; n < tracker.getSize(); n++)
                {
                    if (tracker.get(n).candidateId == cid)
                    {
                        hitIndex = n;
                        break;
                    }
                }
                if (hitIndex == -1)
                {
                    DuplicateRecord rec(cid);
                    rec.stagesFound.add(scanner->stageName);
                    tracker.add(rec);
                }
                else
                {
                    tracker.get(hitIndex).stagesFound.add(scanner->stageName);
                }
            }
            scanner = scanner->next;
        }

        bool corrupted = false;
        for (int q = 0; q < tracker.getSize(); q++)
        {
            if (tracker.get(q).stagesFound.getSize() > 1)
            {
                if (!corrupted)
                {
                    cout << "PIPELINE CORRUPTED" << endl;
                    corrupted = true;
                }
                cout << "Duplicate Candidate: ID = " << tracker.get(q).candidateId << endl;
                cout << "Found in: ";
                for (int r = 0; r < tracker.get(q).stagesFound.getSize(); r++)
                {
                    cout << tracker.get(q).stagesFound.get(r);
                    if (r != tracker.get(q).stagesFound.getSize() - 1)
                        cout << ", ";
                }
                cout << endl;
            }
        }
        if (!corrupted)
            cout << "Pipeline integrity OK, no duplicates found." << endl;
        return corrupted;
    }

    // finds the candidate wherever they sit (O(N)), then finds the destination (O(S))
    bool moveCandidate(int candidateID, const string &destinationStage)
    {
        int idx;
        Stage *origin = locateCandidate(candidateID, idx);
        if (!origin)
        {
            cout << "ERROR: Candidate " << candidateID << " not found in pipeline." << endl;
            return false;
        }
        Stage *landing = findStage(destinationStage);
        if (!landing)
        {
            cout << "ERROR: Destination stage \"" << destinationStage << "\" does not exist." << endl;
            return false;
        }
        if (origin == landing)
        {
            cout << "Candidate " << candidateID << " is already in \"" << destinationStage << "\"." << endl;
            return false;
        }
        Candidate moved = origin->candidates.get(idx);
        origin->candidates.removeAt(idx);
        landing->candidates.add(moved);
        cout << "Candidate " << candidateID << " moved: " << origin->stageName
             << " -> " << landing->stageName << endl;
        return true;
    }

    // pulls the candidate out of wherever they are and parks them in exitedCandidates, O(N)
    void withdrawCandidate(int candidateID)
    {
        int idx;
        Stage *holder = locateCandidate(candidateID, idx);
        if (holder)
        {
            Candidate leaving = holder->candidates.get(idx);
            holder->candidates.removeAt(idx);
            leaving.status = Status::WITHDRAWN;
            exitedCandidates.add(leaving);
            cout << "Candidate " << candidateID << " withdrawn from \"" << holder->stageName << "\"." << endl;
            return;
        }
        if (findExitedIndex(candidateID) != -1)
        {
            cout << "Candidate has already withdrawn." << endl;
            return;
        }
        cout << "Candidate not found." << endl;
    }

    // technical score can only be touched while sitting in Technical Interview, O(N)
    void updateTechnicalScore(int candidateID, double newScore)
    {
        int idx;
        Stage *place = locateCandidate(candidateID, idx);
        if (!place)
        {
            cout << "Candidate not found." << endl;
            return;
        }
        if (place->stageName != "Technical Interview")
        {
            cout << "REJECTED: Technical Score can only be updated in \"Technical Interview\" "
                 << "(currently in \"" << place->stageName << "\")." << endl;
            return;
        }
        place->candidates.get(idx).techScore = newScore;
        cout << "Candidate " << candidateID << " Technical Score updated to " << newScore << endl;
    }

    // same idea but for HR Interview and interview score, O(N)
    void updateInterviewScore(int candidateID, double newScore)
    {
        int idx;
        Stage *spot = locateCandidate(candidateID, idx);
        if (!spot)
        {
            cout << "Candidate not found." << endl;
            return;
        }
        if (spot->stageName != "HR Interview")
        {
            cout << "REJECTED: Interview Score can only be updated in \"HR Interview\" "
                 << "(currently in \"" << spot->stageName << "\")." << endl;
            return;
        }
        spot->candidates.get(idx).interviewScore = newScore;
        cout << "Candidate " << candidateID << " Interview Score updated to " << newScore << endl;
    }

    // snapshots every stage's size first, then only scans the original indices back to front so
    // removeAt shifting things left never disturbs an index still waiting to be checked, meaning
    // nobody hops more than one stage in a single call, O(N + S) time, O(S) space for the snapshot
    void promoteEligibleCandidates()
    {
        struct Checkpoint
        {
            Stage *stagePtr;
            int sizeAtStart;
        };
        ArrayList<Checkpoint> checkpoints(8);
        Stage *scanner = head;
        while (scanner)
        {
            Checkpoint cp;
            cp.stagePtr = scanner;
            cp.sizeAtStart = scanner->candidates.getSize();
            checkpoints.add(cp);
            scanner = scanner->next;
        }

        int totalPromoted = 0;
        for (int p = 0; p < checkpoints.getSize(); p++)
        {
            Stage *stage = checkpoints.get(p).stagePtr;
            int origSize = checkpoints.get(p).sizeAtStart;
            if (!stage->next)
                continue;

            for (int idx = origSize - 1; idx >= 0; idx--)
            {
                Candidate &c = stage->candidates.get(idx);
                bool eligible = false;

                if (stage->stageName == "Applied")
                {
                    eligible = (c.cgpa >= 3.0);
                }
                else if (stage->stageName == "Screening")
                {
                    eligible = (c.cgpa >= 3.2 && c.relevantSkillCount() >= 2);
                }
                else if (stage->stageName == "Technical Interview")
                {
                    eligible = (c.techScore >= 70);
                }
                else if (stage->stageName == "HR Interview")
                {
                    eligible = (c.techScore >= 80 && c.interviewScore >= 75);
                }

                if (eligible)
                {
                    Candidate moved = c;
                    if (stage->next->stageName == "Selected")
                        moved.status = Status::SELECTED;
                    stage->candidates.removeAt(idx);
                    stage->next->candidates.add(moved);
                    cout << "Promoted candidate " << moved.id << ": "
                         << stage->stageName << " -> " << stage->next->stageName << endl;
                    totalPromoted++;
                }
            }
        }
        cout << "promoteEligibleCandidates() complete, total promoted: " << totalPromoted << endl;
    }

    // Final Score = 0.40*CGPA(as %) + 0.35*Technical + 0.25*Interview
    // ties broken by higher technical, then higher cgpa, then smaller id
    // O(N) since every candidate in every stage gets looked at once
    void getBestCandidate()
    {
        Stage *scanner = head;
        bool found = false;
        Candidate champion;
        while (scanner)
        {
            for (int m = 0; m < scanner->candidates.getSize(); m++)
            {
                const Candidate &c = scanner->candidates.get(m);
                if (!found)
                {
                    champion = c;
                    found = true;
                    continue;
                }
                double champScore = champion.finalScore();
                double cScore = c.finalScore();
                bool better = false;
                if (cScore > champScore)
                {
                    better = true;
                }
                else if (cScore == champScore)
                {
                    if (c.techScore > champion.techScore)
                    {
                        better = true;
                    }
                    else if (c.techScore == champion.techScore)
                    {
                        if (c.cgpa > champion.cgpa)
                        {
                            better = true;
                        }
                        else if (c.cgpa == champion.cgpa && c.id < champion.id)
                        {
                            better = true;
                        }
                    }
                }
                if (better)
                    champion = c;
            }
            scanner = scanner->next;
        }
        if (!found)
        {
            cout << "No candidates in pipeline." << endl;
            return;
        }
        cout << "Best Candidate (Final Score = " << fmt2(champion.finalScore()) << "):" << endl;
        champion.print();
    }

    // walks stage by stage in order so matches naturally come out in pipeline order
    // O(N * k), k being the average number of skills per candidate
    void findCandidatesBySkill(const string &skill)
    {
        cout << "Candidates with skill \"" << skill << "\":" << endl;
        bool any = false;
        Stage *scanner = head;
        while (scanner)
        {
            for (int n = 0; n < scanner->candidates.getSize(); n++)
            {
                const Candidate &c = scanner->candidates.get(n);
                if (c.hasSkill(skill))
                {
                    cout << "  (" << scanner->stageName << ") ";
                    c.print();
                    any = true;
                }
            }
            scanner = scanner->next;
        }
        if (!any)
            cout << "  None found." << endl;
    }

    // one pass over the stages, earliest stage wins any tie, O(S)
    void getMostCrowdedStage()
    {
        if (!head)
        {
            cout << "Pipeline is empty." << endl;
            return;
        }
        Stage *champion = head;
        Stage *rover = head->next;
        while (rover)
        {
            if (rover->candidates.getSize() > champion->candidates.getSize())
                champion = rover;
            rover = rover->next;
        }
        cout << "Most crowded stage: \"" << champion->stageName << "\" with "
             << champion->candidates.getSize() << " candidate(s)." << endl;
    }

    // one pass over the stages, and inside that one pass over each stage's candidates, O(N + S)
    void displayStatistics()
    {
        cout << endl
             << "PIPELINE STATISTICS" << endl;
        int totalCandidates = 0;
        Stage *scanner = head;
        while (scanner)
        {
            int n = scanner->candidates.getSize();
            double sumCgpa = 0, sumTech = 0, sumInterview = 0;
            int techCount = 0, interviewCount = 0;
            for (int i = 0; i < n; i++)
            {
                const Candidate &c = scanner->candidates.get(i);
                sumCgpa += c.cgpa;
                if (c.techScore > 0)
                {
                    sumTech += c.techScore;
                    techCount++;
                }
                if (c.interviewScore > 0)
                {
                    sumInterview += c.interviewScore;
                    interviewCount++;
                }
            }
            cout << "Stage: " << scanner->stageName << endl;
            cout << "  Candidates: " << n << endl;
            if (n > 0)
            {
                double avgTech = 0.0;
                if (techCount > 0)
                    avgTech = sumTech / techCount;
                double avgInterview = 0.0;
                if (interviewCount > 0)
                    avgInterview = sumInterview / interviewCount;
                cout << "  Avg CGPA: " << fmt2(sumCgpa / n) << endl;
                cout << "  Avg Technical Score: " << fmt2(avgTech) << endl;
                cout << "  Avg Interview Score: " << fmt2(avgInterview) << endl;
            }
            totalCandidates += n;
            scanner = scanner->next;
        }
        cout << "Total candidates currently in pipeline: " << totalCandidates << endl;
        cout << "Withdrawn candidates (outside pipeline): " << exitedCandidates.getSize() << endl;
    }

    void printPipeline()
    {
        cout << "Pipeline order: ";
        Stage *scanner = head;
        while (scanner)
        {
            cout << scanner->stageName << " (" << scanner->candidates.getSize() << ")";
            if (scanner->next)
                cout << " -> ";
            scanner = scanner->next;
        }
        cout << endl;
    }
};

void loadSampleData(RecruitmentPipeline &pipeline)
{
    ArrayList<string> s1(4);
    s1.add("C++");
    s1.add("Python");
    pipeline.addCandidate("Applied", Candidate(101, "Ali Ahmed", "FAST", 3.1, 0, 0, 0, s1));

    ArrayList<string> s2(2);
    s2.add("Java");
    pipeline.addCandidate("Applied", Candidate(102, "Sara Khan", "LUMS", 2.8, 1, 0, 0, s2));

    ArrayList<string> s3(4);
    s3.add("Python");
    s3.add("Machine Learning");
    s3.add("C++");
    pipeline.addCandidate("Applied", Candidate(103, "Bilal Tariq", "NUST", 3.4, 0, 0, 0, s3));

    ArrayList<string> s4(4);
    s4.add("Python");
    s4.add("Deep Learning");
    pipeline.addCandidate("Screening", Candidate(104, "Hina Riaz", "FAST", 3.5, 1, 0, 0, s4));

    ArrayList<string> s5(4);
    s5.add("C++");
    s5.add("Data Structures");
    pipeline.addCandidate("Screening", Candidate(105, "Usman Javed", "GIKI", 3.3, 2, 0, 0, s5));

    ArrayList<string> s6(4);
    s6.add("Python");
    s6.add("NLP");
    pipeline.addCandidate("Technical Interview", Candidate(106, "Ayesha Noor", "FAST", 3.6, 1, 65, 0, s6));

    ArrayList<string> s7(4);
    s7.add("C++");
    s7.add("TensorFlow");
    pipeline.addCandidate("Technical Interview", Candidate(107, "Danish Iqbal", "UET", 3.0, 0, 75, 0, s7));

    ArrayList<string> s8(4);
    s8.add("Python");
    s8.add("Machine Learning");
    s8.add("PyTorch");
    pipeline.addCandidate("Technical Interview", Candidate(108, "Zara Malik", "FAST", 3.8, 2, 60, 0, s8));

    ArrayList<string> s9(4);
    s9.add("C++");
    s9.add("Computer Vision");
    pipeline.addCandidate("HR Interview", Candidate(109, "Hamza Sheikh", "LUMS", 3.9, 3, 88, 82, s9));

    ArrayList<string> s10(4);
    s10.add("Java");
    s10.add("SQL");
    pipeline.addCandidate("HR Interview", Candidate(110, "Mahnoor Fatima", "NUST", 3.2, 1, 70, 60, s10));
}

void runDemoScenario(RecruitmentPipeline &pipeline)
{
    cout << endl
         << "loading sample candidates 101 to 110..." << endl;
    loadSampleData(pipeline);
    pipeline.printPipeline();
    pipeline.checkPipelineIntegrity();

    cout << endl
         << "step 1: updateTechnicalScore(108, 91)" << endl;
    pipeline.updateTechnicalScore(108, 91);

    cout << endl
         << "step 2: moveCandidate(103, Screening)" << endl;
    pipeline.moveCandidate(103, "Screening");

    cout << endl
         << "step 3: insertStage(Online Assessment, Screening)" << endl;
    pipeline.insertStage("Online Assessment", "Screening");
    pipeline.printPipeline();

    cout << endl
         << "step 4: moveCandidate(106, Online Assessment)" << endl;
    pipeline.moveCandidate(106, "Online Assessment");

    cout << endl
         << "step 5: promoteEligibleCandidates()" << endl;
    pipeline.promoteEligibleCandidates();
    pipeline.printPipeline();

    cout << endl
         << "step 6: withdrawCandidate(110)" << endl;
    pipeline.withdrawCandidate(110);
    cout << "calling it again to show the already withdrawn message:" << endl;
    pipeline.withdrawCandidate(110);

    cout << endl
         << "step 7: removeStage(HR Interview)" << endl;
    pipeline.removeStage("HR Interview");

    cout << endl
         << "step 8: findCandidatesBySkill(C++)" << endl;
    pipeline.findCandidatesBySkill("C++");

    cout << endl
         << "step 9: getBestCandidate()" << endl;
    pipeline.getBestCandidate();

    cout << endl
         << "step 10: reversePipeline()" << endl;
    pipeline.reversePipeline();
    pipeline.printPipeline();

    cout << endl
         << "step 11: displayStatistics()" << endl;
    pipeline.displayStatistics();

    cout << endl
         << "bonus: getMostCrowdedStage()" << endl;
    pipeline.getMostCrowdedStage();

    cout << endl
         << "bonus: hasCycle()" << endl;
    if (pipeline.hasCycle())
        cout << "Cycle detected!" << endl;
    else
        cout << "No cycle detected." << endl;
}

// ---------------- menu driven main ----------------

void setupInitialStages(RecruitmentPipeline &pipeline)
{
    pipeline.addStageAtEnd("Applied");
    pipeline.addStageAtEnd("Screening");
    pipeline.addStageAtEnd("Technical Interview");
    pipeline.addStageAtEnd("HR Interview");
    pipeline.addStageAtEnd("Selected");
}

void showMenu()
{
    cout << endl;
    cout << "1.  Add Stage" << endl;
    cout << "2.  Insert Stage After Another Stage" << endl;
    cout << "3.  Remove Stage" << endl;
    cout << "4.  Add Candidate" << endl;
    cout << "5.  Move Candidate" << endl;
    cout << "6.  Withdraw Candidate" << endl;
    cout << "7.  Update Technical Score" << endl;
    cout << "8.  Update Interview Score" << endl;
    cout << "9.  Promote Eligible Candidates" << endl;
    cout << "10. Get Best Candidate" << endl;
    cout << "11. Find Candidates By Skill" << endl;
    cout << "12. Get Most Crowded Stage" << endl;
    cout << "13. Display Statistics" << endl;
    cout << "14. Check Pipeline Integrity" << endl;
    cout << "15. Print Pipeline" << endl;
    cout << "16. Reverse Pipeline" << endl;
    cout << "17. Check For Cycle" << endl;
    cout << "18. Run Demo Scenario (Section 18)" << endl;
    cout << "19. Exit" << endl;
}

int main()
{
    RecruitmentPipeline pipeline;
    setupInitialStages(pipeline);

    cout << "Recruitment Pipeline started with stages: Applied -> Screening -> Technical Interview -> HR Interview -> Selected" << endl;

    bool running = true;
    while (running)
    {
        showMenu();
        int choice = readIntInRange("Enter your choice: ", 1, 19);

        if (choice == 1)
        {
            string name = readLine("Stage name: ");
            pipeline.addStageAtEnd(name);
            cout << "Stage \"" << name << "\" added at the end." << endl;
        }
        else if (choice == 2)
        {
            string newName = readLine("New stage name: ");
            string afterName = readLine("Insert after which stage: ");
            pipeline.insertStage(newName, afterName);
        }
        else if (choice == 3)
        {
            string name = readLine("Stage name to remove: ");
            pipeline.removeStage(name);
        }
        else if (choice == 4)
        {
            int id = readInt("Candidate ID: ");
            string name = readLine("Name: ");
            string uni = readLine("University: ");
            double cgpa = readDouble("CGPA (0 to 4): ");
            int exp = readInt("Years of experience: ");
            int skillCount = readIntInRange("How many skills: ", 0, 20);
            ArrayList<string> skills(skillCount + 1);
            for (int i = 0; i < skillCount; i++)
            {
                string sk = readLine("Skill: ");
                skills.add(sk);
            }
            string stageName = readLine("Which stage should they start in: ");
            Candidate c(id, name, uni, cgpa, exp, 0, 0, skills);
            pipeline.addCandidate(stageName, c);
        }
        else if (choice == 5)
        {
            int id = readInt("Candidate ID: ");
            string dest = readLine("Destination stage: ");
            pipeline.moveCandidate(id, dest);
        }
        else if (choice == 6)
        {
            int id = readInt("Candidate ID: ");
            pipeline.withdrawCandidate(id);
        }
        else if (choice == 7)
        {
            int id = readInt("Candidate ID: ");
            double score = readDouble("New technical score: ");
            pipeline.updateTechnicalScore(id, score);
        }
        else if (choice == 8)
        {
            int id = readInt("Candidate ID: ");
            double score = readDouble("New interview score: ");
            pipeline.updateInterviewScore(id, score);
        }
        else if (choice == 9)
        {
            pipeline.promoteEligibleCandidates();
        }
        else if (choice == 10)
        {
            pipeline.getBestCandidate();
        }
        else if (choice == 11)
        {
            string skill = readLine("Skill to search for: ");
            pipeline.findCandidatesBySkill(skill);
        }
        else if (choice == 12)
        {
            pipeline.getMostCrowdedStage();
        }
        else if (choice == 13)
        {
            pipeline.displayStatistics();
        }
        else if (choice == 14)
        {
            pipeline.checkPipelineIntegrity();
        }
        else if (choice == 15)
        {
            pipeline.printPipeline();
        }
        else if (choice == 16)
        {
            pipeline.reversePipeline();
        }
        else if (choice == 17)
        {
            if (pipeline.hasCycle())
                cout << "Cycle detected!" << endl;
            else
                cout << "No cycle detected." << endl;
        }
        else if (choice == 18)
        {
            RecruitmentPipeline demoPipeline;
            setupInitialStages(demoPipeline);
            runDemoScenario(demoPipeline);
        }
        else
        {
            running = false;
            cout << "Exiting, goodbye." << endl;
        }
    }

    return 0;
}
