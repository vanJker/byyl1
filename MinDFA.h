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
    vector<map<set<int>, int>> mstaVal;    // ״̬���ϵļ���-��ֵ
    vector<map<int, set<int>>> valMsta;    // ��ֵ-״̬���ϵļ���
    vector<int> mdfaNodeAccount;           // mindfa��������
    vector<vector<vector<int>>> mdfaTable; // mindfa��
    vector<set<int>> mdfaEndState;         // ��¼����mindfa����̬

    MinDFA();
    void initMinDFA();                         // ��ʼ��mindfa״̬��
    void setMinDFA();                          // ����mindfa��
    void setMinDFA(int index);                 // ���õ�index��mindfa��
    string getMinDFA(int index, int x, int y); // ��ȡmindfa���Ӧ(x,y)λ�õ�ֵ
    void setTAble(string data);                // ���еõ�nfa,dfa,mindfa��data:�ı�

    vector<vector<int>> assignMinDFA(vector<vector<int>> dfa, set<int> &end_state,
                                     map<int, set<string>> &endR); // �������dfaתΪmindfa
};
