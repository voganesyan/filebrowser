#include "filecleaner.h"
#include <iostream>
#include <stdio.h>
#ifdef WINDOWS
#include <windows.h>
#include <tchar.h>
#else
#include <unistd.h> //truncate
#include <dirent.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <errno.h>
#include <string.h>   // for strerror()
#endif

using namespace std;

/* Returns a list of files in a directory (except the ones that begin with a dot) */

int FC_GetFilesInDirectory(vector<FC_FileInfo> &files, const string &directory, const string &file_extension)
{
#ifdef WINDOWS
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return -1; /* No files found */

    do {
        const string file_name = file_data.cFileName;
        const string full_file_name = directory + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.')
            continue;

        if (is_directory)
            continue;

        FC_FileInfo info;
        info.name = file_name;
        info.size = file_data.nFileSizeHigh;

        files.push_back(info);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
#else

    class dirent *ent;
    class stat st;

    DIR *dir = opendir(directory.c_str());

    if (!dir) 
    {
      perror("perror: ");
    return -1;
    }
    
    while ((ent = readdir(dir)) != NULL) {
        const string file_name = ent->d_name;
        const string full_file_name = directory + "/" + file_name;

        if (file_name[0] == '.')
            continue;

        if (lstat(full_file_name.c_str(), &st) == -1)
            continue;

        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        if (is_directory)
            continue;
        
        if(!file_extension.empty() &&
            file_extension != file_name.substr(file_name.find_first_of(".") + 1))
            continue;
            
        FC_FileInfo info;
        info.name = file_name;
        info.size = st.st_size;
        
        files.push_back(info);
    }
    closedir(dir);
#endif
} // GetFilesInDirectory
 
bool FC_ClearFile(const string &file, string &error_message)
{
#ifdef WINDOWS
  HANDLE hf = CreateFile (file.c_str(), GENERIC_READ |GENERIC_WRITE,
                    0,
                    NULL, TRUNCATE_EXISTING,
                    FILE_ATTRIBUTE_NORMAL, NULL);
   if (hf == INVALID_HANDLE_VALUE)
   {
      // Handle error.
      cout << "error: " << GetLastError() << "could not open file: " << file << endl;
      return false;
   }
   CloseHandle(hf);

   //SetEndOfFile(hf);   // Force client redirector to set file size to zero.
#else
 if (truncate(file.c_str(), 0) != 0) 
 {
  //perror("perror");
  cout << "DDDDDD" << strerror(errno) << endl;
  error_message = strerror(errno);
  return false;
 }
#endif
return true;
}

