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
    string desfile_name; // 目标文件名

    FileProcessor();
    void setFile_Name(string str);    // 获取文件名并判断该文件是否存在
    string readFile_Data();           // 读取文件内容
    void writeFile_Data(string data); // 将data写入c文件
    string getDesfile_name();         // 返回目标文件路径

    string getExefile_name(); // 返回编码文件名
};
