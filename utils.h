#pragma once
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <atlconv.h>

void string_replace(std::string& strBig, const std::string& strsrc, const std::string& strdst)
{
    std::string::size_type pos = 0;
    std::string::size_type srclen = strsrc.size();
    std::string::size_type dstlen = strdst.size();

    while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
    {
        strBig.replace(pos, srclen, strdst);
        pos += dstlen;
    }
}

//************************************
// Method:    GetFileOrURLShortName
// FullName:  GetFileOrURLShortName
// Access:    public 
// Returns:   std::string
// Qualifier: 获取路径或URL的文件名（包括后缀，如 C:\Test\abc.xyz --> abc.xyz）
// Parameter: std::string strFullName
//************************************
std::string GetPathOrURLShortName(std::string strFullName)
{
    if (strFullName.empty())
    {
        return "";
    }

    string_replace(strFullName, "/", "\\");

    std::string::size_type iPos = strFullName.find_last_of('\\') + 1;

    return strFullName.substr(iPos, strFullName.length() - iPos);
}

int getFileSize(const char* fileName) {
    int fsize = -1;
    HANDLE fhandle = CreateFile(fileName, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (fhandle != INVALID_HANDLE_VALUE) {
        fsize = GetFileSize(fhandle, NULL);
        CloseHandle(fhandle);
    }
    return fsize;
}

std::vector<std::string> TraverseDirectory(const TCHAR* directory)
{
    std::vector<std::string> res;
    WIN32_FIND_DATA fileInfo;
    TCHAR buffer[MAX_PATH];

    // 构建搜索路径
    _tcscpy_s(buffer, directory);
    _tcscat_s(buffer, _T("\\*.*"));

    // 查找第一个文件/目录
    HANDLE hFind = FindFirstFile(buffer, &fileInfo);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // 排除 "." 和 ".."
            if (_tcscmp(fileInfo.cFileName, _T(".")) != 0 && _tcscmp(fileInfo.cFileName, _T("..")) != 0)
            {
                // 输出文件名
                char temp[50];
                sprintf(temp, "%s\\%s", directory, fileInfo.cFileName);
                res.push_back(temp);

                // 如果是目录，则递归遍历子目录
                if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    _tcscpy_s(buffer, directory);
                    _tcscat_s(buffer, _T("\\"));
                    _tcscat_s(buffer, fileInfo.cFileName);
                    TraverseDirectory(buffer);
                }
            }
        } while (FindNextFile(hFind, &fileInfo));

        FindClose(hFind);
    }
    return res;
}
