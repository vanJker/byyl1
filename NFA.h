#pragma once
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "Regex.h"
using namespace std;

class NFA : public Regex
{
  public:
    int nfaNum; // nfa状态表的点数

    vector<vector<vector<set<int>>>> nfaTable; // nfa状态表
    vector<int> nfaHead;                       // 存储各个表达式的状态表start的下标
    vector<int> nfaNodeAccount;                // 各个表达式对应的NFA的节点数

    NFA();
    void initNFA();         // 初始化
    void setNFA();          // 运行所有正则表达式
    void setNFA(int index); // 将正则表达式转换为NFA状态转换表，index:表示第index条正则表达式
    string getNFAunit(int index, int x,
                      int y); // 返回对应index表达式的状态表对应(x,y)位置的数集
};
