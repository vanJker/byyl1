#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct RegUnit
{ // 用于存储单条正则表达式中的运算量
    string name;
    string type;
    RegUnit(string n, string t)
    {
        name = n;
        type = t;
    };
    RegUnit(const RegUnit &other)
    {
        name = other.name;
        type = other.type;
    }
    void setUnit(string n, string t)
    {
        name = n;
        type = t;
    }
    void clear()
    {
        name = "";
        type = "";
    }
};

class Regex
{
  public:
    vector<string> regexps;                // 存储初始的正则表达式
    int expsCount;                         // 正则表达式的数量
    vector<char> regularOperator;          // 正则表达式元运算符
    map<char, int> isp;                    // 运算符栈内优先级
    map<char, int> icp;                    // 运算符栈外优先级
    map<string, int> wordType;             // 正则表达式=左部，即单词的类型以及行第一次出现时的下标
    map<int, string> indexType;            // 相对行下标对应类型的单词类型名
    map<string, vector<int>> languageType; // wordType所属的语言类型
    vector<map<string, int>> key_value;    // 右侧运算量对应的下标
    vector<map<int, string>> value_key;    // 下标对应的右侧运算量
    vector<vector<RegUnit>> expUnitGroup;  // 表达式组，将正则表达式字符串存储到该数据结构中，RegUnit为单条表达式

    // 无参构造，初始化默认正则运算符以及栈内外优先级
    Regex();
    // 将读取到的文本字符串拆分成单条的字符串，data:输入文本内容
    void initRegularExpresions(string data);
    // 判断字符c是否为正则运算符
    bool isRegualrOperator(char c);
    // 检查并处理每一条正则表达式文法
    void checkRegularExp();
    // 向表达式组expUnitGroup中第index条添加分隔符"&"，分割字符和串
    void changeRegularExp(int index);
    // 识别单条正则表达式，exp:表达式的字符串，index:第index条
    void identifyRegularExp(string exp, int index);
    // 向exp_group中插入单元，flag:表示是否为左部
    void insertExpGroup(string name, string type, int index, bool flag);
    // 将正则表达式转化为逆波兰表达式（后缀）
    void change2Post(int index);
};
