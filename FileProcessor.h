#pragma once
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
class FileProcessor
{
  public:
    string file_name;
    string file_data;
    string desfile_name; // Ŀ���ļ���

    FileProcessor();
    void setFile_Name(string str);    // ��ȡ�ļ������жϸ��ļ��Ƿ����
    string readFile_Data();           // ��ȡ�ļ�����
    void writeFile_Data(string data); // ��dataд��c�ļ�
    string getDesfile_name();         // ����Ŀ���ļ�·��

    string getExefile_name(); // ���ر����ļ���
};
