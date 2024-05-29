#include "Regex.h"

// 无参构造，初始化默认正则运算符以及其栈内外优先级和正则表达式数量
Regex::Regex()
{
    regularOperator = {'(', ')', '[', ']', '+', '*', '?', '&', '|'}; // 正则运算符
    isp = {{'#', 0}, {'(', 1}, {'[', 1}, {'*', 7}, {'+', 7},
           {'?', 7}, {'&', 5}, {'|', 3}, {')', 8}, {']', 8}}; // 运算符栈内优先级，#为空，优先级最低
    icp = {{'#', 0}, {'(', 8}, {'[', 8}, {'*', 6}, {'+', 6},
           {'?', 6}, {'&', 4}, {'|', 2}, {')', 1}, {']', 1}}; // 运算符栈外优先级，#为空，优先级最低
    expsCount = 0;                                            // 正则表达式数量
}

// 将读取到的文本字符串拆分成单条的字符串，data:输入文本内容
void Regex::initRegularExpresions(string data)
{
    int cnt = 0; // 正则表达式数量
    string temp[1024];
    for (unsigned int i = 0; i < data.length(); i++)
    { // 把输入数据按行存入temp
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
    { // 遍历temp进行成员的初始化
        if (temp[i].length() < 1)
            continue; // 空
//        if (temp[i].length() > 2 && temp[i][0] == '/' && temp[i][1] == '/')
//        { // 发现新的文法类型
//            string strtype = "";
//            for (unsigned int j = 2; j < temp[i].length(); j++)
//            {
//                // j=2开始说明要越过"//"读取新文法类型名
//                if (temp[i][j] != '\n')
//                    strtype += temp[i][j];
//            }
//            // 为新的文法类型在languageType中添加一个vector<int>用于存入当前非新文法类型名（单条正则表达式）的相对行下标
//            current_type = strtype;
//            vector<int> tempV;
//            languageType[current_type] = tempV;
//        }
        if (temp[i].length() > 1 && temp[i][0] == '#')
        { // 发现新的文法类型
            string strtype = "";
            for (unsigned int j = 1; j < temp[i].length(); j++)
            {
                // j=1开始说明要越过"#"读取新文法类型名
                if (temp[i][j] != '\n')
                    strtype += temp[i][j];
            }
            // 为新的文法类型在languageType中添加一个vector<int>用于存入当前非新文法类型名（单条正则表达式）的相对行下标
            current_type = strtype;
            vector<int> tempV;
            languageType[current_type] = tempV;
        }
        else
        { // 单条正则表达式的读入
            regexps.push_back(temp[i]);
            expsCount++;
            int index = i - languageType.size(); // 存入单条正则表达式的相对行下标（跳过声明新文法类型名的行）
            languageType[current_type].push_back(index);
            // 初始化正则表达式运算符映射，默认"#"为结束符，优先级最低
            map<string, int> kv;
            kv["#"] = 0;
            map<int, string> vk;
            vk[0] = "#";
            key_value.push_back(kv);
            value_key.push_back(vk);
        }
    }
    // 初始化expUnitGroup成员，开辟expsCount大小
    expUnitGroup.resize(expsCount);
}

// 检查并处理每一条正则表达式文法
void Regex::checkRegularExp()
{
    for (int i = 0; i < expsCount; i++)
    {
        identifyRegularExp(regexps[i],
                           i); // 识别正则表达式中的运算量，exp:表达式，index:表示第index条
        changeRegularExp(i);   // 遍历expUnitGroup添加连接符号"&"，连接多行表达式使计算机可以识别
        change2Post(i);        // 转化为逆波兰表达式（后缀）
    }
}

// 向exp_group中插入单元，flag:表示是否为左部
void Regex::insertExpGroup(string name, string type, int index, bool flag)
{
    // 未加入类型的左部，建立相对行下标和名称的映射
    if (flag && wordType.count(name) == 0)
    {
        wordType[name] = index;
        indexType[index] = name;
    }
    // 未加入的非正则表达式的右部，存入映射向量中
    else if (!flag && type != "regularOperator" && key_value[index].count(name) == 0)
    {
        value_key[index][value_key[index].size()] = name;
        key_value[index][name] = key_value[index].size();
    }
    // 新建运算类型与对应名字，并插入expUnitGroup尾
    RegUnit tempRU(name, type);
    expUnitGroup[index].insert(expUnitGroup[index].end(), tempRU);
}

// 向表达式组expUnitGroup中第index条添加分隔符"&"，分割字符和串
void Regex::changeRegularExp(int index)
{
    RegUnit temp_op("&", "temp_operator"); // 为连接符创建一个单条正则表达式
    for (unsigned int i = 1; i < expUnitGroup[index].size(); i++)
    {
        if (i + 1 == expUnitGroup[index].size())
            break; // 读取到最后跳出循环
        // 下一个不是正则运算符或者下一个是正则运算符的"("
        if (expUnitGroup[index][i + 1].type != "regularOperator" ||
            (expUnitGroup[index][i + 1].name == "(" && expUnitGroup[index][i + 1].type == "regularOperator"))
        {
            // 当前不是正则运算符，在下一个位置插入"&"
            if (expUnitGroup[index][i].type != "regularOperator")
            {
                i++;
                expUnitGroup[index].insert(expUnitGroup[index].begin() + i, temp_op);
            }
            // 当前是正则运算符克林闭包"*"、正闭包"+"、可选"？"、右括号")"中的一个,在下一个位置插入"&"
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

// 识别单条正则表达式，exp:表达式的字符串，index:第index条
void Regex::identifyRegularExp(string exp, int index)
{
    bool is_left = 1; // 1为表达式左部，0为表达式右部，刚开始肯定是左部，默认标记
    string tempStr = "";
    for (unsigned int i = 0; i < exp.length(); i++)
    {
        if (is_left && exp[i] != '=')
        { // 存储'='左边的所有字符，也就是正则表达式左部名称
            tempStr += exp[i];
        }
        else if (exp[i] == '=' && is_left)
        { // 在左部标记情况下读取到第一个'='符，调用insertExpGroup函数插入expUnitGroup的尾，取消左部标记
            insertExpGroup(tempStr, "wordType", index, is_left);
            is_left = 0;
            tempStr = ""; // 重置串
        }
        else if (!is_left)
        { // 右部
            if (exp[i] == '\\')
            {   // 出现'\'说明是转义字符，此时跳过符号'/'并记录下一个字符
                //                                while (i + 1 < exp.length() &&
                //                                !isRegualrOperator(exp[i + 1]))
                //                                    if (exp[i + 1] == '\\')
                //                                        tempStr += exp[++i];
                tempStr += exp[++i];
                insertExpGroup(tempStr, expUnitGroup[index][0].name, index, is_left);
            }
            //                        if (exp[i] == '\\') { //正则右部出现'\'说明是转义字符
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
            { // 把整个字符串当成一个整体接收,[]里的内容相当于一个字符
                tempStr += exp[i];
                while (i + 1 < exp.length() && exp[i] != ']')
                {
                    tempStr += exp[++i];
                }
                insertExpGroup(tempStr, expUnitGroup[index][0].name, index, is_left);
            }
            else if (!isRegualrOperator(exp[i]))
            { // 不是正则运算符，新开一个运算符类型并加入表达式组
                tempStr += exp[i];
                /*while (i + 1 < exp.length() && !isRegualrOperator(exp[i + 1])) {
                    i++;
                    tempStr += exp[i];
                }*/
                insertExpGroup(tempStr, expUnitGroup[index][0].name, index, is_left);
            }
            else if (isRegualrOperator(exp[i]))
            { // 是正则运算符，加入到正则运算符的表达式组下
                tempStr += exp[i];
                insertExpGroup(tempStr, "regularOperator", index, is_left);
            }
            tempStr = ""; // 重置串
        }
    }
}

// 判断字符c是否为正则运算符
bool Regex::isRegualrOperator(char c)
{
    return find(regularOperator.begin(), regularOperator.end(), c) != regularOperator.end();
}

// 转化为逆波兰表达式（后缀）
void Regex::change2Post(int index)
{
    stack<RegUnit> sta; // 工作栈
    RegUnit tempUnit("#",
                     "regularOperator"); // 创建字符'#'作为正则运算符,tempUnit为工作的RegUnit
    sta.push(tempUnit);                  //'#'入栈，在栈底作为结束标识

    queue<RegUnit> post_exp;                        // 用于存储转换后的运算符后置表达式
    vector<RegUnit> temp_exp = expUnitGroup[index]; // 当前处理到第index条正则表达式
    temp_exp.insert(temp_exp.end(), tempUnit);      // 尾部插入字符'#'
    temp_exp.erase(temp_exp.begin());

    // int len = expUnitGroup[index].size() - 1;
    int cnt = 0;                                              // 当前RegUnit的下标
    tempUnit.setUnit(temp_exp[cnt].name, temp_exp[cnt].type); // 更新tempUnit
    while (!sta.empty() && tempUnit.name != "#")
    { // 栈不空且当前没处理到栈底的'#'
        if (tempUnit.type == expUnitGroup[index][0].name)
        { // 类型==名字说明为非运算符，直接进入后缀表达式
            post_exp.push(tempUnit);
            ++cnt;
            tempUnit.setUnit(temp_exp[cnt].name, temp_exp[cnt].type);
        }
        // 当前为闭包、正闭包、可选，优先级仅次于'('、')'、'['、']'不用比较大小直接进入后缀表达式中，因为栈中不可能有比这个三个优先级高的
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
            // 当前栈顶字符栈内优先级小于遍历到的字符栈外优先级，当前符号入栈并移动下标cnt并更新tempUnit
            if (isp[isop] < icp[icop])
            {
                sta.push(tempUnit);
                ++cnt;
                tempUnit.setUnit(temp_exp[cnt].name, temp_exp[cnt].type);
            }
            // 当前栈顶字符栈内优先级大于遍历到的字符栈外优先级，栈顶字符出栈并进入后缀表达式中
            else if (isp[isop] > icp[icop])
            {
                RegUnit tempOp(sta.top());
                sta.pop();
                post_exp.push(tempOp);
            }
            // 两字符栈内外优先级相等，存储栈顶元素后出栈，检查是否是左括号，是的话移动下标cnt并更新tempUnit
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
    // 当表达式读取完后是否还有运算符在栈中，如有则退栈输出。
    while (!sta.empty() && sta.top().name != "#")
    {
        post_exp.push(sta.top());
        sta.pop();
    }
    // 替换原来expUnitGroup[index]中的成员
    expUnitGroup[index].erase(expUnitGroup[index].begin() + 1, expUnitGroup[index].end());
    while (!post_exp.empty())
    {
        expUnitGroup[index].push_back(post_exp.front());
        post_exp.pop();
    }
}
