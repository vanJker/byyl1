#pragma once

#include <qfiledialog.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qstring.h>
#include <qtablewidget.h>
#include <qtabwidget.h>

#include <QtWidgets/QMainWindow>

#include "CodeGen.h"
#include "DFA.h"
#include "FileProcessor.h"
#include "MinDFA.h"
#include "NFA.h"
#include "TotalDFA.h"
#include "ui_LexicalAnalysis.h"

class LexicalAnalysisProcedure : public QMainWindow
{
    Q_OBJECT

  public:
    LexicalAnalysisProcedure(QWidget *parent = nullptr);
    ~LexicalAnalysisProcedure();
    void initWidget();                            // ��ʼ����Ӧ��������
    void draw_nfa_table(NFA &nfa);                // ����nfa����״̬��
    void draw_dfa_table(DFA &dfa);                // ����dfa����״̬��
    void draw_mindfa_table(MinDFA &mdfa);         // ����dfa����״̬��
    void draw_totaldfa_table(TotalDFA &totaldfa); // ������dfa����״̬��
    void draw_Program(GenerationSYS &gs);         // ���ƴʷ���������
    void draw_encode(string data);                // ���Ʊ����ļ�
    void cleanLayout(QLayout *layout);            // �ݹ����layout�����֣�

  private:
    Ui::LexicalAnalysisProcedureClass ui;

  public slots:
    void getRegular_File_Path(); // ��ȡ������ʽ�ļ�
    void getSource_File_Path();  // ��ȡ����Դ���ļ�
    void comfirm();              // ȷ����ť�ۺ���
    void clear();                // ȡ���ۺ���
};
