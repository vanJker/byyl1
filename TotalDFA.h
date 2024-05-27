#pragma once
#include "MinDFA.h"

class TotalDFA : public MinDFA
{
  public:
    vector<vector<int>> totalDfa; // ��dfa��
    map<string, int> tkeyVal;     // ��¼�ϲ������������Ӧ��ֵ����Щ�������в�����keyword
    map<int, string> valTkey;
    map<int, set<string>> march2node; // ��¼��node�ܹ�ͨ����Щƥ�����������else,other
    map<int, set<string>> endReturn;  // ��¼��̬���ص����������ĸ�������ʽ

    int tdfaNodeAccount = 0;
    int startNode = 0;
    int endNode = 0;
    int errorNode = 0;

    void setTotalDFA();               // ��������Сdfa�ϳ�һ��
    void initTotalDFA();              // ��ʼ���ܱ�
    string getTotalDFA(int x, int y); // ��ȡ��dfa���Ӧ(x,y)λ�õ�ֵ

    void setTable(string data); // ���еõ�nfa,dfa,mindfa��data:�ı�
    void setTotalTable();

    vector<vector<set<int>>> mergeDFA();
};
