#include "NFA.h"

NFA::NFA()
{
    nfaNum = 0;
}

void NFA::initNFA()
{
    nfaTable.resize(expsCount);
}

void NFA::setNFA()
{
    initNFA();
    for (int i = 0; i < expsCount; i++)
        setNFA(i);
}

void NFA::setNFA(int index)
{ // 从后缀表达式转换为nfa状态表
    int ysize = key_value[index].size();
    int xsize = 2 * expUnitGroup[index].size() + 2;
    vector<vector<set<int>>> tnfa(xsize);
    for (int i = 0; i < xsize; i++)
    {
        tnfa[i].resize(ysize);
    }

    stack<pair<int, int>> calculator;
    int cnt = 0, pt1pre = 0, pt1end = 0, pt2pre = 0, pt2end = 0;
    //    bool flag = 0;
    string wType = expUnitGroup[index][0].name;
    for (unsigned int i = 1; i < expUnitGroup[index].size(); i++)
    {
        if (expUnitGroup[index][i].type == wType)
        {
            int x, y;
            pt1pre = ++cnt;
            x = pt1pre;
            pt1end = ++cnt;
            y = key_value[index][expUnitGroup[index][i].name];
            tnfa[x][y].insert(pt1end);
            calculator.push(make_pair(pt1pre, pt1end));
        }
        else
        {
            if (expUnitGroup[index][i].name == "&" || expUnitGroup[index][i].name == "|")
            {
                pt1pre = calculator.top().first;
                pt1end = calculator.top().second;
                calculator.pop();
                pt2pre = calculator.top().first;
                pt2end = calculator.top().second;
                calculator.pop();
            }
            else
            {
                pt1pre = calculator.top().first;
                pt1end = calculator.top().second;
                calculator.pop();
            }

            // 默认情况下tnfa[x][0]为'#'下的数集
            if (expUnitGroup[index][i].name == "&")
            {
                tnfa[pt2end][0].insert(pt1pre);
                pt1pre = pt2pre;
            }
            else if (expUnitGroup[index][i].name == "|")
            {
                int ptpre = ++cnt, ptend = ++cnt;
                tnfa[ptpre][0].insert(pt1pre);
                tnfa[ptpre][0].insert(pt2pre);
                tnfa[pt1end][0].insert(ptend);
                tnfa[pt2end][0].insert(ptend);

                pt1pre = ptpre;
                pt1end = ptend;
            }
            else if (expUnitGroup[index][i].name == "*")
            {
                int ptpre = ++cnt, ptend = ++cnt;
                tnfa[ptpre][0].insert(pt1pre);
                tnfa[pt1end][0].insert(ptend);
                tnfa[pt1end][0].insert(pt1pre);
                tnfa[ptpre][0].insert(ptend);

                pt1pre = ptpre;
                pt1end = ptend;
            }
            else if (expUnitGroup[index][i].name == "+")
            {
                int ptpre = ++cnt, ptend = ++cnt;
                tnfa[ptpre][0].insert(pt1pre);
                tnfa[pt1end][0].insert(ptend);
                tnfa[pt1end][0].insert(pt1pre);

                pt1pre = ptpre;
                pt1end = ptend;
            }
            else if (expUnitGroup[index][i].name == "?")
            {
                int ptpre = ++cnt, ptend = ++cnt;
                tnfa[ptpre][0].insert(pt1pre);
                tnfa[pt1end][0].insert(ptend);
                tnfa[ptpre][0].insert(ptend);

                pt1pre = ptpre;
                pt1end = ptend;
            }
            calculator.push(make_pair(pt1pre, pt1end));
        }
    }
    nfaTable[index] = tnfa;
    nfaHead.push_back(pt1pre);
    nfaNodeAccount.push_back(cnt);
}

string NFA::getNFAunit(int index, int x, int y)
{
    string str = "";
    set<int>::iterator it;
    for (it = nfaTable[index][x][y].begin(); it != nfaTable[index][x][y].end(); it++)
    {
        if (it != nfaTable[index][x][y].begin())
            str += ",";
        str += to_string(*it);
    }
    return str;
}
