#include <iostream>
#include <cstdlib>
#include <unistd.h>
using namespace std;

string path()
{
    char directory[1024];
    char *dir;
    // getcwd -> get the current working directory 
    if (getcwd(directory, sizeof(directory)) == NULL)
    {
        perror("Error ");
        return "";
    }
    else
    {
        dir = getcwd(directory, sizeof(directory));
        return string(dir);
    }
}