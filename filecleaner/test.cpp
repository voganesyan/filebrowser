#include "filecleaner.h"
#include <iostream>
using namespace std;
int main (int argc, char *argv[])
{
  switch (argc)
  { 
      //test ClearFile
      case 2:
      {
       string error_message;
       if (!FC_ClearFile(argv[1], error_message))
          cout << "Error: " << error_message << endl;
      } break;
      
      //test GetFilesInDirectory with Extensions
      case 3:
      {
       vector<FC_FileInfo> files;
       string dir = argv[1];
       string ext = argv[2];
       FC_GetFilesInDirectory(files, dir, ext);
      
       for (vector<FC_FileInfo>::iterator it = files.begin() ; it != files.end(); ++it)
       {
        FC_FileInfo info = *it;
        cout << "FILE: name: " << info.name << " size: " << info.size << endl; 
       }
      } break;
      
      default: cout << "usage: " <<  argv[0] <<  " <path/to/directory>" << endl;
 }
 return 0;
}
