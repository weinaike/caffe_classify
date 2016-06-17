#include "cdirfile.h"

CDirfile::CDirfile(const char* dir)
{
    m_dir=dir;

}

bool fileCmp(string file1, string file2)
{
    return file1<file2;
}

string CDirfile::getContent(string filetype,int fileOrDir)
{
    m_filetype=filetype;
    m_fileOrDir=fileOrDir;
    static vector<string> vs;
    static vector<string> vdir;
    static unsigned index = 0;
    static unsigned idir = 0;

    if(index==0)
    {
        vs = getall(m_filetype,m_fileOrDir);
        fileCount = vs.size();
        sort(vs.begin(),vs.end(),fileCmp);
    }

    if(idir==0)
    {
        vdir = getall(m_filetype,m_fileOrDir);
        dirCount = vdir.size();
        sort(vdir.begin(),vdir.end(),fileCmp);
    }

    if(m_fileOrDir==FILE)
    {
        if(index<vs.size())
        {
            return vs.at(index++);
        }
        else
        {
            return "";
        }
    }
    else
    {
        if(idir<vdir.size())
            return vdir.at(idir++);
        else
            return "";
    }
}

vector<string> CDirfile::getall(string filetype,int fileOrDir)
{
    m_filetype=filetype;
    m_fileOrDir=fileOrDir;
    vector<string> dirlists,filelists;
    dirlists.clear();
    filelists.clear();

    string strCurDir = "";
    string strSubDir = "";
    string strFile = "";

    strCurDir = m_dir;
    if(strCurDir.length()==0)
    {
        return filelists;
    }

    if(strCurDir.at(strCurDir.length()-1)!='/')
    {
        strCurDir += "/";
    }

    dirlists.push_back(strCurDir);
    dirCount++;
    DIR* dp;
    struct dirent* direntp;

    int k = m_filetype.size();
    int n = 0;
    string strTemp = "";

    while(!dirlists.empty())
    {
        strCurDir = dirlists.back();
        dirlists.pop_back();
        dp = NULL;

        if((dp=opendir(strCurDir.c_str()))==NULL)
        {
            cout << "ERROR:Open " << strCurDir << " failed!" << endl;
            //该目录无法打开，查找下一个目录
            continue;
        }
        while((direntp=readdir(dp))!=NULL)
        {
            strFile.clear();
            strFile = strCurDir;
            strFile += direntp->d_name;

            //每个文件下都有“.”代表当前目录， “..”代表上一层目录，都要过滤掉
            if(strcmp(direntp->d_name,".")==0||strcmp(direntp->d_name,"..")==0)
            {
                continue;
            }

            if(m_fileOrDir==FILE)
            {
                if(direntp->d_type==DT_DIR)
                {
                    strSubDir.clear();
                    strSubDir = strCurDir+direntp->d_name;
                    strSubDir += "/";
                    dirlists.push_back(strSubDir);
                    continue;
                }
                if(m_filetype.size()!=0)
                {
                    n = strFile.size();
                    if(strFile.at(n-k-1)!='.') continue;
                    strTemp.clear();
                    strTemp = strFile.substr(n-k,k);
                    if(!(m_filetype==strTemp)) continue;
                }
                filelists.push_back(strFile);
            }
            else
            {
                if(direntp->d_type!=DT_DIR)
                {
                    continue;
                }
                else
                {
                    strSubDir.clear();
                    strSubDir = strCurDir+direntp->d_name;
                    strSubDir += "/";
                    filelists.push_back(strSubDir);
                    dirlists.push_back(strSubDir);
                }
            }
        }
        closedir(dp);
    }
    return filelists;
}

string CDirfile::toUpper(string& str)
{
    int n = str.size();
    char c = 'a';
    for(int i=0; i<n; i++)
    {
        c = str.at(i);
        if(c>='a'&&c<='z')
        {
            str.at(i) = c + 'A' - 'a';
        }
    }
    return str;
}
