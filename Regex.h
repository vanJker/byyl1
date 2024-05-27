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
{ // ���ڴ洢����������ʽ�е�������
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
    vector<string> regexps;                // �洢��ʼ��������ʽ
    int expsCount;                         // ������ʽ������
    vector<char> regularOperator;          // ������ʽԪ�����
    map<char, int> isp;                    // �����ջ�����ȼ�
    map<char, int> icp;                    // �����ջ�����ȼ�
    map<string, int> wordType;             // ������ʽ=�󲿣������ʵ������Լ��е�һ�γ���ʱ���±�
    map<int, string> indexType;            // ������±��Ӧ���͵ĵ���������
    map<string, vector<int>> languageType; // wordType��������������
    vector<map<string, int>> key_value;    // �Ҳ���������Ӧ���±�
    vector<map<int, string>> value_key;    // �±��Ӧ���Ҳ�������
    vector<vector<RegUnit>> expUnitGroup;  // ���ʽ�飬��������ʽ�ַ����洢�������ݽṹ�У�RegUnitΪ�������ʽ

    // �޲ι��죬��ʼ��Ĭ������������Լ�ջ�������ȼ�
    Regex();
    // ����ȡ�����ı��ַ�����ֳɵ������ַ�����data:�����ı�����
    void initRegularExpresions(string data);
    // �ж��ַ�c�Ƿ�Ϊ���������
    bool isRegualrOperator(char c);
    // ��鲢����ÿһ��������ʽ�ķ�
    void checkRegularExp();
    // ����ʽ��expUnitGroup�е�index����ӷָ���"&"���ָ��ַ��ʹ�
    void changeRegularExp(int index);
    // ʶ����������ʽ��exp:���ʽ���ַ�����index:��index��
    void identifyRegularExp(string exp, int index);
    // ��exp_group�в��뵥Ԫ��flag:��ʾ�Ƿ�Ϊ��
    void insertExpGroup(string name, string type, int index, bool flag);
    // ��������ʽת��Ϊ�沨�����ʽ����׺��
    void change2Post(int index);
};
