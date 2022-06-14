#include "miniGit.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int split(string split, char separator, string array[], int size)
{
    if (split == "")
    {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < split.length(); i++)
    {

        if (split[i] == separator)
        {
            count++;
            if (count == size)
            {
                return -1;
            }
        }
        else
        {
            array[count] += split[i];
        }
    }

    return count + 1;
}

void copyFile(string repoName, string filename, string fileVersion, int commitNum)
{
    ifstream originalFile;
    //Dont think I need this

    ofstream newfile;
    string line;
    originalFile.open(filename);
    //I understand what you are doing here, but I don/t think there should be multiple directories
    //also I think the code I have in newCommit can take care of the version number
    string newfilename = repoName + "/" + filename + "_" + to_string(commitNum);
    //string newfilename = repoName + "/" + filename;

    newfile.open(newfilename);
    if (originalFile.is_open() && newfile.is_open())
    {
        while (getline(originalFile, line))
        {
            newfile << line;
            newfile << endl;
            line = "";
        }
    }
    originalFile.close();
    newfile.close();
}

commitList::commitList()
{
    head->commitNumber = 0;
    head->previous = NULL;
    head->fileHead = NULL;
    head->next = NULL;
    totalCommits = 0;
}
commitList::~commitList()
{
    Commit *ptr = head;
    Commit *ptrC = ptr;

    File *filePtr;
    File *tFilePtr;

    Commit *tPtr;

    while (ptr != NULL)
    {
        filePtr = ptr->fileHead;

        while (filePtr != NULL)
        {
            tFilePtr = filePtr;
            filePtr = filePtr->next;
            delete tFilePtr;
        }

        tPtr = ptr;
        ptr = ptr->next;

        if (tPtr != head)
        {
            delete tPtr;
        }
        else
        {
            head = NULL;
        }
    }
}
bool commitList::fileInCommit(std::string fileName)
{
    Commit *ptr = head;
    Commit *ptrC = ptr;

    while (ptr != NULL)
    {
        ptrC = ptr;
        ptr = ptr->next;
    }

    File *filePtr = ptrC->fileHead;

    while (filePtr != NULL)
    {
        if (filePtr->fileName == fileName)
        {
            return true;
        }
        filePtr = filePtr->next;
    }

    return false;
}
void commitList::addHead(Commit *node)
{
    head = node;
}
void commitList::printCurrentCommit()
{
    Commit *ptr = head;

    while (ptr->next != NULL)
    {
        ptr = ptr->next;
    }

    File *filePtr = ptr->fileHead;

    if (filePtr == NULL)
    {
        std::cout << "No files in current commit" << std::endl;
        return;
    }

    std::cout << std::endl;
    std::cout << "Files in current commit:" << std::endl;
    while (filePtr != NULL)
    {
        std::cout << filePtr->fileName << std::endl;
        filePtr = filePtr->next;
    }
}
void commitList::newFile(std::string fileName)
{
    File *nFile = new File();

    nFile->fileName = fileName;
    nFile->fileVersion = totalCommits;
    nFile->next = NULL;

    Commit *ptr = head;
    Commit *ptrC = ptr;

    while (ptr != NULL)
    {
        ptrC = ptr;
        ptr = ptr->next;
    }

    File *filePtr = ptrC->fileHead;
    File *filePtrC = filePtr;
    File *filePtrC2 = filePtrC;

    if (filePtr == NULL)
    {
        ptrC->fileHead = nFile;
        return;
    }

    while (filePtr != NULL)
    {
        filePtrC2 = filePtrC;
        filePtrC = filePtr;
        filePtr = filePtr->next;
    }

    filePtrC->next = nFile;
}
void commitList::removeFile(std::string filename)
{
    //get to current commit DLL node
    Commit *ptr = head;
    Commit *ptrC = ptr;

    while (ptr != NULL)
    {
        ptrC = ptr;
        ptr = ptr->next;
    }
    File *filePtr = ptrC->fileHead;
    File *slowPtr = NULL;
    while (filePtr != NULL)
    {
        if (filename == filePtr->fileName)
        {
            //if its head we just need to change heads
            if (slowPtr == NULL)
            {
                ptrC->fileHead = filePtr->next;
                delete filePtr;
            }
            //if not head then remove it normally
            else
            {
                slowPtr->next = filePtr->next;
                delete filePtr;
            }
        }
        slowPtr = filePtr;
        filePtr = filePtr->next;
    }
}
void commitList::newCommit(string repoName)
{
    totalCommits++;
    //I dont think we need to have a new directory for each commit
    //I think all committed files should be in the original repository and
    //the linked lists let us know what commit they are from
    //fs::create_directory(repoName + "/" + to_string(totalCommits));

    string array[2];

    ifstream lF;
    ifstream cF;

    string checkLine;
    string lastLine;

    Commit *ptr = head;
    Commit *ptrC = ptr;

    while (ptr != NULL)
    {
        ptrC = ptr;
        ptr = ptr->next;
    }

    File *filePtr = ptrC->fileHead;

    while (filePtr != NULL)
    {
        copyFile(repoName, filePtr->fileName, to_string(totalCommits), totalCommits);

        filePtr = filePtr->next;
    }

    if (totalCommits > 1)
    {
        ptr = head;
        ptrC = ptr;

        while (ptr != NULL)
        {
            filePtr = ptrC->fileHead;

            while (filePtr != NULL)
            {
                string filename = repoName + "/" + filePtr->fileName + "_" + to_string(totalCommits);

                if (!fs::exists(filename))
                {
                    copyFile(repoName, filePtr->fileName, filePtr->fileVersion, totalCommits);
                }

                filePtr = filePtr->next;
            }

            ptrC = ptr;
            ptr = ptr->next;
        }
    }

    Commit *newCommit = new Commit();

    newCommit->commitNumber = totalCommits;
    newCommit->fileHead = NULL;
    newCommit->previous = ptrC;
    newCommit->next = NULL;

    ptrC->next = newCommit;
}

void checkoutHelper(string repoName, string fileVersion, string filename, int commitNum)
{
    ifstream originalFile;
    ofstream newfile;
    string line;
    if (fs::exists(filename))
    {
        fs::remove(filename);
    }
    originalFile.open(repoName + "/" + filename + "_" + to_string(commitNum));
    newfile.open(filename);
    if (originalFile.is_open() && newfile.is_open())
    {
        while (getline(originalFile, line))
        {
            newfile << line;
            newfile << endl;
            line = "";
        }
    }
}
//Clears directory and reverts to old save
bool commitList::Checkout(string reponame, int commitNum)
{
    Commit *crawler = head;
    Commit *slowCrawler = NULL;
    while (crawler != NULL)
    {
        if (crawler->commitNumber == commitNum)
        {
            File *fileCrawler = slowCrawler->fileHead;
            while (fileCrawler != NULL)
            {
                checkoutHelper(reponame, fileCrawler->fileVersion, fileCrawler->fileName, commitNum);
                fileCrawler = fileCrawler->next;
            }

            return true;
        }
        slowCrawler = crawler;
        crawler = crawler->next;
    }
    return false;
}

/*string lastFile = repoName + "/" + filePtr->fileName + "_" + to_string(totalCommits - 1);

            split(lastFile, '/', array, 2);

            string checkFile = filePtr->fileName;

            cout << lastFile << endl;
            cout << checkFile << endl;
            cout << endl;

            lF.open(lastFile);
            cF.open(checkFile);

            if (lF.is_open() && cF.is_open())
            {
                while (getline(cF, checkLine))
                {
                    getline(lF, lastLine);
                    if (checkLine != lastLine)
                    {
                        copyFile(repoName, checkFile, array[1], totalCommits);
                        break;
                    }
                }
            }
            else
            {
                cout << "Failed to open one or more file(s)" << endl;
            }

            lF.close();
            cF.close();*/
