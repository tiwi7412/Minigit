#include <string>

struct File
{
    std::string fileName;
    std::string fileVersion;
    File *next;
};

struct Commit
{
    int commitNumber;
    File *fileHead = NULL;
    Commit *previous;
    Commit *next;
};

//Responsible for the entire commit
class commitList
{
private:
    Commit *head;
    int totalCommits;

public:
    commitList();
    ~commitList();
    void addHead(Commit* head);
    void printCurrentCommit();
    void newCommit(std::string repoName);
    bool fileInCommit(std::string filename);
    void newFile(std::string fileName);
    void removeFile(std::string fileName);
    bool Checkout(std::string reponame, int commitNum);
};
