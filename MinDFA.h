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

#include "DFA.h"
using namespace std;

class MinDFA : public DFA
{
  public:
    vector<map<set<int>, int>> mstaVal;    // 状态集合的集合-数值
    vector<map<int, set<int>>> valMsta;    // 数值-状态集合的集合
    vector<int> mdfaNodeAccount;           // mindfa表点的数量
    vector<vector<vector<int>>> mdfaTable; // mindfa表
    vector<set<int>> mdfaEndState;         // 记录各个mindfa的终态

    MinDFA();
    void initMinDFA();                         // 初始化mindfa状态表
    void setMinDFA();                          // 设置mindfa表
    void setMinDFA(int index);                 // 设置第index张mindfa表
    string getMinDFA(int index, int x, int y); // 获取mindfa表对应(x,y)位置的值
    void setTAble(string data);                // 运行得到nfa,dfa,mindfa表，data:文本

    vector<vector<int>> assignMinDFA(vector<vector<int>> dfa, set<int> &end_state,
                                     map<int, set<string>> &endR); // 将传入的dfa转为mindfa
};
