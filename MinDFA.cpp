#include "MinDFA.h"

MinDFA::MinDFA()
{
}

// ��ʼ��mindfa״̬��
void MinDFA::initMinDFA()
{
    mdfaNodeAccount.resize(expsCount);
    mdfaTable.resize(expsCount);
    mstaVal.resize(expsCount);
    valMsta.resize(expsCount);
}

// ����mindfa״̬��
void MinDFA::setMinDFA()
{
    initMinDFA();
    for (int i = 0; i < expsCount; i++)
    {
        setMinDFA(i);
    }
}

// ���õ�index��mindfa״̬��
void MinDFA::setMinDFA(int index)
{
    map<vector<int>, set<int>> tMap;
    set<int> tend_state; // �洢��̬
    mdfaNodeAccount[index] = 0;

    // dfaתmindfa�㷨
    set<int> nonendset; // ����̬����
    set<int> endset;    // ��̬����
    // ������index��dfa��ڵ㣬�����Ƿ���dfa����̬�����������̬���Ϻͷ���̬����
    for (int i = 1; i <= dfa_node_account[index]; i++)
    {
        if (dfa_end_state[index].count(i) > 0)
            endset.insert(i);
        else
            nonendset.insert(i);
    }

    vector<set<int>> setStack; // ��̬�����̬��������
    setStack.push_back(nonendset);
    setStack.push_back(endset);
    // int newsize = setStack.size();
    // int oldsize = 0;
    vector<set<int>>
        temp2Stack; // ����temp2Stack�����ڼ�¼��һ����̬�����̬������������������ڱȽϷֱ�������С�Ƿ�仯
    while (temp2Stack.size() != setStack.size())
    {                          // �洢����̬���Ϻ���̬���ϵ�����setStack�Ĵ�С�����˱仯
        temp2Stack = setStack; // ������setStack�仯ǰ��ֵ�������´αȽ�
        for (unsigned int i = 1; i < key_value[index].size(); i++)
        {                               // �����±�Ϊindex�Ĺؼ��ֵ�ÿһ���ַ�
            vector<set<int>> tempStack; // ����tempStack������ѭ����������̬���������̬��������
            for (unsigned int j = 0; j < setStack.size(); j++)
            {                                 // ��������setStack��ÿһ������
                set<int> oldset;              // ��֮ǰ��ͬ�ļ���
                map<int, set<int>> mp_newset; // �������¼��ϣ�����һ��ѭ��������ֹһ��set��������mapӳ��
                set<int> otherset;            // �޷�ͨ���ļ���
                for (set<int>::iterator it = setStack[j].begin(); it != setStack[j].end(); it++)
                { // ������������ǰҪ�����ļ���
                    // �ӵ�ǰ״̬�������������±�Ϊi���ַ������Խ��뵱ǰ����j�е�ĳ��״̬,��Ϊ�ϵļ��ϲ���
                    if (dfa_table[index][*it][i] > 0 && setStack[j].count(dfa_table[index][*it][i]) > 0)
                    {
                        oldset.insert(*it);
                    }
                    // �ӵ�ǰ״̬�������������±�Ϊi���ַ����޷����뵱ǰ����j�е��κ�״̬,�����µļ��ϲ����뵽��Ӧ��״̬map��
                    else if (dfa_table[index][*it][i] > 0 && setStack[j].count(dfa_table[index][*it][i]) == 0)
                    {
                        mp_newset[dfa_table[index][*it][i]].insert(*it);
                    }
                    // �ӵ�ǰ״̬�������޷������±�Ϊi���ַ�����Ϊ�������ϲ���
                    else if (dfa_table[index][*it][i] == 0)
                    {
                        otherset.insert(*it);
                    }
                }
                // ��������ѭ��������������������������������tempStack��
                if (oldset.size() > 0)
                    tempStack.push_back(oldset);
                if (mp_newset.size() > 0)
                    for (map<int, set<int>>::iterator it = mp_newset.begin(); it != mp_newset.end(); it++)
                        tempStack.push_back(it->second);
                if (otherset.size() > 0)
                    tempStack.push_back(otherset);
            }
            setStack = tempStack; // ��ʱ��������setStack
        }
    }

    mdfaNodeAccount[index] = setStack.size(); // �ڵ���==������������Ϊͬһ��������Ľڵ���Ժϲ�
    int pos = 1;
    for (unsigned int i = 0; i < setStack.size(); i++)
    {
        if (setStack[i].count(1) == 1)
        {
            mstaVal[index][setStack[i]] = 1;
            valMsta[index][1] = setStack[i];
        }
        else
        {
            pos++;
            mstaVal[index][setStack[i]] = pos;
            valMsta[index][pos] = setStack[i];
            for (auto it : dfa_end_state[index])
            {
                if (setStack[i].count(it) > 0)
                {
                    tend_state.insert(pos);
                    break;
                }
            }
        }
    }
    // ��ʼ��minDFA��
    int ysize = key_value[index].size();
    int xsize = mdfaNodeAccount[index] + 1;
    vector<vector<int>> tmdfa(xsize);
    for (int i = 0; i < xsize; i++)
    {
        tmdfa[i].resize(ysize);
    }

    for (int i = 1; i <= dfa_node_account[index]; i++)
    {
        for (unsigned int j = 1; j < key_value[index].size(); j++)
        {
            if (dfa_table[index][i][j] > 0)
            {
                int x = 0, val = 0;
                for (unsigned int k = 0; k < valMsta[index].size(); k++)
                {
                    if (valMsta[index][k + 1].count(i) > 0)
                        x = k + 1;
                    if (valMsta[index][k + 1].count(dfa_table[index][i][j]) > 0)
                        val = k + 1;
                }
                tmdfa[x][j] = val;
            }
        }
    }

    mdfaTable[index] = tmdfa;
    mdfaEndState.push_back(tend_state);
}

// ��ȡ��index��mindfa���Ӧ(x,y)λ�õ�ֵ
string MinDFA::getMinDFA(int index, int x, int y)
{
    if (mdfaTable[index][x][y] > 0)
        return to_string(mdfaTable[index][x][y]);
    return "";
}

vector<vector<int>> MinDFA::assignMinDFA(vector<vector<int>> dfa, set<int> &end_state, map<int, set<string>> &endR)
{
    map<vector<int>, set<int>> tMap;
    set<int> tend_state; // �洢��̬
    map<set<int>, int> tmstaVal;
    map<int, set<int>> val_tmsta;
    map<int, set<string>> tendR;
    int mdfa_cnt = 0;

    int xsize = dfa.size();
    int ysize = dfa[0].size();

    // ���㷨
    set<int> nonendset; // ��¼����̬��
    set<int> endset;    // ��¼��̬��
    for (int i = 1; i < xsize; i++)
    {
        if (end_state.count(i) > 0)
            endset.insert(i);
        else
            nonendset.insert(i);
    }

    vector<set<int>> setStack;
    setStack.push_back(nonendset);
    setStack.push_back(endset);
    // int newsize = setStack.size();
    // int oldsize = 0;
    vector<set<int>> temp2Stack;
    while (temp2Stack.size() != setStack.size())
    {
        temp2Stack = setStack;

        for (int i = 1; i < ysize; i++)
        {
            vector<set<int>> tempStack;
            for (unsigned int j = 0; j < setStack.size(); j++)
            {
                set<int> oldset;
                set<int> newset;
                set<int> otherset; // ��¼����ͨ���ĵ�ļ���
                for (set<int>::iterator it = setStack[j].begin(); it != setStack[j].end(); it++)
                {
                    if (dfa[*it][i] > 0 && setStack[j].count(dfa[*it][i]) > 0)
                    { // �����ܷ��ص�ǰ����
                        oldset.insert(*it);
                    }
                    else if (dfa[*it][i] > 0 && setStack[j].count(dfa[*it][i]) == 0)
                    {
                        newset.insert(*it);
                    }
                    else if (dfa[*it][i] == 0)
                    {
                        otherset.insert(*it);
                    }
                }
                if (oldset.size() > 0)
                    tempStack.push_back(oldset);
                if (newset.size() > 0)
                    tempStack.push_back(newset);
                if (otherset.size() > 0)
                    tempStack.push_back(otherset);
            }
            setStack = tempStack;
        }
    }

    mdfa_cnt = setStack.size();
    int pos = 1;
    for (unsigned int i = 0; i < setStack.size(); i++)
    {
        if (setStack[i].count(1) == 1)
        {
            tmstaVal[setStack[i]] = 1;
            val_tmsta[1] = setStack[i];
        }
        else
        {
            pos++;
            tmstaVal[setStack[i]] = pos;
            val_tmsta[pos] = setStack[i];
            for (auto it : end_state)
            {
                if (setStack[i].count(it) > 0)
                {
                    tend_state.insert(pos);
                    break;
                }
            }
        }
    }
    for (unsigned int i = 0; i < setStack.size(); i++)
    { // ��¼����̬�㷵�ص�����
        set<int> tset = setStack[i];
        for (set<int>::iterator it = tset.begin(); it != tset.end(); it++)
        {
            if (endR.count(*it) > 0)
            {
                tendR[tmstaVal[tset]].insert(endR[*it].begin(), endR[*it].end());
            }
        }
    }
    // ��ʼ��minDFA��
    int new_xsize = mdfa_cnt + 1; // ��һ��ʼ
    vector<vector<int>> tmdfa(new_xsize);
    for (int i = 0; i < new_xsize; i++)
    {
        tmdfa[i].resize(ysize);
    }

    for (int i = 1; i < xsize; i++)
    {
        for (int j = 1; j < ysize; j++)
        {
            if (dfa[i][j] > 0)
            {
                int x = 0, val = 0;
                for (int k = 0; k < ysize; k++)
                {
                    if (val_tmsta[k + 1].count(i) > 0)
                        x = k + 1;
                    if (val_tmsta[k + 1].count(dfa[i][j]) > 0)
                        val = k + 1;
                }
                tmdfa[x][j] = val;
            }
        }
    }

    int endpos = mdfa_cnt + 1;
    tmdfa.erase(tmdfa.begin() + endpos, tmdfa.end());
    end_state = tend_state;
    endR = tendR;
    return tmdfa;
}
