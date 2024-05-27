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
    int nfaNum; // nfa״̬��ĵ���

    vector<vector<vector<set<int>>>> nfaTable; // nfa״̬��
    vector<int> nfaHead;                       // �洢�������ʽ��״̬��start���±�
    vector<int> nfaNodeAccount;                // �������ʽ��Ӧ��NFA�Ľڵ���

    NFA();
    void initNFA();         // ��ʼ��
    void setNFA();          // ��������������ʽ
    void setNFA(int index); // ��������ʽת��ΪNFA״̬ת����index:��ʾ��index��������ʽ
    string getNFAunit(int index, int x,
                      int y); // ���ض�Ӧindex���ʽ��״̬���Ӧ(x,y)λ�õ�����
};
