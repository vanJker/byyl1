#include "TotalDFA.h"

void TotalDFA::initTotalDFA()
{
    startNode = 1;
    for (int i = 0; i < expsCount; i++)
    {
        if (count(languageType["keyword"].begin(), languageType["keyword"].end(), i) == 1)
            continue;                          // keyword�Ͳ�ֱ�����ڱ���
        int cur_size = mdfaNodeAccount[i] - 1; // ȥ��ÿһ����Ŀ�ʼ��
        tdfaNodeAccount += cur_size;
    }
    tdfaNodeAccount += 3; // ���һ��start�㡢end���error��
    endNode = tdfaNodeAccount;
    errorNode = endNode - 1;

    int cnt = 0;
    tkeyVal["#"] = 0;
    valTkey[0] = "#";
    for (int i = 0; i < expsCount; i++)
    { // ���������ʽ����������ӵ��ܵ�dfa����
        if (count(languageType["keyword"].begin(), languageType["keyword"].end(), i) == 1)
            continue; // keyword�Ͳ�ֱ�����ڱ���
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
    tkeyVal["else"] = ++cnt; // ���else������
    valTkey[cnt] = "else";
    tkeyVal["other"] = ++cnt; // ���other������
    valTkey[cnt] = "other";

    int xSize = tdfaNodeAccount + 1; // ��ʼ����ͬʱ�ټ�һ��ֱֹ�ӷ���Խ��
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
    tdfaNodeAccount = mindfa.size() - 1; // ȥ��[0]
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
                march2node[i].insert(valTkey[j]); // ��i��ͨ��j���ƥ���
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
            continue; // keyword�Ͳ�ֱ�����ڱ���
        nodecnt += mdfaNodeAccount[i];
    }
    nodecnt += 1; // error���end��
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
            continue; // keyword�Ͳ�ֱ�����ڱ���
        int sp = tkeyVal["#"];
        tVec[head][sp].insert(pos + 1); // ��headͨ��#��ÿ��mindfa����ʼ��
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
        { // �����ע���򷵻����
            for (auto j : mdfaEndState[i])
            {
                /*
                int x = j + pos - 1;
                int preend = value_key[i].size()-1;
                //��ֵΪע�������ǰһ��λ����Ҫͨ�������� string op =
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
                endReturn[x].insert(indexType[i]); // ��¼��̬���ص�����
                int y = tkeyVal["else"];
                tVec[x][y].insert(end);
            }
        }
        pos += mdfaNodeAccount[i];
    }

    return tVec;
}
