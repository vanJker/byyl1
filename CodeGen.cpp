#include "CodeGen.h"

GenerationSYS::GenerationSYS()
{
    lexcode = "";
}

void GenerationSYS::setProgram()
{
    addInclude();
    addFunStatement();
    addFun_identifyKeyword();
    addFun_identfiy();
    string endcoing_code = addFun_encoding();

    lexcode += headfile;           // 加入头文件
    lexcode += addFun_TokenType(); // 加入单词类型
    // lexcode += addSideMap();
    lexcode += addGlobalvar();
    lexcode += funstatement + "\n"; // 加入函数声明
    lexcode += addFun_isChar();
    lexcode += keyword_code + addFun_isKeyword();
    lexcode += identify_code;
    lexcode += addFun_isWhiteSpace();
    lexcode += addFun_combinStr();
    lexcode += identFun_code;
    lexcode += endcoing_code;
    lexcode += addFun_showWord();
    lexcode += addFun_getSourceFile();
    lexcode += addFun_readFile();
    lexcode += addFun_writedata();
    lexcode += addFun_main();
}

string GenerationSYS::getCode()
{
    return lexcode;
}

void GenerationSYS::addInclude()
{
    headfile = "#include <stdio.h>\n"
               "#include <stdbool.h>\n"
               "#include <stdlib.h>\n"
               "#include <assert.h>\n"
               "#include <string.h>\n";
}

void GenerationSYS::addInclude(string hname)
{
    headfile += "#include <" + hname + ">\n";
}

void GenerationSYS::addFunStatement()
{
    funstatement = "bool isChar(char c);\n"
                   "bool isWhiteSpace(char c);\n"
                   "enum TokenType isKeyword(enum TokenType tok, char *s);\n"
                   "char* combindStr(char* a, char* b);\n"
                   "char* getSourceFile();\n"
                   "char* readFile();\n"
                   "void showWord(enum TokenType tok, char* c);\n";
}

void GenerationSYS::addFunStatement(string statement)
{
    funstatement += statement + ";\n";
}

string GenerationSYS::addFun_isChar()
{
    string code = "bool isChar(char c){    //identify the c whether a char\n"
                  "\tif ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))\n"
                  "\t\treturn true;\n"
                  "\treturn false;\n}\n\n";
    return code;
}

string GenerationSYS::addGlobalvar()
{
    string code = "#define LINE_MAX 1024\n"
                  "int lineno = 1;  //current row\n"
                  "int linepos = 1;\t//current positon of the line\n"
                  "int pospoint = 0;   //current character position\n"
                  "int linesize = 0;   //filedata has how many row\n"
                  "int datalen = 0;      //data length\n"
                  "bool saveflag = true;  //whether should save the current char\n"
                  "enum TokenType token;  //current tokentype\n"
                  "char line[LINE_MAX] = \"\";\n"
                  "char* word = \"\";\n"
                  "char* result_data=\"\";\n"
                  "char* code_result=\"\";\n"
                  "char* source_code = \"\"; \n\n";
    return code;
}

string GenerationSYS::addFun_TokenType()
{
    string code = "enum TokenType{\n\tERR, NONE";

    for (map<string, int>::iterator iter = wordType.begin(); iter != wordType.end(); iter++)
    {
        string tempN = iter->first;
        transform(tempN.begin(), tempN.end(), tempN.begin(), ::toupper);
        code += ", " + tempN;
    }
    code += "\n};\n\n";
    return code;
}

string GenerationSYS::addFun_isOperator()
{
    string code = "bool isOperator(char c){\n\tif(";

    int index = wordType["operator"];
    for (unsigned int i = 0; i < key_value[index].size(); i++)
    { // 找出运算符
        if (value_key[index][i].length() > 1)
            continue; // 处理一个字节长度的运算符
        if (i == 0)   // 第一个则开头无"||"，其余用或连接
            code += "c == \'" + value_key[index][i] + "\'";
        else
            code += " || c == \'" + value_key[index][i] + "\'";
    }
    code += ";\n}\n\n";
    return code;
}

string GenerationSYS::unpackCon(string con, string x)
{
    string tstr = con;
    string ch = x;
    string unpackStr = "";
    int len = tstr.length();
    bool flag_pack = 0; // 是否为[]
    if (tstr.length() > 2 && tstr[0] == '[' && tstr[len - 1] == ']')
    { // 判断是否为[]类型，是的话删除'['和']'符号后标记
        tstr.erase(tstr.begin());
        tstr.erase(tstr.end() - 1);
        flag_pack = 1;
    }
    if (!flag_pack)
    { // 不是[]类型，加入串处理完后直接返回
        for (int i = 0; i < len; i++)
        { // 查看当前字符和下一个字符是否组成换行，制表和回车符,是的话把'\n'、'\t'、'\r'加入到要返回的串中
            if (i > 0)
                unpackStr += " || ";
            if (tstr[i] == '\\' && tstr[i + 1] == 'n')
            {
                i++;
                unpackStr += ch + " == \'\\n\'";
            }
            else if (tstr[i] == '\\' && tstr[i + 1] == 't')
            {
                i++;
                // unpackStr += ch + " == \'\n\'";
                unpackStr += ch + " == \'\\t\'";
            }
            else if (tstr[i] == '\\' && tstr[i + 1] == 'r')
            {
                i++;
                // unpackStr += ch + " == \'\n\'";
                unpackStr += ch + " == \'\\r\'";
            }
            else
            { // 都不是,把'tstr[i]'加入要返回的串中
                // string ts = "";ts += tstr[i];//char转string
                unpackStr += ch + " == \'" + tstr[i] + '\'';
            }
        }
        return unpackStr; // 返回拼接好的串
    }
    // 处理[]类型
    bool flag = 0; // 判断是否取反
    if (tstr[0] == '^')
    { // 检测到取反符'^'，标记为取反后删除取反符'^'
        flag = 1;
        tstr.erase(tstr.begin());
    }
    // 判断[]内是否有符号'-'，例如[a-z]
    for (unsigned int i = 1; i < tstr.length() - 1; i++)
    {
        // 要求前ascii码<后ascii码
        if (tstr[i] == '-' && ((int)tstr[i - 1] < (int)tstr[i + 1]))
        {
            int from = (int)tstr[i - 1];
            int to = (int)tstr[i + 1];
            tstr.erase(tstr.begin() + i); // 去掉运算符'-'
            // 转int之后把跨度中的符号或者字母全部加入串中（利用ascii连续这个特性）
            for (int j = to - 1; j > from; j--)
            {
                char c = (char)j;
                string tc = "";
                tc += c; // char转string
                tstr.insert(i, tc);
            }
        }
    }
    if (tstr.find("\\n") != string::npos)
    { // 如果存在换行符，标记完位置之后截取，同时读入串中
        int index = tstr.find("\\n");
        tstr.erase(index, 2);
        tstr.insert(index, "\\n");
    }
    if (!flag)
    { // 处理不是取反的情况，直接拼接
        for (unsigned int i = 0; i < tstr.length(); i++)
        {
            // string ts = "";ts += tstr[i];//char转string
            if (i > 0)
                unpackStr += "|| " + ch + " == \'" + tstr[i] + "\' ";
            else
                unpackStr += ch + " == \'" + tstr[i] + "\' ";
        }
    }
    else
    { // 处理取反的情况，^(a||b)等价于(^a&&^b)
        if (tstr == "\\n")
        {
            return ch + " != \'\\n\'";
        }
        for (unsigned int i = 0; i < tstr.length(); i++)
        {
            string ts = "";
            ts += tstr[i];
            if (i > 0)
                unpackStr += "&& " + ch + " != \'" + ts + "\' ";
            else
                unpackStr += ch + " != \'" + ts + "\' ";
        }
    }
    return unpackStr; // 返回拼接好的串
}

void GenerationSYS::addFun_identifyKeyword()
{
    string funname = "";
    string code = "";
    for (unsigned int i = 0; i < languageType["keyword"].size(); i++)
    {                                           // 为每一个关键字添加一个识别函数
        int index = languageType["keyword"][i]; // 关键字所在的位置
        string keywordname = indexType[index];
        transform(keywordname.begin(), keywordname.end(), keywordname.begin(),
                  ::toupper);                            // keywordname所有小写转大写
        funname = "bool is" + keywordname + "(char* c)"; // 函数名
        addFunStatement(funname);                        // 添加函数声明
        string identcode = "";                           // 函数体
        // 按长度进行初步筛选
        identcode += funname + " {\n\tif(strlen(c)!=" + to_string(mdfaNodeAccount[index] - 1) +
                     ") return false;\n\tint cnt=0;\n";
        for (int j = 1; j != mdfaNodeAccount[index];)
        { // 遍历下标为index关键字的最小化dfa向量
            int tempj = j;
            for (unsigned int k = 1; k < value_key[index].size(); k++)
            { // 在当前状态j下寻找对应的字符k
                if (mdfaTable[index][j][k] > 0)
                { // 查找到状态j对应字符k有转跳
                    string top = value_key[index][k];
                    // 如果带有[]运算，进入处理[]函数unpackCon，返回对应拼接串，单个字符则直接拼接
                    if (top.length() > 1 && top[0] == '[' && top[top.length() - 1] == ']')
                        identcode += "\tif (" + unpackCon(top, "c[cnt]") + ") cnt++; else return false;\n";
                    else if (top.length() == 1)
                        identcode += "\tif (c[cnt]== \'" + top + "\') cnt++; else return false;\n";
                    j = mdfaTable[index][j][k]; // j跳到下一个状态
                }
            }
            // 自环则退出，说明mindfa出问题
            if (tempj == j)
                break;
        }
        identcode += "\treturn true;\n}\n\n";
        code += identcode;
    }
    keyword_code += code;
}

string GenerationSYS::addFun_isKeyword()
{
    string code = "enum TokenType isKeyword(enum TokenType tok, char* str){\n";
    for (unsigned int i = 0; i < languageType["keyword"].size(); i++)
    {
        int index = languageType["keyword"][i]; // 关键字所在的位置
        string keywordname = indexType[index];
        transform(keywordname.begin(), keywordname.end(), keywordname.begin(), ::toupper);
        code += "\t";
        if (i > 0)
        {
            code += "else ";
        }
        code += "if (is" + keywordname + "(str)) return " + keywordname + ";\n";
    }
    code += "\treturn tok;\n}\n\n";
    return code;
}

void GenerationSYS::addFun_identfiy()
{
    string funname = "";
    string code = "";
    for (auto it : languageType)
    {
        if (it.first == "keyword" || it.first == "statement")
            continue;
        for (unsigned int i = 0; i < it.second.size(); i++)
        { // 处理每一个dfa表
            int index = it.second[i];
            string tpname = indexType[index];
            transform(tpname.begin(), tpname.end(), tpname.begin(), ::toupper);
            funname = "bool is" + tpname + "(char* c)";
            addFunStatement(funname); // 添加函数声明

            int xsize = mdfaNodeAccount[index] + 1;
            int ysize = key_value[index].size();
            string mapcode = "int map" + tpname + "[" + to_string(xsize) + "][" + to_string(ysize) + "]={\n";
            for (int j = 0; j < xsize; j++)
            {
                mapcode += "\t{";
                for (int k = 0; k < ysize; k++)
                { // 生成对应的dfa表
                    if (k > 0)
                        mapcode += ", ";
                    mapcode += to_string(mdfaTable[index][j][k]);
                }
                mapcode += "},\n";
            }
            mapcode.erase(mapcode.end() - 2, mapcode.end() - 1); // 去掉末尾的",\n"
            mapcode += "};\n";
            string funcode = funname + " {\t//judge the c whether equal with" + tpname + "\n";
            string opcode = "\tchar op[" + to_string(ysize) + "][32]={";
            string endcode = "\tint end[" + to_string(mdfaEndState[index].size()) + "]={";
            for (int j = 0; j < ysize; j++)
            { // 添加匹配符
                if (j > 0)
                    opcode += ", ";
                opcode += "\"" + value_key[index][j] + "\"";
            }
            opcode += "};\n";
            funcode += opcode;
            for (set<int>::iterator it = mdfaEndState[index].begin(); it != mdfaEndState[index].end(); it++)
            { // 添加终态
                if (it != mdfaEndState[index].begin())
                    endcode += ", ";
                endcode += to_string(*it);
            }
            endcode += "};\n";
            funcode += endcode;
            funcode += "\tint pos=0; int sta=1; int xsize=" + to_string(xsize) + "; int ysize=" + to_string(ysize) +
                       ";\n\twhile(pos<strlen(c)){\n\t\tint i=1;\n";
            funcode += "\t\tfor(i=1;i<ysize;i++){\n";
            funcode += "\t\t\tif(map" + tpname +
                       "[sta][i]>0 && strlen(op[i])==1 && op[i][0]==c[pos]) {pos++; "
                       "sta=map" +
                       tpname + "[sta][i]; break;}\n";
            for (int j = 1; j < ysize; j++)
            {
                if (value_key[index][j].length() > 1)
                    funcode += "\t\t\tif(map" + tpname + "[sta][i]>0 && strlen(op[i])>1 && isType" +
                               to_string(tkeyVal[value_key[index][j]]) + "(c[pos])) {pos++; sta=map" + tpname +
                               "[sta][i];break;}\n";
            }
            funcode += "\t\t}\n\t\tif(i==ysize) return false;\n\t}\n";
            funcode += "\tif(pos==strlen(c)) return true;\n\treturn false;\n}\n\n";
            code += mapcode + funcode;
        }
    }
    identify_code += code;
}

string GenerationSYS::addFun_isWhiteSpace()
{
    string code = "bool isWhiteSpace(char c) {   //judge whether a whiteSpace\n"
                  "\tif (c == ' ' || c == '\\t' || c == '\\n')\n"
                  "\t\treturn true;\n"
                  "\treturn false;\n}\n\n";
    return code;
}

string GenerationSYS::addFun_combinStr()
{
    string code = "char* combindStr(char* a, char* b){\n"
                  "\tchar* new = (char*)malloc(strlen(a) + strlen(b) + 1);\n"
                  "\tstrcpy(new, a);\n"
                  "\tstrcat(new, b);\n"
                  "\treturn new;\n}\n\n";
    return code;
}

void GenerationSYS::addFun_marchFun(string data, int state)
{ // 针对dfa中的匹配符
    if (funstate_set.count(data) > 0)
        return; // 如若已经声明则跳过
    funstate_set.insert(data);

    string funname = "bool isType"; // 生成的识别函数的函数名
    funname += to_string(state) + "(char c)";
    addFunStatement(funname); // 在声明部分添加函数声明

    string code = funname + " {\t//identify whether the ch march the " + data + "\n";
    string tstr = data;

    code += "\tif (";
    code += unpackCon(tstr, "c");
    code += ")\n\t\treturn true;\n\treturn false;\n}\n\n";
    identFun_code += code;
}

string GenerationSYS::addFun_encoding()
{
    string code = "void encoding(char* data){\n\tint pospoint=0;\n\tint "
                  "prepos=pospoint;\t//previous position\n\tint "
                  "state=1;\n\tint linepos=0;\n";
    code += "\twhile(pospoint < datalen){\n\t\tsaveflag=true;\n\t\tchar "
            "ch=data[pospoint];\n\t\tswitch(state){\n";

    for (unsigned int i = 1; i < valTkey.size() - 2; i++)
    { // 暂不处理error和else
        string marchop = valTkey[i];
        if (marchop.length() > 1)
        {
            addFun_marchFun(marchop, i); // 添加识别函数
        }
    }

    for (int i = 1; i < tdfaNodeAccount; i++)
    { // 先不处理error点
        if (i == endNode)
            continue;
        int flag1 = 0; // 要对第一行做处理
        if (i == 1)
        {
            flag1 = 1;
        }
        code += "\t\t\tcase " + to_string(i) + ":\n";
        if (flag1)
        {
            code += "\t\t\t\tword=\"\";\n";
        }
        int cnt = 0; // 判断是否为case中的第一个
        for (unsigned int j = 1; j < valTkey.size() - 1; j++)
        { // 不看other匹配符
            if (totalDfa[i][j] > 0)
            { // 如若存在路径
                if (j != tkeyVal["else"])
                {
                    code += "\t\t\t\t";
                    if (cnt > 0)
                    {
                        code += "else ";
                    }
                    string nextstate = to_string(totalDfa[i][j]);
                    string marchop = valTkey[j];
                    if (marchop.length() > 1)
                    {
                        code += "if(isType" + to_string(j) + "(ch)) state=" + nextstate + ";\n"; // 进入对应状态
                    }
                    else
                    {
                        code += "if(ch=='" + marchop + "') state=" + nextstate + ";\n"; // 进入对应状态
                    }
                    cnt++;
                }
                else
                {
                    code += "\t\t\t\t";
                    if (cnt > 0)
                    { // 如若i能到达其他点
                        code += "else {state = " + to_string(endNode) + "; ";
                        if (endReturn.size() == 1)
                        {
                            string toktype = *endReturn[i].begin();
                            transform(toktype.begin(), toktype.end(), toktype.begin(), ::toupper);
                            code += "\n\t\t\t\t\ttoken=" + toktype + "; ";
                        }
                        else if (endReturn.size() > 1)
                        {
                            code += "\n";
                            for (set<string>::iterator it = endReturn[i].begin(); it != endReturn[i].end(); it++)
                            {
                                code += "\t\t\t\t\t";
                                if (it != endReturn[i].begin())
                                    code += "else ";
                                string toktype = *it;
                                transform(toktype.begin(), toktype.end(), toktype.begin(), ::toupper);
                                code += "if (is" + toktype + "(word)) token = " + toktype + ";\n";
                            }
                        }
                        code += "\t\t\t\t\tpospoint--; saveflag = false;}\n";
                    }
                    if (cnt == 0)
                    { // i不能到达其他点则直接跳到结尾
                        code += "state = " + to_string(endNode) + ";";
                        if (endReturn.size() == 1)
                        {
                            string toktype = *endReturn[i].begin();
                            transform(toktype.begin(), toktype.end(), toktype.begin(), ::toupper);
                            code += "\n\t\t\t\ttoken=" + toktype + "; ";
                        }
                        else if (endReturn.size() > 1)
                        {
                            code += "\n";
                            for (set<string>::iterator it = endReturn[i].begin(); it != endReturn[i].end(); it++)
                            {
                                code += "\t\t\t\t";
                                if (it != endReturn[i].begin())
                                    code += "else ";
                                string toktype = *it;
                                transform(toktype.begin(), toktype.end(), toktype.begin(), ::toupper);
                                code += "if (is" + toktype + "(word)) token = " + toktype + ";\n";
                            }
                        }
                        code += "\t\t\t\tpospoint--; saveflag = false;\n";
                    }
                }
            }
        }
        if (flag1)
        {
            code += "\t\t\t\telse if(isChar(ch)) {state = " + to_string(tdfaNodeAccount + 1) + ";}\n";
            code += "\t\t\t\telse if(isWhiteSpace(ch)) {saveflag = false;}\n";
            code += "\t\t\t\telse state = " + to_string(errorNode) + ";\n"; // 如若遇到错误则跳到error点
        }
        code += "\t\t\t\tbreak;\n";
    }
    // 处理error点
    code += "\t\t\tcase " + to_string(errorNode) + ":\n\t\t\t\tif(";
    int tcnt = 0;
    for (unsigned int i = 1; i < tkeyVal.size() - 1; i++)
    { // 去除本身
        if (totalDfa[startNode][i] > 0)
        {
            if (tcnt > 0)
                code += "||";
            string nextstate = to_string(totalDfa[startNode][i]);
            string marchop = valTkey[i];
            if (marchop.length() > 1)
            {
                code += "isType" + to_string(i) + "(ch)"; // 进入对应状态
            }
            else
            {
                code += "ch=='" + marchop + "'"; // 进入对应状态
            }
            tcnt++;
        }
    }
    code += ")\n\t\t\t\t{state=" + to_string(endNode) + "; pospoint--; saveflag=false; token= ERR;}\n\t\t\tbreak;\n";

    code += "\t\t\tcase " + to_string(tdfaNodeAccount + 1) +
            ":\n\t\t\t\tif(isChar(ch)) state=" + to_string(tdfaNodeAccount + 1) +
            ";\n\t\t\t\telse {state=" + to_string(endNode) + "; pospoint--; saveflag=false; token=NONE;}\n";
    code += "\t\t\t\tbreak;\n";
    code += "\t\t\t}\n";
    // 执行switch后的操作
    code += "\t\tif(saveflag) {char tempstr[2]={ch}; "
            "word=combindStr(word,tempstr);}\n";

    code += "\t\tif(state==" + to_string(endNode) +
            ") {state=1; showWord(isKeyword(token,word),word); word=\"\";}\n"; // showWord()函数待补
    code += "\t\tch=data[pospoint];\n\t\tif(ch=='\\n'&& pospoint!=prepos) {lineno++; "
            "prepos=pospoint;}\n\t\tpospoint++;\n";
    code += "\t}\n}\n\n";
    return code;
}

string GenerationSYS::addFun_getSourceFile()
{
    string code = "char* getSourceFile() {\n"
                  "\tchar* path = (char*)malloc(300 * sizeof(char));\n"
                  "\tmemset(path, 0, 300 * sizeof(char));\n"
                  "\tprintf(\"please enter the path of the sourcefile: \");\n"
                  "\tscanf(\"%s\", path);\n"
                  "\treturn path;\n}\n\n";
    return code;
}

string GenerationSYS::addFun_readFile()
{
    string code = "char* readFile(){\n"
                  "\t//open the file first\n"
                  //"\tchar filename[300] = \"\";\n"
                  //"\tstrcpy(filename, "+sourec_filename+");\n"
                  "\tFILE* file = fopen(\"" +
                  srcfilename +
                  "\", \"r\");\n"
                  "\t//judge the file\n"
                  "\tif (file == NULL) {\n"
                  "\t\tprintf(\"Open The File ERROR!!!\");\n"
                  "\t\texit(1);\n"
                  "\t}\n"
                  "\tchar ch;\n"
                  "\tchar* data = \"\"; \n"
                  "\twhile (!feof(file)) {\n"
                  "\t\tch = fgetc(file); \n"
                  "\t\tdatalen++;\n"
                  "\t\tchar temp[2] = { ch }; \n"
                  "\t\tdata = combindStr(data, temp); \n"
                  "\t\tif (ch == '\\n' || ch == EOF)\n"
                  "\t\t\tlinesize++; \n"
                  "\t}\n"
                  "\treturn data; \n}\n\n";
    return code;
}

string GenerationSYS::addFun_showWord()
{
    string code = "void showWord(enum TokenType tok, char* c) {\t//show the word that has "
                  "been recognized\n";
    code += "\tswitch(tok){\n";
    for (auto it : languageType)
    {
        if (it.first == "statement")
            continue;
        for (unsigned int i = 0; i < it.second.size(); i++)
        {
            int index = it.second[i];
            string tpname = indexType[index];
            transform(tpname.begin(), tpname.end(), tpname.begin(), ::toupper);
            code += "\t\tcase " + tpname + ":\n";
            string ttype = "\"" + tpname + " \"";
            code += "\t\t\tprintf(\"\tline%d: " + it.first + ": <" + tpname +
                    ",%s>\\n\", lineno, c); code_result=combindStr(code_result," + ttype +
                    "); code_result=combindStr(code_result,c); "
                    "code_result=combindStr(code_result,\"\\n\"); break;\n";
        }
    }
    string terror = "\"error \"";
    code += "\t\tcase ERR:\n\t\t\tprintf(\"\tline%d: ERROR: error: %s\\n\", lineno, "
            "c); "
            "code_result=combindStr(code_result," +
            terror +
            "); code_result=combindStr(code_result,c); "
            "code_result=combindStr(code_result,\"\\n\"); break;\n";
    code += "\t\tdefault: break;\n\t}\n}\n\n";
    return code;
}

string GenerationSYS::addFun_main()
{
    string code = "int main() {\n";
    code += "\tsource_code=readFile();\n\tencoding(source_code);\n\twriteFile();\n}";
    return code;
}

void GenerationSYS::compile(string filename)
{
    string command = "gcc " + filename;
    string run = "./" + filename;
    system(command.c_str());
    system(run.c_str());
}

string GenerationSYS::addFun_writedata()
{
    string codefilename = srcfilename;
    int i = 0;
    for (i = codefilename.length() - 1; i > 0; i--)
    {
        if (codefilename[i] == '.')
            break;
    }
    codefilename.erase(codefilename.begin() + i, codefilename.end());
    codefilename += ".txt";
    string code = "void writeFile() {\n\tFILE* file = fopen(\"" + codefilename +
                  "\", \"w\");\n"
                  "\t//judge the file\n"
                  "\tif (file == NULL) {\n"
                  "\t\tprintf(\"Open The File ERROR!!!\");\n"
                  "\t\texit(1);\n"
                  "\t}\n"
                  "\tfprintf(file,\"%s\",code_result);\n}\n\n";
    return code;
}
