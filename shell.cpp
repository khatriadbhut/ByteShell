#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include "path.cpp"
#include "prompt.cpp"
#include <vector>
using namespace std;

#define DELIMITER " \n\t\r"
char st[1024][1024];
int idx;

void execute_cd(const vector<string> &tokens)
{
    if (!tokens.empty() && tokens[1] == "~")
    {
        chdir("C:\\Users\\Antriksh");
    }
    else if (!tokens.empty() && tokens[1] == "/")
    {
        chdir("/");
    }
    else if (tokens.empty())
    {
        chdir("C:\\Users\\Antriksh");
    }
    else if (tokens[1] == "..")
    {
        chdir(tokens[1].c_str());
    }
    else
    {
        struct stat buf;
        stat((path() + "/" + tokens[1]).c_str(), &buf);

        if (chdir(tokens[1].c_str()) == 0)
            chdir(tokens[1].c_str());
        else if (chdir(tokens[1].c_str()) == -1)
        {
            if (S_ISREG(buf.st_mode))
            {
                cout << tokens[1] << ": Not a directory" << endl;
            }
            else
            {
                cout << tokens[1] << ": No such file or directory present" << endl;
            }
        }
    }
}

void execute_exit()
{
    exit(1);
}

int helper_echo(const vector<string>& tokens)
{
    int k;
    if (!tokens.empty() && tokens[1] == "-n")
    {
        k = 2;
    }
    else
        k = 1;
    if (!tokens.empty() && tokens[1] == "-E")
    {
        k = 2;
    }
    int count1 = 0;
    int idx1 = 0;
    if (idx != 0)
        k = 0;
    while (k < tokens.size())
    {
        idx1 = 0;
        for (int t = 0; t < tokens[k].length(); t++)
        {
            if (tokens[k][t] != '"')
            {
                st[idx][idx1] = tokens[k][t];
                idx1++;
            }
            else
            {
                count1 += 1;
            }
        }
        st[idx][idx1] = '\0';

        idx++;
        k++;
    }

    return count1;
}

void execute_echo(const vector<string>& tokens)
{
    idx = 0;
    int count = helper_echo(tokens);

    if (count % 2 != 0)
    {
        char inst[1024];

        while (count % 2 != 0)
        {
            print_prompt2();
            cin.getline(inst, 1024);
            char *ptr;

            if ((ptr = strchr(inst, '\n')) != nullptr)
            {
                *ptr = '\0';
            }

            int m = 0;
            vector<string> arr;
            char *c = strtok(inst, DELIMITER);
            while (c != nullptr)
            {
                arr.push_back(c);
                c = strtok(nullptr, DELIMITER);
            }

            st[idx][0] = '\n';
            idx++;
            count += helper_echo(arr);
        }
    }
    for (int y = 0; y < idx; y++)
    {
        if (st[y][0] != '\n')
        {
            cout << st[y] << ' ';
        }
        else
        {
            cout << st[y];
        }
        memset(st[y], '\0', strlen(st[y]));
    }

    if (!tokens.empty() && tokens[1] != "-n")
    {
        cout << endl;
    }
}

void execute_pwd()
{
    char directory[1024];

    if (getcwd(directory, sizeof(directory)) == nullptr)
    {
        perror("Error ");
    }
    else
    {
        cout << directory << endl;
    }
}

vector<string> tokenize_input(const string &input)
{
    vector<string> tokens;
    size_t start = 0;
    size_t end = 0;

    while ((start = input.find_first_not_of(DELIMITER, end)) != string::npos)
    {
        end = input.find_first_of(DELIMITER, start);
        tokens.push_back(input.substr(start, end - start));
    }

    return tokens;
}

void process_input(const string &cmd)
{
    if (cmd.empty())
    {
        return;
    }

    vector<string> tokens = tokenize_input(cmd);

    if (tokens.empty())
    {
        return;
    }

    if (tokens[0] == "cd")
    {
        execute_cd(tokens);
    }
    else if (tokens[0] == "exit")
    {
        execute_exit();
    }
    else if (tokens[0] == "echo")
    {
        execute_echo(tokens);
    }
    else if (tokens[0] == "pwd")
    {
        execute_pwd();
    }
    else
    {
        cout << tokens[0] << ": command not found" << endl;
        exit(1);
    }
}

int main(int argc, char const *argv[])
{
    char cmd[1024];
    cout << "****Welcome to Byteshell****" << endl;
    while (true)
    {
        print_prompt1();
        cin.getline(cmd, 1024);
        process_input(cmd);
    }

    return 0;
}
