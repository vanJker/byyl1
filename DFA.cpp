#include "DFA.h"

DFA::DFA()
{
}

void DFA::initDFA()
{
    dfa_node_account.resize(expsCount);
    dfa_table.resize(expsCount);
    sta_val.resize(expsCount);
    val_sta.resize(expsCount);
}

// 生成每个NFA对应的DFA状态表
void DFA::setDFA()
{
    initDFA();
    for (int i = 0; i < expsCount; i++)
    {
        setDFA(i);
    }
}

// 搜索从当前NFA状态x经过某个输入y所能到达的所有状态
set<int> DFA::search_node(int index, int x, int y)
{
    set<int> sta = nfaTable[index][x][y];
    set<int>::iterator it;
    for (it = sta.begin(); it != sta.end(); it++)
    {
        sta.insert(*it);
        set<int> res = search_node(index, *it, 0);
        sta.insert(res.begin(), res.end());
    }
    return sta;
}

// 由某个特定的NFA转换表生成对应的DFA状态表
void DFA::setDFA(int index)
{
    int ysize = key_value[index].size();
    int xsize = nfaNodeAccount[index] + 1;
    vector<vector<int>> tdfa(xsize);
    set<int> tend_state;
    for (int i = 0; i < xsize; i++)
    {
        tdfa[i].resize(ysize);
    }

    int start = nfaHead[index];
    int dfacount = 0;
    set<int> origin = search_node(index, start, 0);
    origin.insert(nfaHead[index]);
    sta_val[index][origin] = ++dfacount;
    val_sta[index][dfacount] = origin;

    queue<set<int>> list;
    list.push(origin);
    while (!list.empty())
    {
        set<int> sta = list.front();
        list.pop();

        int row = sta_val[index][sta];
        set<int>::iterator it;
        for (unsigned int i = 1; i < key_value[index].size(); i++)
        {
            set<int> tsta;
            for (it = sta.begin(); it != sta.end(); it++)
            {
                set<int> follow = search_node(index, *it, i);
                tsta.insert(follow.begin(), follow.end());
            }
            if (sta_val[index].count(tsta) == 0 && tsta.size() != 0)
            {
                sta_val[index][tsta] = ++dfacount;
                val_sta[index][dfacount] = tsta;
                if (tsta.count(nfaNodeAccount[index]) > 0)
                {
                    tend_state.insert(dfacount);
                }
                list.push(tsta);
            }
            if (tsta.size() != 0)
            {
                tdfa[row][i] = sta_val[index][tsta];
            }
        }
    }
    dfa_table[index] = tdfa;
    dfa_node_account[index] = dfacount;
    dfa_end_state.push_back(tend_state);
}

// 获取特定位置的DFA状态表值
string DFA::getDFA(int index, int x, int y)
{
    set<int> sta = val_sta[index][dfa_table[index][x][y]];
    string str = "";
    set<int>::iterator it;
    for (it = sta.begin(); it != sta.end(); it++)
    {
        if (it != sta.begin())
            str += ",";
        str += to_string(*it);
    }
    return str;
}

// 获取特定编码的状态集合
string DFA::getSet(int index, int value)
{
    set<int> sta = val_sta[index][value];
    string str = "";
    set<int>::iterator it;
    for (it = sta.begin(); it != sta.end(); it++)
    {
        if (it != sta.begin())
            str += ",";
        str += to_string(*it);
    }
    return str;
}

// 搜索从当前NFA状态x经过某个输入y所能到达的所有状态
set<int> DFA::search_node(vector<vector<set<int>>> nfa, int x, int y)
{
    set<int> sta = nfa[x][y];
    set<int>::iterator it;
    for (it = sta.begin(); it != sta.end(); it++)
    {
        sta.insert(*it);
        set<int> res = search_node(nfa, *it, 0);
        sta.insert(res.begin(), res.end());
    }
    return sta;
}

// 将NFA转换为DFA
vector<vector<int>> DFA::assignDFA(vector<vector<set<int>>> nfa, int head, int end, set<int> &end_state,
                                   map<int, set<string>> &endR)
{
    int start = head;
    int dfacount = 0;
    vector<vector<int>> tdfa;
    map<int, set<string>> tendR;
    int xsize = nfa.size();
    int ysize = nfa[0].size();
    tdfa.resize(xsize);
    for (int i = 0; i < xsize; i++)
        tdfa[i].resize(ysize);

    set<int> origin = search_node(nfa, start, 0);
    origin.insert(start);

    map<set<int>, int> tsta_val;
    map<int, set<int>> val_tsta;

    dfacount++;
    tsta_val[origin] = dfacount;
    val_tsta[dfacount] = origin;

    queue<set<int>> list;
    list.push(origin);
    while (!list.empty())
    {
        set<int> sta = list.front();
        list.pop();

        int row = tsta_val[sta];
        set<int>::iterator it;
        for (int i = 1; i < ysize; i++)
        {
            set<int> tsta;
            for (it = sta.begin(); it != sta.end(); it++)
            {
                set<int> follow = search_node(nfa, *it, i);
                tsta.insert(follow.begin(), follow.end());
            }
            if (tsta_val.count(tsta) == 0 && tsta.size() != 0)
            {
                tsta_val[tsta] = ++dfacount;
                val_tsta[dfacount] = tsta;
                if (tsta.count(end) > 0)
                {
                    end_state.insert(dfacount);
                }
                for (set<int>::iterator j = tsta.begin(); j != tsta.end(); j++)
                {
                    if (endR.count(*j))
                    {
                        tendR[dfacount].insert(endR[*j].begin(), endR[*j].end());
                    }
                }
                list.push(tsta);
            }
            if (tsta.size() != 0)
            {
                tdfa[row][i] = tsta_val[tsta];
            }
        }
    }
    int endpos = dfacount + 1;
    tdfa.erase(tdfa.begin() + endpos, tdfa.end());
    endR = tendR;
    return tdfa;
}
