#pragma once

#include <string>
#include <vector>

//interface definition
class i_filesystem_op
{
    virtual long scanDir(const char* dirName, std::vector<std::string>& vecFileList);
};

class CPosixFsOp :public i_filesystem_op
{
    long scanDir(const char* dirName, std::vector<std::string>& vecFileList)
    {
        DIR* pstDirHandle = ::opendir(dirName);
        if( nullptr == pstDirHandle )
        {
            return -1;
        }

        struct dirent* pstEntry = nullptr;
        while( nullptr != (pstEntry = ::readdir(pstDirHandle)))
        {
            if( 0 == strcmp(".", pstEntry->d_name) || 0 == strcmp("..", pstEntry->d_name) )
            {
                continue;
            }

            std::string strFullPath = std::string(pcDirName) + "/" + std::string(pstEntry->d_name);
            switch (pstEntry->d_type)
            {
                case 8:
                case 10:
                {
                    vecFileList.push_back(strFullPath);
                }
                case 4:
                {
                    scanDir(strFullPath.c_str(), vecFileList);
                }
                default:
                {
                    break;
                }
            }

        }
        closedir(pstDirHandle);
    }
};

class CWinFsOp : public i_filesystem_op
{

};

class CFileSystem
{
    static CFileSystem* create();
    long scanDir();

private:
    i_filesystem_op m_Impl;
}