#pragma once
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "NFA.h"

class DFA : public NFA
{
  public:
    vector<int> dfa_node_account;          // 各个表达式中dfa状态表的节点的个数
    vector<vector<vector<int>>> dfa_table; // 记录各个表达式的状态表
    vector<map<set<int>, int>> sta_val;    // 状态集合对应的值
    vector<map<int, set<int>>> val_sta;    // 值对应的状态集合
    vector<set<int>> dfa_end_state;        // 存储dfa表终态

    DFA();
    void initDFA();
    void setDFA(); // 生成各个表达式的dfa状态表
    set<int> search_node(int index, int x,
                         int y); // 搜索第index张表从当前点x触发，经过y所能到达的所有点
    void setDFA(int index);      // 生成第index条表达式的状态表
    string getDFA(int index, int x,
                  int y);                // 获取第index个dfa表对应(x,y)位置的值
    string getSet(int index, int value); // 获取index表对应编码value的状态集合

    vector<vector<int>> assignDFA(vector<vector<set<int>>> nfa, int head, int end, set<int> &end_state,
                                  map<int, set<string>> &endR); // 将传入的nfa转为dfa
    set<int> search_node(vector<vector<set<int>>> nfa, int x, int y);
};
