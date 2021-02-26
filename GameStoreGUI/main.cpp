#include "GameStoreGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameStoreGUI w;
    w.show();
    return a.exec();
}
