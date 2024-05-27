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
    vector<int> dfa_node_account;          // �������ʽ��dfa״̬��Ľڵ�ĸ���
    vector<vector<vector<int>>> dfa_table; // ��¼�������ʽ��״̬��
    vector<map<set<int>, int>> sta_val;    // ״̬���϶�Ӧ��ֵ
    vector<map<int, set<int>>> val_sta;    // ֵ��Ӧ��״̬����
    vector<set<int>> dfa_end_state;        // �洢dfa����̬

    DFA();
    void initDFA();
    void setDFA(); // ���ɸ������ʽ��dfa״̬��
    set<int> search_node(int index, int x,
                         int y); // ������index�ű�ӵ�ǰ��x����������y���ܵ�������е�
    void setDFA(int index);      // ���ɵ�index�����ʽ��״̬��
    string getDFA(int index, int x,
                  int y);                // ��ȡ��index��dfa���Ӧ(x,y)λ�õ�ֵ
    string getSet(int index, int value); // ��ȡindex���Ӧ����value��״̬����

    vector<vector<int>> assignDFA(vector<vector<set<int>>> nfa, int head, int end, set<int> &end_state,
                                  map<int, set<string>> &endR); // �������nfaתΪdfa
    set<int> search_node(vector<vector<set<int>>> nfa, int x, int y);
};
