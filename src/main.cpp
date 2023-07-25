#include <iostream>
#include <QApplication>

#include "TestDragger.h"

int main(int argc, char **argv)
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    MainWindow wnd;
    wnd.init();

    TestTranslationDragger(&wnd);
   
    wnd.show();
    return app.exec();
}