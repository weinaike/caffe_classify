#ifndef CDIRFILE_H
#define CDIRFILE_H

#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string.h>
#include <algorithm>
#include <stdlib.h>
#define MAX_COUNT 100 //如果查找的文件很多，超过100个，每按一次显示100个
#define FILE 0 //查找的是文件
#define DIRECTORY 1 //查找的是目录
using namespace std;


class CDirfile
{
public:
    CDirfile(const char* dir);
    //返回目录dir下的，filetype类型文件或目录，每次返回一条，没有返回“”，
    string getContent(string filetype="",int fileOrDir=FILE);
    //得到目录dir下的filetype类型文件或dir下的目录的列表
    vector<string> getall(string filetype="", int fileOrDir=FILE);
    //对得到得到的返回列表进行排序的函数，实则为比较两个字符串大小
    //把一个string字符串中的小写字符转换成大写
    string toUpper(string& str);
private:
    const char *m_dir;
    string m_filetype;
    int m_fileOrDir;
    //目录数
    int dirCount = 0;
    //文件数
    int fileCount = 0;
};

#endif // CDIRFILE_H
