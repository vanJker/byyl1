#pragma once
#include "MinDFA.h"

class TotalDFA : public MinDFA
{
  public:
    vector<vector<int>> totalDfa; // 总dfa表
    map<string, int> tkeyVal;     // 记录合并后的运算量对应的值，这些运算量中不包括keyword
    map<int, string> valTkey;
    map<int, set<string>> march2node; // 记录到node能够通过那些匹配符：不包括else,other
    map<int, set<string>> endReturn;  // 记录终态返回的内容属于哪个正则表达式

    int tdfaNodeAccount = 0;
    int startNode = 0;
    int endNode = 0;
    int errorNode = 0;

    void setTotalDFA();               // 将各个最小dfa合成一个
    void initTotalDFA();              // 初始化总表
    string getTotalDFA(int x, int y); // 获取总dfa表对应(x,y)位置的值

    void setTable(string data); // 运行得到nfa,dfa,mindfa表，data:文本
    void setTotalTable();

    vector<vector<set<int>>> mergeDFA();
};
