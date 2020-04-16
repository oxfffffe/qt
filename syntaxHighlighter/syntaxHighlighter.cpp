#include "syntaxHighlighter.hpp"
#include "../ui_mainwindow.h"
#include <QRegularExpression>
//#include <QPlainTextEdit>
//#include "QPalette"
#include "colorDataTypes.hpp"


void mySyntaxHighLighter::highlightBlock(const QString &text) {

    enum {
        NormalState = -1, CStyleComment
    };


    int state = previousBlockState();
    int start = 0;


/* * * * * * * * * * * * *  NUMBERS * * * * * * * * * * * * * * */

    for (int i = 0; i < text.length(); ++i) {

        if (text.at(i).isNumber()) {
            setFormat(i, 1, "#D33682");
        }
    }


/* * * * * * * * * * * * *  COMMENTS * * * * * * * * * * * * * * */

    for (int i = 0; i < text.length(); ++i) {

        if (state == CStyleComment) {

            if (text.mid(i, 2) == "*/") {

                state = NormalState;
                setFormat(start, i - start + 2, Qt::gray);
            }
        } else {
            if (text.mid(i, 2) == "//") {

                setFormat(i, text.length() - i, "#707593");
                //break;
            } else if (text.mid(i, 2) == "/*") {

                start = i;
                state = CStyleComment;
            }
        }

    }



/* * * * * * * * * * * * *  ANGLE BRACKETS * * * * * * * * * * * * * * */


    for (int i = 0; i < text.length(); ++i) {

        if (state == CStyleComment) {

            if (text.mid(i, 1) == ">") {

                state = NormalState;
                setFormat(start, i - start + 1, "#859900");
            }
        } else {
            if (text.mid(i, 1) == "<") {

                start = i;
                state = CStyleComment;
            }
        }

    }

/* * * * * * * * * * * * *  " " * * * * * * * * * * * * * * */


    for (int i = 0; i < text.length(); ++i) {

        if (state == CStyleComment) {

            if (text.mid(i, 1) == '"') {

                state = NormalState;
                setFormat(start, i - start + 1, "#859900");
            }
        } else {
            if (text.mid(i, 1) == '"') {

                start = i;
                state = CStyleComment;
            }
        }

    }

/* * * * * * * * * * * * *  KEYWORDS * * * * * * * * * * * * * * */


    /* DATA TYPES */




    for (int i = 0; i < text.length(); ++i) {


        /* ACCESS MODIFIERS*/
        if (text.mid(i, Kstatic.size()) == "static " || text.mid(i, Kprivate.size()) == "private " ||
            text.mid(i, Kpublic.size()) == "public ") {
            setFormat(i, 7, Qt::darkCyan);
            //break
        }
        if (text.mid(i, Kprotected.size()) == "protected ") {
            setFormat(i, Kprotected.size(), Qt::darkCyan);
        }

        /* DADA TYPES */

        //            if (text.startsWith(typeInt)) {
        //                setFormat(i, typeInt.size(), Qt::darkYellow);
        //                break;
        //            }

        if (text.mid(i, typeInt.size()) == "int ")
            setFormat(i, typeInt.size(), Qt::darkYellow);

        if (text.mid(i, typeChar.size()) == "char ")
            setFormat(i, typeChar.size(), Qt::darkYellow);

        if (text.mid(i, typeShort.size()) == "short ")

            setFormat(i, typeShort.size(), Qt::darkYellow);
        if (text.mid(i, typeLong.size()) == "long ")

            setFormat(i, typeLong.size(), Qt::darkYellow);
        if (text.mid(i, typeAuto.size()) == "auto ")

            setFormat(i, typeAuto.size(), Qt::darkYellow);
        if (text.mid(i, typeFloat.size()) == "float ")

            setFormat(i, typeFloat.size(), Qt::darkYellow);
        if (text.mid(i, typeDouble.size()) == "double ")

            setFormat(i, typeDouble.size(), Qt::darkYellow);

        if (text.mid(i, typeBoolean.size()) == "bool ")
            setFormat(i, typeBoolean.size(), Qt::darkYellow);

        if (text.startsWith("#include")) {
            setFormat(i, 8, "#0E6A87");
            break;
        }




//            if (text.mid(i, Kinclude.size()) == "#include ") {
//                setFormat(i, Kinclude.size(), "#0E6A87");
//            }
    }
}

void mySyntaxHighLighter::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!this->edit->isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::darkBlue).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = this->edit->textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    this->edit->setExtraSelections(extraSelections);
}

mySyntaxHighLighter::mySyntaxHighLighter(QTextDocument *document, QPlainTextEdit *edit) :
        QSyntaxHighlighter(document) {
    this->edit = edit;
    connect(edit, &edit->cursorPositionChanged, this, &mySyntaxHighLighter::highlightCurrentLine);
}