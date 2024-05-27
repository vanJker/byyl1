#include "MinDFA.h"

MinDFA::MinDFA()
{
}

// 初始化mindfa状态表
void MinDFA::initMinDFA()
{
    mdfaNodeAccount.resize(expsCount);
    mdfaTable.resize(expsCount);
    mstaVal.resize(expsCount);
    valMsta.resize(expsCount);
}

// 设置mindfa状态表
void MinDFA::setMinDFA()
{
    initMinDFA();
    for (int i = 0; i < expsCount; i++)
    {
        setMinDFA(i);
    }
}

// 设置第index张mindfa状态表
void MinDFA::setMinDFA(int index)
{
    map<vector<int>, set<int>> tMap;
    set<int> tend_state; // 存储终态
    mdfaNodeAccount[index] = 0;

    // dfa转mindfa算法
    set<int> nonendset; // 非终态集合
    set<int> endset;    // 终态集合
    // 遍历第index张dfa表节点，根据是否在dfa的终态集合里，划分终态集合和非终态集合
    for (int i = 1; i <= dfa_node_account[index]; i++)
    {
        if (dfa_end_state[index].count(i) > 0)
            endset.insert(i);
        else
            nonendset.insert(i);
    }

    vector<set<int>> setStack; // 终态与非终态集合向量
    setStack.push_back(nonendset);
    setStack.push_back(endset);
    // int newsize = setStack.size();
    // int oldsize = 0;
    vector<set<int>>
        temp2Stack; // 向量temp2Stack：用于记录上一次终态与非终态集合向量的情况，用于比较分辨向量大小是否变化
    while (temp2Stack.size() != setStack.size())
    {                          // 存储非终态集合和终态集合的向量setStack的大小发生了变化
        temp2Stack = setStack; // 在向量setStack变化前赋值，便于下次比较
        for (unsigned int i = 1; i < key_value[index].size(); i++)
        {                               // 遍历下标为index的关键字的每一个字符
            vector<set<int>> tempStack; // 向量tempStack：本次循环产生的终态集合与非终态集合向量
            for (unsigned int j = 0; j < setStack.size(); j++)
            {                                 // 遍历向量setStack的每一个集合
                set<int> oldset;              // 和之前相同的集合
                map<int, set<int>> mp_newset; // 产生的新集合，可能一轮循环产生不止一个set，所以用map映射
                set<int> otherset;            // 无法通过的集合
                for (set<int>::iterator it = setStack[j].begin(); it != setStack[j].end(); it++)
                { // 迭代器迭代当前要遍历的集合
                    // 从当前状态出发，处理完下标为i的字符，可以进入当前集合j中的某个状态,作为老的集合插入
                    if (dfa_table[index][*it][i] > 0 && setStack[j].count(dfa_table[index][*it][i]) > 0)
                    {
                        oldset.insert(*it);
                    }
                    // 从当前状态出发，处理完下标为i的字符，无法进入当前集合j中的任何状态,产生新的集合并插入到对应的状态map里
                    else if (dfa_table[index][*it][i] > 0 && setStack[j].count(dfa_table[index][*it][i]) == 0)
                    {
                        mp_newset[dfa_table[index][*it][i]].insert(*it);
                    }
                    // 从当前状态出发，无法处理下标为i的字符，作为其他集合插入
                    else if (dfa_table[index][*it][i] == 0)
                    {
                        otherset.insert(*it);
                    }
                }
                // 集合上面循环产生的三个集合如果不空则插入向量tempStack中
                if (oldset.size() > 0)
                    tempStack.push_back(oldset);
                if (mp_newset.size() > 0)
                    for (map<int, set<int>>::iterator it = mp_newset.begin(); it != mp_newset.end(); it++)
                        tempStack.push_back(it->second);
                if (otherset.size() > 0)
                    tempStack.push_back(otherset);
            }
            setStack = tempStack; // 临时向量赋给setStack
        }
    }

    mdfaNodeAccount[index] = setStack.size(); // 节点数==集合数量，因为同一个集合里的节点可以合并
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
    // 初始化minDFA表
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

// 获取第index个mindfa表对应(x,y)位置的值
string MinDFA::getMinDFA(int index, int x, int y)
{
    if (mdfaTable[index][x][y] > 0)
        return to_string(mdfaTable[index][x][y]);
    return "";
}

vector<vector<int>> MinDFA::assignMinDFA(vector<vector<int>> dfa, set<int> &end_state, map<int, set<string>> &endR)
{
    map<vector<int>, set<int>> tMap;
    set<int> tend_state; // 存储终态
    map<set<int>, int> tmstaVal;
    map<int, set<int>> val_tmsta;
    map<int, set<string>> tendR;
    int mdfa_cnt = 0;

    int xsize = dfa.size();
    int ysize = dfa[0].size();

    // 新算法
    set<int> nonendset; // 记录非终态点
    set<int> endset;    // 记录终态点
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
                set<int> otherset; // 记录不能通过的点的集合
                for (set<int>::iterator it = setStack[j].begin(); it != setStack[j].end(); it++)
                {
                    if (dfa[*it][i] > 0 && setStack[j].count(dfa[*it][i]) > 0)
                    { // 如若能返回当前集合
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
    { // 记录接受态点返回的内容
        set<int> tset = setStack[i];
        for (set<int>::iterator it = tset.begin(); it != tset.end(); it++)
        {
            if (endR.count(*it) > 0)
            {
                tendR[tmstaVal[tset]].insert(endR[*it].begin(), endR[*it].end());
            }
        }
    }
    // 初始化minDFA表
    int new_xsize = mdfa_cnt + 1; // 从一开始
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
