#include "optionwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OptionWindow w;

    w.show();

    return a.exec();
}
