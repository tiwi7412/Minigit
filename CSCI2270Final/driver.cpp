#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>
#include <fstream>
#include "miniGit.hpp"

namespace fs = std::filesystem;

using namespace std;

//FINISHED
//Adds file and location to singly linked list
void addFileToCommit(commitList &commits, string repoName)
{
    string filename;

retry:
    cout << endl;
    cout << "Please input a filename (Type \"cancel\" to quit) " << endl;
    getline(cin, filename);

    if (filename == "cancel")
    {
        return;
    }

    if (fs::exists(filename))
    {
        if (commits.fileInCommit(filename))
        {
            cout << endl;
            cout << "File is already in the commit!" << endl;
            cout << "Please enter a new file" << endl;
            goto retry;
        }

        commits.newFile(filename);
        cout << endl;
        cout << "File " << filename << " added to commit!" << endl;
        return;
    }
    else
    {
        cout << endl;
        cout << "File does not exist in this directory!" << endl;
        cout << "Please enter a valid file name" << endl;
        goto retry;
    }
}

//Removes file/location from SLL
void removeFileFromCommit(commitList &commits)
{
    string filename;

retry:
    cout << endl;
    cout << "Please input a filename (Type \"cancel\" to quit) " << endl;
    getline(cin, filename);

    if (filename == "cancel")
    {
        return;
    }

    if (fs::exists(filename))
    {

        commits.removeFile(filename);
        cout << endl;
        cout << "File " << filename << " removed from commit!" << endl;
        return;
    }
    else
    {
        cout << endl;
        cout << "File does not exist in this directory!" << endl;
        cout << "Please enter a valid file name" << endl;
        goto retry;
    }
}

//Creates DLL node and copies all files SLL into correct directory
void Commit()
{
}

void checkout(commitList &commits, string reponame)
{
    

    int commitNum;
    string commitNumStr;
    retry:
    cout << endl;
    cout << "Please input a commit number (Type \"cancel\" to quit) " << endl;

    getline(cin, commitNumStr);
    if (commitNumStr == "cancel")
    {
        return;
    }
    for (int i = 0; i < commitNumStr.length(); i++)
    {
        if (!isdigit(commitNumStr[i]))
        {
            cout << "Value entered not a number" << endl;
            goto retry;
        }
    }
        commitNum = stoi(commitNumStr);
    if (commits.Checkout(reponame, commitNum))
    {
        return;
    }
    else
    {
        cout << endl;
        cout << "Commit does not exist!" << endl;
        cout << "Please enter a valid commit num" << endl;
        goto retry;
    }
}

int main()
{
    char option;
    string strOption;

    string repositoryName;

    commitList commits;

newRepository:
    cout << "Would you like to create a new repository? (Y/N)" << endl;
    getline(cin, strOption);

    option = strOption[0];

    if (option == 'Y' || option == 'y')
    {
    repoName:
        cout << endl;
        cout << "What would you like to name this repository?" << endl;
        getline(cin, repositoryName);

        if (fs::exists(repositoryName))
        {
        repoExists:
            cout << endl;
            cout << "A repository with this name already exists. Would you like to overwrite it? (Y/N)" << endl;
            getline(cin, strOption);

            option = strOption[0];

            if (option == 'Y' || option == 'y')
            {
                fs::remove_all(repositoryName);
            }
            else if (option == 'N' || option == 'n')
            {
                goto repoName;
            }
            else
            {
                cout << "Invalid Input" << endl;
                goto repoExists;
            }
        }
        fs::create_directory(repositoryName);

        cout << endl;
        cout << "Repository " << repositoryName << " successfully created!" << endl;
    }
    else if (option == 'N' || option == 'n')
    {
        cout << "No new repository created" << endl;
        return 0;
    }
    else
    {
        cout << "Please enter Y/N" << endl;
        goto newRepository;
    }

    while (option != '5')
    {
        cout << endl;
        commits.printCurrentCommit();
        cout << endl;
        cout << "                 Menu                 " << endl;
        cout << "______________________________________" << endl;
        cout << "1. Add file to the current commit" << endl;
        cout << "2. Remove file from the current commit" << endl;
        cout << "3. Commit changes" << endl;
        cout << "4. Revert to a previous commit" << endl;
        cout << "5. Quit" << endl;

        getline(cin, strOption);
        option = strOption[0];

        switch (option)
        {
        case '1':
            addFileToCommit(commits, repositoryName);
            break;
        case '2':
            removeFileFromCommit(commits);
            break;
        case '3':
            commits.newCommit(repositoryName);
            break;
        case '4':
            checkout(commits,repositoryName);
            break;
        case '5':
            break;
        default:
            cout << "Please choose one of the options listed." << endl;
        }
    }
}
