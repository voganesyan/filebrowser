#ifndef _FILECLEANER_H_
#define _FILECLEANER_H_

#include <vector>
#include <string>

class FC_FileInfo
{
public:
 std::string name;
 unsigned long size;
};

/**
 *  Returns the @files with @file_extension contained in the @directory
 */
int FC_GetFilesInDirectory(std::vector<FC_FileInfo> &files, const std::string &directory, const std::string &file_extension);
/**
 * Sets the size of @file to 0
 * Return 0 on success or errno
 */
bool FC_ClearFile(const std::string &file, std::string &error_message);


#endif  // _FILECLEANER_H_
