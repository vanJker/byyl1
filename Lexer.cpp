#include "lexer.h"

LexicalAnalysisProcedure::LexicalAnalysisProcedure(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.get_regular_file_pushButton, SIGNAL(clicked(bool)), this, SLOT(getRegular_File_Path()));
    connect(ui.get_source_file_pushButton, SIGNAL(clicked(bool)), this, SLOT(getSource_File_Path()));
    connect(ui.run_pushButton, SIGNAL(clicked(bool)), this, SLOT(comfirm()));
    connect(ui.clear_pushButton, SIGNAL(clicked(bool)), this, SLOT(clear()));
}

LexicalAnalysisProcedure::~LexicalAnalysisProcedure()
{
}

void LexicalAnalysisProcedure::getRegular_File_Path()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(QStringLiteral("choose file"));
    fileDialog->setDirectory("./");
    fileDialog->setNameFilter(tr("File(*.txt*)"));
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->show();

    QString filepath;
    if (fileDialog->exec())
    {
        filepath = fileDialog->selectedFiles().join("");
        ui.regular_file_edit->setText(filepath);
        FileProcessor fp;
        fp.setFile_Name(filepath.toStdString());
        fp.readFile_Data();
    }
}

void LexicalAnalysisProcedure::getSource_File_Path()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(QStringLiteral("choose file"));
    fileDialog->setDirectory("./");
    // fileDialog->setNameFilter(tr("File(*.txt*)"));
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->show();

    QString filepath;
    if (fileDialog->exec())
    {
        filepath = fileDialog->selectedFiles().join("");
        ui.source_file_edit->setText(filepath);
        FileProcessor fp;
        fp.setFile_Name(filepath.toStdString());
        fp.readFile_Data();
    }
}

string getCodeFilename(string filename)
{
    string code_name = filename;
    int i = 0;
    for (i = code_name.length() - 1; i > 0; i--)
    {
        if (code_name[i] == '.')
            break;
    }
    code_name.erase(code_name.begin() + i, code_name.end());
    code_name += ".txt";
    return code_name;
}

void LexicalAnalysisProcedure::comfirm()
{
    initWidget();

    QString qstr = ui.regular_file_edit->text();
    string str = qstr.toStdString();
    QString sstr = ui.source_file_edit->text();

    FileProcessor file_processor;
    string fstr = str;
    file_processor.setFile_Name(fstr);
    string data = file_processor.readFile_Data();

    GenerationSYS gsys;
    gsys.setTable(data);
    draw_nfa_table(gsys);
    draw_dfa_table(gsys);
    draw_mindfa_table(gsys);

    gsys.setTotalTable();
    gsys.setSrc_filename(sstr.toStdString());
    gsys.setProgram();
    draw_totaldfa_table(gsys);
    draw_Program(gsys);

    // 编译运行
    char cwd[256];
    _getcwd(cwd, 256);
    char *path;
    if ((path = _getcwd(NULL, 0)) == NULL)
    {
        printf("Error message : _getcwd error\n");
    }
    else
    {
        printf("%s\n", path);
    }

    file_processor.writeFile_Data(gsys.getCode()); // 写入c文件
    string srcfilename = file_processor.getDesfile_name();
    string exename = file_processor.getExefile_name();
    string comorder = "gcc " + srcfilename + " -o " + exename;
    system(comorder.c_str());
    string runorder = "\"" + exename + ".exe\"";
    system(runorder.c_str());

    FileProcessor result_file;
    string result_filename = getCodeFilename(sstr.toStdString());
    result_file.setFile_Name(result_filename);
    draw_encode(result_file.readFile_Data());

    // gsys.compile(file_processor.getDesfile_name());
    ui.tabWidget->update();
}

void LexicalAnalysisProcedure::initWidget()
{
    cleanLayout(ui.nfa_area->layout());
    ui.nfa_area->update();
    cleanLayout(ui.dfa_area->layout());
    ui.dfa_area->update();
    cleanLayout(ui.mindfa_area->layout());
    ui.mindfa_area->update();
    cleanLayout(ui.total_tab->layout());
    ui.total_tab->update();

    ui.program_edit->clear();
    ui.encode_edit->clear();
}

// 递归清除layout（布局）
void LexicalAnalysisProcedure::cleanLayout(QLayout *layout)
{
    if (layout == nullptr)
    {
        return;
    }
    QLayoutItem *child = nullptr;
    while ((child = layout->takeAt(0)))
    {
        if (child->widget())
        {
            delete child->widget();
            // child->widget()->setParent(nullptr);
        }
        if (child->layout())
        {
            cleanLayout(child->layout());
        }
        // layout->removeItem(child);
        delete child;
    }
    layout->update(); // 更新，防止界面撕裂
    delete layout;
}

void LexicalAnalysisProcedure::clear()
{
    ui.regular_file_edit->clear();
    ui.source_file_edit->clear();
    initWidget();
    ui.tabWidget->update();
}

void LexicalAnalysisProcedure::draw_nfa_table(NFA &nfa)
{
    QVBoxLayout *vBox = new QVBoxLayout();
    ;
    QSplitter *vqs = new QSplitter(Qt::Vertical, ui.nfa_area);
    for (int i = 0; i < nfa.expsCount; i++)
    {
        QTableWidget *nfaTable = new QTableWidget();

        QStringList strList;
        // 生成nfa表
        // 设置行列
        int rowSize = nfa.nfaNodeAccount[i], colSize = nfa.key_value[i].size();
        nfaTable->setRowCount(rowSize);
        nfaTable->setColumnCount(colSize);

        // 设置表头
        for (unsigned int j = 0; j < nfa.key_value[i].size(); j++)
        {
            const char *ch;
            string str = "";
            str += nfa.value_key[i][j];
            ch = str.c_str();
            strList << tr(ch);
        }
        nfaTable->setHorizontalHeaderLabels(strList);

        for (int j = 0; j < rowSize; j++)
        {
            for (int k = 0; k < colSize; k++)
            {
                const char *ch;
                string str = "";
                str += nfa.getNFAunit(i, j + 1, k);
                ch = str.c_str();
                nfaTable->setItem(j, k, new QTableWidgetItem(tr(ch)));
            }
        }

        vqs->addWidget(nfaTable);
    }
    vBox->addWidget(vqs);
    ui.nfa_area->setLayout(vBox);
    ui.nfa_area->show();
}

void LexicalAnalysisProcedure::draw_dfa_table(DFA &dfa)
{
    QVBoxLayout *vBox = new QVBoxLayout();
    ;
    QSplitter *vqs = new QSplitter(Qt::Vertical, ui.dfa_area);
    for (int i = 0; i < dfa.expsCount; i++)
    {
        QTableWidget *dfaTable = new QTableWidget();

        QStringList strList;
        // 生成dfa表
        // 设置行列
        int rowSize = dfa.dfa_node_account[i], colSzie = dfa.key_value[i].size() - 1;
        dfaTable->setRowCount(rowSize);
        dfaTable->setColumnCount(colSzie);

        // 设置表头
        for (unsigned int j = 1; j < dfa.key_value[i].size(); j++)
        {
            const char *ch;
            string str = "";
            str += dfa.value_key[i][j];
            ch = str.c_str();
            strList << tr(ch);
        }
        dfaTable->setHorizontalHeaderLabels(strList);
        // 设置侧表头
        strList.clear();
        for (int j = 1; j <= rowSize; j++)
        {
            const char *ch;
            string str = "";
            str += "{" + dfa.getSet(i, j) + "}";
            ch = str.c_str();
            strList << tr(ch);
        }
        dfaTable->setVerticalHeaderLabels(strList);

        for (int j = 0; j < rowSize; j++)
        {
            for (int k = 0; k < colSzie; k++)
            {
                const char *ch;
                string str = "";
                str += dfa.getDFA(i, j + 1, k + 1);
                ch = str.c_str();
                dfaTable->setItem(j, k, new QTableWidgetItem(tr(ch)));
            }
        }

        vqs->addWidget(dfaTable);
    }
    vBox->addWidget(vqs);
    ui.dfa_area->setLayout(vBox);
    ui.dfa_area->show();
}

void LexicalAnalysisProcedure::draw_mindfa_table(MinDFA &mdfa)
{
    QVBoxLayout *vBox = new QVBoxLayout();
    ;
    QSplitter *vqs = new QSplitter(Qt::Vertical, ui.mindfa_area);
    for (int i = 0; i < mdfa.expsCount; i++)
    {
        QTableWidget *mindfaTable = new QTableWidget();

        QStringList strList;
        // 生成mindfa表
        // 设置行列
        int rowSize = mdfa.mdfaNodeAccount[i],
            colSzie = mdfa.key_value[i].size() - 1; // 去掉'#'
        mindfaTable->setRowCount(rowSize);
        mindfaTable->setColumnCount(colSzie);

        // 设置表头
        for (unsigned int j = 1; j < mdfa.key_value[i].size(); j++)
        {
            const char *ch;
            string str = "";
            str += mdfa.value_key[i][j];
            ch = str.c_str();
            strList << tr(ch);
        }
        mindfaTable->setHorizontalHeaderLabels(strList);

        for (int j = 0; j < rowSize; j++)
        {
            for (int k = 0; k < colSzie; k++)
            {
                const char *ch;
                string str = "";
                str += mdfa.getMinDFA(i, j + 1, k + 1);
                ch = str.c_str();
                mindfaTable->setItem(j, k, new QTableWidgetItem(tr(ch)));
            }
        }
        vqs->addWidget(mindfaTable);
    }
    vBox->addWidget(vqs);
    ui.mindfa_area->setLayout(vBox);
    ui.mindfa_area->show();
}

void LexicalAnalysisProcedure::draw_totaldfa_table(TotalDFA &totaldfa)
{
    QVBoxLayout *vBox = new QVBoxLayout();
    ;
    QSplitter *vqs = new QSplitter(Qt::Vertical, ui.dfa_tab);
    QTableWidget *totaldfaTable = new QTableWidget();

    QStringList strList;
    // 生成totaldfa表
    // 设置行列
    int rowSize = totaldfa.tdfaNodeAccount,
        colSzie = totaldfa.tkeyVal.size() - 1; // 去掉'#'
    totaldfaTable->setRowCount(rowSize);
    totaldfaTable->setColumnCount(colSzie);

    // 设置表头
    for (unsigned int j = 1; j < totaldfa.tkeyVal.size(); j++)
    {
        const char *ch;
        string str = "";
        str += totaldfa.valTkey[j];
        ch = str.c_str();
        strList << tr(ch);
    }
    totaldfaTable->setHorizontalHeaderLabels(strList);

    for (int j = 0; j < rowSize; j++)
    {
        for (int k = 0; k < colSzie; k++)
        {
            const char *ch;
            string str = "";
            str += totaldfa.getTotalDFA(j + 1, k + 1);
            ch = str.c_str();
            totaldfaTable->setItem(j, k, new QTableWidgetItem(tr(ch)));
        }
    }
    vqs->addWidget(totaldfaTable);

    vBox->addWidget(vqs);
    ui.total_tab->setLayout(vBox);
    ui.total_tab->show();
}

void LexicalAnalysisProcedure::draw_Program(GenerationSYS &gs)
{
    string code = gs.getCode();
    QString showCode = QString::fromStdString(code);
    ui.program_edit->setText(showCode);
}

void LexicalAnalysisProcedure::draw_encode(string data)
{
    string encode = data;
    QString showEncode = QString::fromStdString(encode);
    ui.encode_edit->setText(showEncode);
}
