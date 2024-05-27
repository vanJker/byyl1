#pragma once
#include <direct.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "TotalDFA.h"
using namespace std;

class GenerationSYS : public TotalDFA
{
  public:
    string lexcode = "";       // ���ɵĴʷ���������
    string srcfilename = "";   // Դ�ļ��ļ���
    string headfile = "";      // ͷ�ļ�����
    string funstatement = "";  // ������������
    set<string> funstate_set;  // �����������ϱ��ⷴ������
    string identFun_code = ""; // id
    string keyword_code = "";  // �ؼ���
    string identify_code = ""; // ���ؼ��֡�ע���������
    GenerationSYS();

    vector<int> word_code;
    vector<map<string, int>> fun_name; // �洢������
    vector<vector<int>> sidemap;       // ת���ĵõ�����·��ͼ
    map<pair<int, int>, int> side_id;  // �洢��Ӧ�ߵı��

    void setProgram(); // ���ɴʷ���������
    string getCode();  // ���شʷ����������ַ���

    // ���ɳ�����
    void addInclude();             // �޲γ�ʼ��Ĭ��ͷ�ļ�
    void addInclude(string hname); // ���ͷ�ļ�

    void addFunStatement();                 // �޲γ�ʼ��Ĭ�Ϻ�������
    void addFunStatement(string statement); // ��Ӻ�����statement

    string addGlobalvar();     // ���ȫ�ֱ���
    string addFun_TokenType(); // enum TokenType����
    string addFun_isChar();    // ����Ƿ�Ϊ�ַ����������ڸ��ؼ��ֽ��룩

    // �����еĹؼ��֣����������ʶ������ֵ��ע�ͣ��հ׵�
    string addFun_isWhiteSpace(); // ����Ƿ�Ϊ�հ׸���
    string addFun_isOperator();   // ����Ƿ�Ϊ���������
    string addFun_isKeyword();    // ����Ƿ�Ϊ�ؼ��ֺ���

    string unpackCon(string con,
                     string x);    // ������conת����x==con[i]��x!=con[i]����ʽ��con����ʽΪ[.*],xΪ������
    string addFun_combinStr();     // �����������char*���ַ�������
    string addFun_getSourceFile(); // ��ӻ�ȡԴ�����ļ�����
    string addFun_readFile();      // ��Ӷ�ȡԴ�������ݺ���
    string addFun_encoding();      // ��ӻ�ȡ���������
    void addFun_marchFun(string data,
                         int state); // ���ʶ��data�ĺ���,���dfa�е�ƥ���,stateΪƥ�����ֵ����
    void addFun_identifyKeyword();   // ���ʶ��ؼ���id�ĺ���
    void addFun_identfiy();          // ���ʶ���ʶ��id�ĺ���
    string addFun_showWord();        // ���������뺯��
    string addFun_main();            // ���main����

    string addFun_writedata(); // ������ʱ��뺯��

    void compile(string filename); // �������ɵĿ�ִ���ļ���
    void setSrc_filename(string source_filename)
    {
        srcfilename = source_filename;
    }
};
