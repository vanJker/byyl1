#include "Regex.h"

// �޲ι��죬��ʼ��Ĭ������������Լ���ջ�������ȼ���������ʽ����
Regex::Regex()
{
    regularOperator = {'(', ')', '[', ']', '+', '*', '?', '&', '|'}; // ���������
    isp = {{'#', 0}, {'(', 1}, {'[', 1}, {'*', 7}, {'+', 7},
           {'?', 7}, {'&', 5}, {'|', 3}, {')', 8}, {']', 8}}; // �����ջ�����ȼ���#Ϊ�գ����ȼ����
    icp = {{'#', 0}, {'(', 8}, {'[', 8}, {'*', 6}, {'+', 6},
           {'?', 6}, {'&', 4}, {'|', 2}, {')', 1}, {']', 1}}; // �����ջ�����ȼ���#Ϊ�գ����ȼ����
    expsCount = 0;                                            // ������ʽ����
}

// ����ȡ�����ı��ַ�����ֳɵ������ַ�����data:�����ı�����
void Regex::initRegularExpresions(string data)
{
    int cnt = 0; // ������ʽ����
    string temp[1024];
    for (unsigned int i = 0; i < data.length(); i++)
    { // ���������ݰ��д���temp
        if (data[i] != '\n')
        {
            temp[cnt] += data[i];
        }
        else
        {
            cnt++;
        }
    }
    string current_type = "";
    for (int i = 0; i <= cnt; i++)
    { // ����temp���г�Ա�ĳ�ʼ��
        if (temp[i].length() < 1)
            continue; // ��
//        if (temp[i].length() > 2 && temp[i][0] == '/' && temp[i][1] == '/')
//        { // �����µ��ķ�����
//            string strtype = "";
//            for (unsigned int j = 2; j < temp[i].length(); j++)
//            {
//                // j=2��ʼ˵��ҪԽ��"//"��ȡ���ķ�������
//                if (temp[i][j] != '\n')
//                    strtype += temp[i][j];
//            }
//            // Ϊ�µ��ķ�������languageType�����һ��vector<int>���ڴ��뵱ǰ�����ķ�������������������ʽ����������±�
//            current_type = strtype;
//            vector<int> tempV;
//            languageType[current_type] = tempV;
//        }
        if (temp[i].length() > 1 && temp[i][0] == '#')
        { // �����µ��ķ�����
            string strtype = "";
            for (unsigned int j = 1; j < temp[i].length(); j++)
            {
                // j=1��ʼ˵��ҪԽ��"#"��ȡ���ķ�������
                if (temp[i][j] != '\n')
                    strtype += temp[i][j];
            }
            // Ϊ�µ��ķ�������languageType�����һ��vector<int>���ڴ��뵱ǰ�����ķ�������������������ʽ����������±�
            current_type = strtype;
            vector<int> tempV;
            languageType[current_type] = tempV;
        }
        else
        { // ����������ʽ�Ķ���
            regexps.push_back(temp[i]);
            expsCount++;
            int index = i - languageType.size(); // ���뵥��������ʽ��������±꣨�����������ķ����������У�
            languageType[current_type].push_back(index);
            // ��ʼ��������ʽ�����ӳ�䣬Ĭ��"#"Ϊ�����������ȼ����
            map<string, int> kv;
            kv["#"] = 0;
            map<int, string> vk;
            vk[0] = "#";
            key_value.push_back(kv);
            value_key.push_back(vk);
        }
    }
    // ��ʼ��expUnitGroup��Ա������expsCount��С
    expUnitGroup.resize(expsCount);
}

// ��鲢����ÿһ��������ʽ�ķ�
void Regex::checkRegularExp()
{
    for (int i = 0; i < expsCount; i++)
    {
        identifyRegularExp(regexps[i],
                           i); // ʶ��������ʽ�е���������exp:���ʽ��index:��ʾ��index��
        changeRegularExp(i);   // ����expUnitGroup������ӷ���"&"�����Ӷ��б��ʽʹ���������ʶ��
        change2Post(i);        // ת��Ϊ�沨�����ʽ����׺��
    }
}

// ��exp_group�в��뵥Ԫ��flag:��ʾ�Ƿ�Ϊ��
void Regex::insertExpGroup(string name, string type, int index, bool flag)
{
    // δ�������͵��󲿣�����������±�����Ƶ�ӳ��
    if (flag && wordType.count(name) == 0)
    {
        wordType[name] = index;
        indexType[index] = name;
    }
    // δ����ķ�������ʽ���Ҳ�������ӳ��������
    else if (!flag && type != "regularOperator" && key_value[index].count(name) == 0)
    {
        value_key[index][value_key[index].size()] = name;
        key_value[index][name] = key_value[index].size();
    }
    // �½������������Ӧ���֣�������expUnitGroupβ
    RegUnit tempRU(name, type);
    expUnitGroup[index].insert(expUnitGroup[index].end(), tempRU);
}

// ����ʽ��expUnitGroup�е�index����ӷָ���"&"���ָ��ַ��ʹ�
void Regex::changeRegularExp(int index)
{
    RegUnit temp_op("&", "temp_operator"); // Ϊ���ӷ�����һ������������ʽ
    for (unsigned int i = 1; i < expUnitGroup[index].size(); i++)
    {
        if (i + 1 == expUnitGroup[index].size())
            break; // ��ȡ���������ѭ��
        // ��һ���������������������һ���������������"("
        if (expUnitGroup[index][i + 1].type != "regularOperator" ||
            (expUnitGroup[index][i + 1].name == "(" && expUnitGroup[index][i + 1].type == "regularOperator"))
        {
            // ��ǰ�������������������һ��λ�ò���"&"
            if (expUnitGroup[index][i].type != "regularOperator")
            {
                i++;
                expUnitGroup[index].insert(expUnitGroup[index].begin() + i, temp_op);
            }
            // ��ǰ��������������ֱհ�"*"�����հ�"+"����ѡ"��"��������")"�е�һ��,����һ��λ�ò���"&"
            else if (expUnitGroup[index][i].type == "regularOperator")
            {
                if (expUnitGroup[index][i].name == "*" || expUnitGroup[index][i].name == "+" ||
                    expUnitGroup[index][i].name == "?" || expUnitGroup[index][i].name == ")")
                {
                    i++;
                    expUnitGroup[index].insert(expUnitGroup[index].begin() + i, temp_op);
                }
            }
        }
    }
}

// ʶ����������ʽ��exp:���ʽ���ַ�����index:��index��
void Regex::identifyRegularExp(string exp, int index)
{
    bool is_left = 1; // 1Ϊ���ʽ�󲿣�0Ϊ���ʽ�Ҳ����տ�ʼ�϶����󲿣�Ĭ�ϱ��
    string tempStr = "";
    for (unsigned int i = 0; i < exp.length(); i++)
    {
        if (is_left && exp[i] != '=')
        { // �洢'='��ߵ������ַ���Ҳ����������ʽ������
            tempStr += exp[i];
        }
        else if (exp[i] == '=' && is_left)
        { // ���󲿱������¶�ȡ����һ��'='��������insertExpGroup��������expUnitGroup��β��ȡ���󲿱��
            insertExpGroup(tempStr, "wordType", index, is_left);
            is_left = 0;
            tempStr = ""; // ���ô�
        }
        else if (!is_left)
        { // �Ҳ�
            if (exp[i] == '\\')
            {   // ����'\'˵����ת���ַ�����ʱ��������'/'����¼��һ���ַ�
                //                                while (i + 1 < exp.length() &&
                //                                !isRegualrOperator(exp[i + 1]))
                //                                    if (exp[i + 1] == '\\')
                //                                        tempStr += exp[++i];
                tempStr += exp[++i];
                insertExpGroup(tempStr, expUnitGroup[index][0].name, index, is_left);
            }
            //                        if (exp[i] == '\\') { //�����Ҳ�����'\'˵����ת���ַ�
            //                            i++;
            //                            tempStr += exp[i];
            //                            if (exp[i] == '\\') {
            //                                i++;
            //                                tempStr += exp[i];
            //                            }
            //                            /*while (i + 1 < exp.length() &&
            //                            !isRegualrOperator(exp[i + 1])) {
            //                                if (exp[i + 1] == '\\') {
            //                                    i++;
            //                                }
            //                                tempStr += exp[++i];
            //                            }*/
            //                            insertExpGroup(tempStr, expUnitGroup[index][0].name,
            //                            index, is_left);
            //                        }
            else if (exp[i] == '[')
            { // �������ַ�������һ���������,[]��������൱��һ���ַ�
                tempStr += exp[i];
                while (i + 1 < exp.length() && exp[i] != ']')
                {
                    tempStr += exp[++i];
                }
                insertExpGroup(tempStr, expUnitGroup[index][0].name, index, is_left);
            }
            else if (!isRegualrOperator(exp[i]))
            { // ����������������¿�һ����������Ͳ�������ʽ��
                tempStr += exp[i];
                /*while (i + 1 < exp.length() && !isRegualrOperator(exp[i + 1])) {
                    i++;
                    tempStr += exp[i];
                }*/
                insertExpGroup(tempStr, expUnitGroup[index][0].name, index, is_left);
            }
            else if (isRegualrOperator(exp[i]))
            { // ����������������뵽����������ı��ʽ����
                tempStr += exp[i];
                insertExpGroup(tempStr, "regularOperator", index, is_left);
            }
            tempStr = ""; // ���ô�
        }
    }
}

// �ж��ַ�c�Ƿ�Ϊ���������
bool Regex::isRegualrOperator(char c)
{
    return find(regularOperator.begin(), regularOperator.end(), c) != regularOperator.end();
}

// ת��Ϊ�沨�����ʽ����׺��
void Regex::change2Post(int index)
{
    stack<RegUnit> sta; // ����ջ
    RegUnit tempUnit("#",
                     "regularOperator"); // �����ַ�'#'��Ϊ���������,tempUnitΪ������RegUnit
    sta.push(tempUnit);                  //'#'��ջ����ջ����Ϊ������ʶ

    queue<RegUnit> post_exp;                        // ���ڴ洢ת�������������ñ��ʽ
    vector<RegUnit> temp_exp = expUnitGroup[index]; // ��ǰ������index��������ʽ
    temp_exp.insert(temp_exp.end(), tempUnit);      // β�������ַ�'#'
    temp_exp.erase(temp_exp.begin());

    // int len = expUnitGroup[index].size() - 1;
    int cnt = 0;                                              // ��ǰRegUnit���±�
    tempUnit.setUnit(temp_exp[cnt].name, temp_exp[cnt].type); // ����tempUnit
    while (!sta.empty() && tempUnit.name != "#")
    { // ջ�����ҵ�ǰû����ջ�׵�'#'
        if (tempUnit.type == expUnitGroup[index][0].name)
        { // ����==����˵��Ϊ���������ֱ�ӽ����׺���ʽ
            post_exp.push(tempUnit);
            ++cnt;
            tempUnit.setUnit(temp_exp[cnt].name, temp_exp[cnt].type);
        }
        // ��ǰΪ�հ������հ�����ѡ�����ȼ�������'('��')'��'['��']'���ñȽϴ�Сֱ�ӽ����׺���ʽ�У���Ϊջ�в������б�����������ȼ��ߵ�
        else if (tempUnit.name == "*" || tempUnit.name == "+" || tempUnit.name == "?")
        {
            post_exp.push(tempUnit);
            ++cnt;
            tempUnit.setUnit(temp_exp[cnt].name, temp_exp[cnt].type);
        }
        else
        {
            char icop = tempUnit.name[0];
            char isop = sta.top().name[0];
            // ��ǰջ���ַ�ջ�����ȼ�С�ڱ��������ַ�ջ�����ȼ�����ǰ������ջ���ƶ��±�cnt������tempUnit
            if (isp[isop] < icp[icop])
            {
                sta.push(tempUnit);
                ++cnt;
                tempUnit.setUnit(temp_exp[cnt].name, temp_exp[cnt].type);
            }
            // ��ǰջ���ַ�ջ�����ȼ����ڱ��������ַ�ջ�����ȼ���ջ���ַ���ջ�������׺���ʽ��
            else if (isp[isop] > icp[icop])
            {
                RegUnit tempOp(sta.top());
                sta.pop();
                post_exp.push(tempOp);
            }
            // ���ַ�ջ�������ȼ���ȣ��洢ջ��Ԫ�غ��ջ������Ƿ��������ţ��ǵĻ��ƶ��±�cnt������tempUnit
            else
            {
                RegUnit tempOp(sta.top());
                sta.pop();
                if (tempOp.name == "(")
                {
                    ++cnt;
                    tempUnit.setUnit(temp_exp[cnt].name, temp_exp[cnt].type);
                }
            }
        }
    }
    // �����ʽ��ȡ����Ƿ����������ջ�У���������ջ�����
    while (!sta.empty() && sta.top().name != "#")
    {
        post_exp.push(sta.top());
        sta.pop();
    }
    // �滻ԭ��expUnitGroup[index]�еĳ�Ա
    expUnitGroup[index].erase(expUnitGroup[index].begin() + 1, expUnitGroup[index].end());
    while (!post_exp.empty())
    {
        expUnitGroup[index].push_back(post_exp.front());
        post_exp.pop();
    }
}
