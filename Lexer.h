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
    void initWidget();                            // 初始化对应窗口内容
    void draw_nfa_table(NFA &nfa);                // 绘制nfa部分状态表
    void draw_dfa_table(DFA &dfa);                // 绘制dfa部分状态表
    void draw_mindfa_table(MinDFA &mdfa);         // 绘制dfa部分状态表
    void draw_totaldfa_table(TotalDFA &totaldfa); // 绘制总dfa部分状态表
    void draw_Program(GenerationSYS &gs);         // 绘制词法分析程序
    void draw_encode(string data);                // 绘制编码文件
    void cleanLayout(QLayout *layout);            // 递归清除layout（布局）

  private:
    Ui::LexicalAnalysisProcedureClass ui;

  public slots:
    void getRegular_File_Path(); // 获取正则表达式文件
    void getSource_File_Path();  // 获取测试源码文件
    void comfirm();              // 确定按钮槽函数
    void clear();                // 取消槽函数
};
