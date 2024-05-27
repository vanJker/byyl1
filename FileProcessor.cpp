#include "FileProcessor.h"

FileProcessor::FileProcessor()
{
    file_name = "";
    file_data = "";
    desfile_name = "";
}

void FileProcessor::setFile_Name(string str)
{
    ifstream fin;
    fin.open(str, ios::in);
    if (!fin.is_open())
    {
        cerr << "无法打开文件!" << endl;
        return;
    }
    fin.close();
    file_name = str;
    int pos = 0;
    for (int i = str.length() - 1; i > 0; i--)
    {
        if (str[i] == '.')
        {
            pos = i;
            break;
        }
    }
    str.erase(str.begin() + pos, str.end());
    desfile_name = str + ".c";
}

string FileProcessor::readFile_Data()
{
    ifstream fin;
    fin.open(file_name, ios::in);
    if (!fin.is_open())
    {
        cout << "无法打开文件!" << endl;
        exit(1);
    }
    string line;
    while (getline(fin, line))
    {
        if (line.length() != 0)
            file_data += line + "\n";
    }
    fin.close();
    return file_data;
}

void FileProcessor::writeFile_Data(string data)
{
    ofstream fout;
    fout.open(desfile_name, ios::out);
    if (!fout.is_open())
    {
        cout << "无法打开文件!" << endl;
        return;
    }
    fout << data << endl;
    fout.close();
    return;
}

string FileProcessor::getDesfile_name()
{
    return desfile_name;
}

string FileProcessor::getExefile_name()
{
    string exename = desfile_name;
    int i = 0;
    for (i = exename.length() - 1; i > 0; i--)
    {
        if (desfile_name[i] == '.')
            break;
    }
    exename.erase(exename.begin() + i, exename.end());
    return exename;
}
