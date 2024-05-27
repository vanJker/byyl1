#pragma once
#include <direct.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "TotalDFA.h"
using namespace std;

class GenerationSYS : public TotalDFA
{
  public:
    string lexcode = "";       // 生成的词法分析代码
    string srcfilename = "";   // 源文件文件名
    string headfile = "";      // 头文件代码
    string funstatement = "";  // 函数声明代码
    set<string> funstate_set;  // 函数声明集合避免反复声明
    string identFun_code = ""; // id
    string keyword_code = "";  // 关键字
    string identify_code = ""; // 除关键字、注释外的其他
    GenerationSYS();

    vector<int> word_code;
    vector<map<string, int>> fun_name; // 存储函数名
    vector<vector<int>> sidemap;       // 转换的得到的总路径图
    map<pair<int, int>, int> side_id;  // 存储对应边的编号

    void setProgram(); // 生成词法分析程序
    string getCode();  // 返回词法分析程序字符创

    // 生成程序函数
    void addInclude();             // 无参初始化默认头文件
    void addInclude(string hname); // 添加头文件

    void addFunStatement();                 // 无参初始化默认函数声明
    void addFunStatement(string statement); // 添加函数名statement

    string addGlobalvar();     // 添加全局变量
    string addFun_TokenType(); // enum TokenType函数
    string addFun_isChar();    // 添加是否为字符函数（用于给关键字进入）

    // 语言中的关键字，运算符，标识符，数值，注释，空白等
    string addFun_isWhiteSpace(); // 添加是否为空白格函数
    string addFun_isOperator();   // 添加是否为运算符函数
    string addFun_isKeyword();    // 添加是否为关键字函数

    string unpackCon(string con,
                     string x);    // 将条件con转换成x==con[i]或x!=con[i]的形式，con的形式为[.*],x为参数名
    string addFun_combinStr();     // 添加连接两个char*型字符串函数
    string addFun_getSourceFile(); // 添加获取源程序文件函数
    string addFun_readFile();      // 添加读取源程序内容函数
    string addFun_encoding();      // 添加获取编码程序函数
    void addFun_marchFun(string data,
                         int state); // 添加识别data的函数,针对dfa中的匹配符,state为匹配符的值函数
    void addFun_identifyKeyword();   // 添加识别关键字id的函数
    void addFun_identfiy();          // 添加识别标识符id的函数
    string addFun_showWord();        // 添加输出编码函数
    string addFun_main();            // 添加main函数

    string addFun_writedata(); // 输出单词编码函数

    void compile(string filename); // 编译生成的可执行文件名
    void setSrc_filename(string source_filename)
    {
        srcfilename = source_filename;
    }
};
