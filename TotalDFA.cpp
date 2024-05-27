#include "TotalDFA.h"

void TotalDFA::initTotalDFA()
{
    startNode = 1;
    for (int i = 0; i < expsCount; i++)
    {
        if (count(languageType["keyword"].begin(), languageType["keyword"].end(), i) == 1)
            continue;                          // keyword型不直借连在表上
        int cur_size = mdfaNodeAccount[i] - 1; // 去除每一个表的开始点
        tdfaNodeAccount += cur_size;
    }
    tdfaNodeAccount += 3; // 添加一个start点、end点和error点
    endNode = tdfaNodeAccount;
    errorNode = endNode - 1;

    int cnt = 0;
    tkeyVal["#"] = 0;
    valTkey[0] = "#";
    for (int i = 0; i < expsCount; i++)
    { // 将各个表达式的运算量添加到总的dfa表中
        if (count(languageType["keyword"].begin(), languageType["keyword"].end(), i) == 1)
            continue; // keyword型不直借连在表上
        for (auto it : value_key[i])
        {
            if (tkeyVal.count(it.second) == 0 && it.second != "#")
            {
                cnt++;
                tkeyVal[it.second] = cnt;
                valTkey[cnt] = it.second;
            }
        }
    }
    tkeyVal["else"] = ++cnt; // 添加else运算量
    valTkey[cnt] = "else";
    tkeyVal["other"] = ++cnt; // 添加other运算量
    valTkey[cnt] = "other";

    int xSize = tdfaNodeAccount + 1; // 初始化，同时再加一防止直接访问越界
    int ySize = cnt + 1;
    totalDfa.resize(xSize);
    for (int i = 0; i < xSize; i++)
    {
        totalDfa[i].resize(ySize);
    }
}

void TotalDFA::setTotalDFA()
{
    vector<vector<set<int>>> merge_dfa = mergeDFA();
    set<int> end_state;
    int nfa_cnt = merge_dfa.size();
    int endpos = nfa_cnt - 1;
    vector<vector<int>> dfa = assignDFA(merge_dfa, 1, endpos, end_state, endReturn);
    vector<vector<int>> mindfa = assignMinDFA(dfa, end_state, endReturn);
    vector<int> errorline;
    errorline.resize(tkeyVal.size());
    mindfa.push_back(errorline);

    totalDfa = mindfa;
    tdfaNodeAccount = mindfa.size() - 1; // 去除[0]
    endNode = *end_state.begin();
    errorNode = tdfaNodeAccount;
    totalDfa[startNode][tkeyVal["other"]] = errorNode;
    totalDfa[errorNode][tkeyVal["other"]] = errorNode;
    totalDfa[errorNode][tkeyVal["else"]] = startNode;

    for (int i = 1; i <= tdfaNodeAccount; i++)
    {
        for (int j = 1; j < valTkey.size() - 2; j++)
        {
            if (totalDfa[i][j] > 0)
            {
                march2node[i].insert(valTkey[j]); // 点i能通过j这个匹配符
            }
        }
    }
}

string TotalDFA::getTotalDFA(int x, int y)
{
    if (totalDfa[x][y] > 0)
        return to_string(totalDfa[x][y]);
    return "";
}

void TotalDFA::setTable(string data)
{
    initRegularExpresions(data);
    checkRegularExp();

    setNFA();
    setDFA();
    setMinDFA();
}

void TotalDFA::setTotalTable()
{
    // combindOperator();
    initTotalDFA();
    setTotalDFA();
}

vector<vector<set<int>>> TotalDFA::mergeDFA()
{
    int head = startNode, pos = 1, nodecnt = 1;
    vector<vector<set<int>>> tVec;

    for (int i = 0; i < expsCount; i++)
    {
        if (count(languageType["keyword"].begin(), languageType["keyword"].end(), i) == 1)
            continue; // keyword型不直借连在表上
        nodecnt += mdfaNodeAccount[i];
    }
    nodecnt += 1; // error点和end点
    // int error = nodecnt - 1;
    int end = nodecnt;

    tVec.resize(nodecnt + 1);
    int ysize = tkeyVal.size();
    for (int i = 0; i <= nodecnt; i++)
    {
        tVec[i].resize(ysize);
    }

    for (int i = 0; i < expsCount; i++)
    {
        if (count(languageType["keyword"].begin(), languageType["keyword"].end(), i) == 1)
            continue; // keyword型不直借连在表上
        int sp = tkeyVal["#"];
        tVec[head][sp].insert(pos + 1); // 点head通过#到每个mindfa的起始点
        for (int j = 1; j <= mdfaNodeAccount[i]; j++)
        {
            for (int k = 1; k < key_value[i].size(); k++)
            {
                if (mdfaTable[i][j][k] > 0)
                {
                    int x = pos + j;
                    int y = tkeyVal[value_key[i][k]];
                    int val = pos + mdfaTable[i][j][k];
                    tVec[x][y].insert(val);
                }
            }
        }
        if (count(languageType["statement"].begin(), languageType["statement"].end(), i) == 1)
        { // 如果是注释则返回起点
            for (auto j : mdfaEndState[i])
            {
                /*
                int x = j + pos - 1;
                int preend = value_key[i].size()-1;
                //该值为注释最后点的前一个位置所要通过的条件 string op =
                value_key[i][preend]; int y = tkeyVal[op];
                */
                int x = j + pos;
                int y = tkeyVal["#"];
                tVec[x][y].insert(head);
            }
        }
        else
        {
            for (auto j : mdfaEndState[i])
            {
                int x = j + pos;
                endReturn[x].insert(indexType[i]); // 记录终态返回的内容
                int y = tkeyVal["else"];
                tVec[x][y].insert(end);
            }
        }
        pos += mdfaNodeAccount[i];
    }

    return tVec;
}
