#include <QtWidgets/QApplication>

#include "CodeGen.h"
#include "DFA.h"
#include "FileProcessor.h"
#include "Lexer.h"
#include "MinDFA.h"
#include "NFA.h"
#include "TotalDFA.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LexicalAnalysisProcedure w;
    w.show();

    return a.exec();
}
